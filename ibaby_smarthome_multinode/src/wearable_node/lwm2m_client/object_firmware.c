/*******************************************************************************
 *
 * Copyright (c) 2013, 2014 Intel Corporation and others.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Julien Vermillard - initial implementation
 *    Fabien Fleutot - Please refer to git log
 *    David Navarro, Intel Corporation - Please refer to git log
 *    Bosch Software Innovations GmbH - Please refer to git log
 *    Pascal Rieux - Please refer to git log
 *
 *******************************************************************************/

/*
 * This object is single instance only, and provide firmware upgrade functionality.
 * Object ID is 5.
 */

/*
 * resources:
 * 0 package                   write
 * 1 package url               write
 * 2 update                    exec
 * 3 state                     read
 * 4 update supported objects  read/write
 * 5 update result             read
 */

#include "liblwm2m.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "embARC.h"
#include "embARC_debug.h"


/** Resource Id's: */
#define RES_M_PACKAGE                   0
#define RES_M_PACKAGE_URI               1
#define RES_M_UPDATE                    2
#define RES_M_STATE                     3
#define RES_O_UPDATE_SUPPORTED_OPJECTS  4
#define RES_M_UPDATE_RESULT             5
#define RES_O_PKG_NAME                  6
#define RES_O_PKG_VERSION               7

typedef struct
{
	uint8_t state;
	uint8_t supported;
	uint8_t result;
} firmware_data_t;


static uint8_t prv_firmware_read(uint16_t instanceId,
                                 int * numDataP,
                                 lwm2m_tlv_t ** dataArrayP,
                                 lwm2m_object_t * objectP)
{
	int i;
	uint8_t result;
	firmware_data_t * data = (firmware_data_t*)(objectP->userData);

	/* this is a single instance object */
	if (instanceId != 0)
	{
		return COAP_404_NOT_FOUND;
	}

	/* is the server asking for the full object? */
	if (*numDataP == 0)
	{
		*dataArrayP = lwm2m_tlv_new(3);
		if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
		*numDataP = 3;
		(*dataArrayP)[0].id = 3;
		(*dataArrayP)[1].id = 4;
		(*dataArrayP)[2].id = 5;
	}

	i = 0;
	do
	{
		switch ((*dataArrayP)[i].id)
		{
		case RES_M_PACKAGE:
		case RES_M_PACKAGE_URI:
		case RES_M_UPDATE:
			result = COAP_405_METHOD_NOT_ALLOWED;
			break;

		case RES_M_STATE:
			/* firmware update state (int) */
			lwm2m_tlv_encode_int(data->state, *dataArrayP + i);
			(*dataArrayP)[i].type = LWM2M_TYPE_RESOURCE;

			if (0 != (*dataArrayP)[i].length) result = COAP_205_CONTENT;
			else result = COAP_500_INTERNAL_SERVER_ERROR;

			break;

		case RES_O_UPDATE_SUPPORTED_OPJECTS:
			lwm2m_tlv_encode_int(data->supported, *dataArrayP + i);
			(*dataArrayP)[i].type = LWM2M_TYPE_RESOURCE;

			if (0 != (*dataArrayP)[i].length) result = COAP_205_CONTENT;
			else result = COAP_500_INTERNAL_SERVER_ERROR;

			break;

		case RES_M_UPDATE_RESULT:
			lwm2m_tlv_encode_int(data->result, *dataArrayP + i);
			(*dataArrayP)[i].type = LWM2M_TYPE_RESOURCE;

			if (0 != (*dataArrayP)[i].length) result = COAP_205_CONTENT;
			else result = COAP_500_INTERNAL_SERVER_ERROR;

			break;

		default:
			result = COAP_404_NOT_FOUND;
		}

		i++;
	} while (i < *numDataP && result == COAP_205_CONTENT);

	return result;
}

static uint8_t prv_firmware_write(uint16_t instanceId,
                                  int numData,
                                  lwm2m_tlv_t * dataArray,
                                  lwm2m_object_t * objectP)
{
	int i, j;
	bool bvalue;
	uint8_t result;
	firmware_data_t * data = (firmware_data_t*)(objectP->userData);

	FIL dstfile, tempfile;
	FRESULT err;
	char tempname[20];
	uint8_t filesum, filenum, cnt_fh = 0;
	char sumstr[10], numstr[10];
	uint8_t res = 0;
	uint32_t br, bw;
	uint32_t cnt;
	uint8_t buff[4096];
	uint32_t blen = sizeof(buff);

	static uint16_t file_ota_cnt;      /*!< counter for number of files transported in OTA by LwM2M */

	/* this is a single instance object */
	if (instanceId != 0)
	{
		return COAP_404_NOT_FOUND;
	}

	i = 0;

	do
	{
		switch (dataArray[i].id)
		{
		case RES_M_PACKAGE:
			/* inline firmware binary */

			/* get the sum number of the temp files */
			j = 0;
			for (;;)
			{
				if (*dataArray->value == '\n')
				{
					dataArray->value++;
					cnt_fh++;
					break;
				}
				sumstr[j++] = *dataArray->value;
				dataArray->value++;
				cnt_fh++;
			}
			sumstr[j] = '\n';

			/* get the number of this temp file */
			j = 0;
			for (;;)
			{
				if (*dataArray->value == '\n')
				{
					dataArray->value++;
					cnt_fh++;
					break;
				}
				numstr[j++] = *dataArray->value;
				dataArray->value++;
				cnt_fh++;
			}
			numstr[j] = '\n';

			filesum = atoi(sumstr);
			filenum = atoi(numstr);
			printf("filenum = %d\n", filenum);

			/* doesn't support temp format file create */
			sprintf(tempname, "boot%d.bin", filenum);

			/* receive and save temp file as boot0.bin */
			if (data->state == 1 || data->state == 3) {
				if ((err = f_open(&tempfile, tempname, FA_WRITE | FA_CREATE_ALWAYS))) {
					printf("create temp file %s err : %d\n", tempname, err);
					break;
				}
				f_write(&tempfile, dataArray->value, dataArray->length - cnt_fh, &cnt);
				data->state = 2;
			} else {
				f_write(&tempfile, dataArray->value, dataArray->length - cnt_fh, &cnt);
			}
			if (cnt != 256) {
				data->state = 3;
				printf("receive temp file: %s successfully\n", tempname);
				f_close(&tempfile);
				file_ota_cnt++;
				printf("file_ota_cnt = %d\n\n", file_ota_cnt);
			}

			/* merge all temp file into boot.bin and replace previous one */
			if (file_ota_cnt == filesum)
			{
				file_ota_cnt = 0;
				if ((err = f_open(&dstfile, "boot.bin", FA_CREATE_ALWAYS | FA_WRITE))) {
					printf("open file boot.bin err : %d\n", err);
					break;
				}

				printf("\n***********************************");
				printf("\n    merging file, wait a moment");
				printf("\n***********************************\n");

				for (int i = 0; i < filesum; i++)
				{
					sprintf(tempname, "boot%d.bin", i);

					if ((err = f_open(&tempfile, tempname, FA_OPEN_EXISTING | FA_READ))) {
						printf("\nopen file %s err : %d\n", tempname, err);
						break;
					}

					for (;;)
					{
						/* Read a chunk of source file */
						res = f_read(&tempfile, buff, blen, &br);
						if (res || br == 0) break; /* error or eof */

						/* Write it to the destination file */
						res = f_write(&dstfile, buff, br, &bw);
						if (res || bw < br) break; /* error or disk full */
					}
					file_ota_cnt++;

					f_close(&tempfile);

					/* delete temp file like boot0.bin */
					if (f_unlink(tempname) != FR_OK)
						printf("delete file %s failed\n", tempname);
				}

				f_close(&dstfile);
				if (file_ota_cnt == filesum)
				{
					printf("\nfirmware update online completed\r\n\n");
					result = COAP_204_CHANGED;
				} else {
					printf("\nfirmware update online failed\r\n\n");
					result = COAP_400_BAD_REQUEST;
				}
				file_ota_cnt = 0;
			}

			if (file_ota_cnt >= filesum)
			{
				file_ota_cnt = 0;
			}
			break;

		case RES_M_PACKAGE_URI:
			/* URL for download the firmware */
			result = COAP_204_CHANGED;
			break;

		case RES_O_UPDATE_SUPPORTED_OPJECTS:
			if (lwm2m_tlv_decode_bool(&dataArray[i], &bvalue) == 1)
			{
				data->supported = bvalue ? 1 : 0;
				result = COAP_204_CHANGED;
			}
			else
			{
				result = COAP_400_BAD_REQUEST;
			}
			break;

		default:
			result = COAP_405_METHOD_NOT_ALLOWED;
		}

		i++;
	} while (i < numData && result == COAP_204_CHANGED);

	return result;
}

static uint8_t prv_firmware_execute(uint16_t instanceId,
                                    uint16_t resourceId,
                                    uint8_t * buffer,
                                    int length,
                                    lwm2m_object_t * objectP)
{
	firmware_data_t * data = (firmware_data_t*)(objectP->userData);

	/* this is a single instance object */
	if (instanceId != 0)
	{
		return COAP_404_NOT_FOUND;
	}

	if (length != 0) return COAP_400_BAD_REQUEST;

	/* for execute callback, resId is always set */
	switch (resourceId)
	{
	case RES_M_UPDATE:
		if (data->state == 1)
		{
			EMBARC_PRINTF("\n\t FIRMWARE UPDATE\r\n\n");
			/* trigger your firmware download and update logic */
			data->state = 2;
			return COAP_204_CHANGED;
		}
		else
		{
			/* firmware update already running */
			return COAP_400_BAD_REQUEST;
		}
	default:
		return COAP_405_METHOD_NOT_ALLOWED;
	}
}

static void prv_firmware_close(lwm2m_object_t * objectP) {
	if (NULL != objectP->userData)
	{
		lwm2m_free(objectP->userData);
		objectP->userData = NULL;
	}
	if (NULL != objectP->instanceList)
	{
		lwm2m_free(objectP->instanceList);
		objectP->instanceList = NULL;
	}
}

void display_firmware_object(lwm2m_object_t * object)
{
#ifdef WITH_LOGS
	firmware_data_t * data = (firmware_data_t *)object->userData;
	EMBARC_PRINTF("  /%u: Firmware object:\r\n", object->objID);
	if (NULL != data)
	{
		EMBARC_PRINTF("    state: %u, supported: %u, result: %u\r\n",
		              data->state, data->supported, data->result);
	}
#endif
}


lwm2m_object_t * get_object_firmware(void)
{
	/*
	 * The get_object_firmware function create the object itself and return a pointer to the structure that represent it.
	 */
	lwm2m_object_t * firmwareObj;

	firmwareObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

	if (NULL != firmwareObj)
	{
		memset(firmwareObj, 0, sizeof(lwm2m_object_t));

		/*
		 * It assigns its unique ID
		 * The 5 is the standard ID for the optional object "Object firmware".
		 */
		firmwareObj->objID = LWM2M_FIRMWARE_UPDATE_OBJECT_ID;

		/*
		 * and its unique instance
		 *
		 */
		firmwareObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
		if (NULL != firmwareObj->instanceList)
		{
			memset(firmwareObj->instanceList, 0, sizeof(lwm2m_list_t));
		}
		else
		{
			lwm2m_free(firmwareObj);
			return NULL;
		}

		/*
		 * And the private function that will access the object.
		 * Those function will be called when a read/write/execute query is made by the server. In fact the library don't need to
		 * know the resources of the object, only the server does.
		 */
		firmwareObj->readFunc    = prv_firmware_read;
		firmwareObj->writeFunc   = prv_firmware_write;
		firmwareObj->executeFunc = prv_firmware_execute;
		firmwareObj->closeFunc   = prv_firmware_close;
		firmwareObj->userData    = lwm2m_malloc(sizeof(firmware_data_t));

		/*
		 * Also some user data can be stored in the object with a private structure containing the needed variables
		 */
		if (NULL != firmwareObj->userData)
		{
			((firmware_data_t*)firmwareObj->userData)->state = 1;
			((firmware_data_t*)firmwareObj->userData)->supported = 0;
			((firmware_data_t*)firmwareObj->userData)->result = 0;
		}
		else
		{
			lwm2m_free(firmwareObj);
			firmwareObj = NULL;
		}
	}

	return firmwareObj;
}
