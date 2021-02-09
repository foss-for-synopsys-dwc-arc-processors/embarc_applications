/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/
#include <stdlib.h>
#include <math.h>
#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "mli_api.h"

#include "ref_in.h"
#include "model.h"

unsigned char kSingleIn[IN_POINTS];
//const char * file =  "test_txt/defect/211.txt";
//const float kSingleOutRef[OUT_POINTS] = OUT_PROB_12;
const unsigned char * IMAGE_LIST[22] = {\
  IN_IMG_17,\
  IN_IMG_81,\
  IN_IMG_43,\
  IN_IMG_7,\
  IN_IMG_75,\
  IN_IMG_50,\
  IN_IMG_91,\
  IN_IMG_66,\
  IN_IMG_112,\
  IN_IMG_21,\
  IN_IMG_46,\
  IN_IMG_211,\
  IN_IMG_116,\
  IN_IMG_189,\
  IN_IMG_49,\
  IN_IMG_136,\
  IN_IMG_84,\
  IN_IMG_117,\
  IN_IMG_14,\
  IN_IMG_170,\
  IN_IMG_119,\
  IN_IMG_121\
};

/*const unsigned char * IMAGE_LIST[180] = {\
  IN_IMG_211,\
  IN_IMG_1356,\
  IN_IMG_659,\
  IN_IMG_381,\
  IN_IMG_3532,\
  IN_IMG_171,\
  IN_IMG_1302,\
  IN_IMG_597,\
  IN_IMG_1293,\
  IN_IMG_954,\
  IN_IMG_1474,\
  IN_IMG_274,\
  IN_IMG_1423,\
  IN_IMG_116,\
  IN_IMG_460,\
  IN_IMG_562,\
  IN_IMG_1459,\
  IN_IMG_393,\
  IN_IMG_317,\
  IN_IMG_933,\
  IN_IMG_1931,\
  IN_IMG_1094,\
  IN_IMG_895,\
  IN_IMG_1453,\
  IN_IMG_172,\
  IN_IMG_842,\
  IN_IMG_3751,\
  IN_IMG_1360,\
  IN_IMG_3753,\
  IN_IMG_365,\
  IN_IMG_870,\
  IN_IMG_1106,\
  IN_IMG_2669,\
  IN_IMG_135,\
  IN_IMG_992,\
  IN_IMG_575,\
  IN_IMG_1089,\
  IN_IMG_574,\
  IN_IMG_963,\
  IN_IMG_973,\
  IN_IMG_3296,\
  IN_IMG_1308,\
  IN_IMG_189,\
  IN_IMG_49,\
  IN_IMG_1373,\
  IN_IMG_646,\
  IN_IMG_3309,\
  IN_IMG_760,\
  IN_IMG_1268,\
  IN_IMG_957,\
  IN_IMG_2904,\
  IN_IMG_400,\
  IN_IMG_1550,\
  IN_IMG_685,\
  IN_IMG_3766,\
  IN_IMG_3821,\
  IN_IMG_395,\
  IN_IMG_896,\
  IN_IMG_1560,\
  IN_IMG_688,\
  IN_IMG_143,\
  IN_IMG_69,\
  IN_IMG_1390,\
  IN_IMG_2350,\
  IN_IMG_1472,\
  IN_IMG_396,\
  IN_IMG_893,\
  IN_IMG_3741,\
  IN_IMG_3709,\
  IN_IMG_1167,\
  IN_IMG_1155,\
  IN_IMG_711,\
  IN_IMG_387,\
  IN_IMG_1370,\
  IN_IMG_864,\
  IN_IMG_1456,\
  IN_IMG_488,\
  IN_IMG_1247,\
  IN_IMG_490,\
  IN_IMG_1080,\
  IN_IMG_1022,\
  IN_IMG_2901,\
  IN_IMG_136,\
  IN_IMG_3705,\
  IN_IMG_3884,\
  IN_IMG_1056,\
  IN_IMG_1366,\
  IN_IMG_1053,\
  IN_IMG_1438,\
  IN_IMG_697,\
  IN_IMG_1538,\
  IN_IMG_1261,\
  IN_IMG_1555,\
  IN_IMG_569,\
  IN_IMG_313,\
  IN_IMG_1530,\
  IN_IMG_988,\
  IN_IMG_927,\
  IN_IMG_1372,\
  IN_IMG_2831,\
  IN_IMG_1427,\
  IN_IMG_1258,\
  IN_IMG_1276,\
  IN_IMG_319,\
  IN_IMG_524,\
  IN_IMG_117,\
  IN_IMG_1440,\
  IN_IMG_340,\
  IN_IMG_1303,\
  IN_IMG_628,\
  IN_IMG_922,\
  IN_IMG_62,\
  IN_IMG_1397,\
  IN_IMG_743,\
  IN_IMG_3823,\
  IN_IMG_695,\
  IN_IMG_1110,\
  IN_IMG_1082,\
  IN_IMG_1170,\
  IN_IMG_606,\
  IN_IMG_1262,\
  IN_IMG_14,\
  IN_IMG_601,\
  IN_IMG_1175,\
  IN_IMG_571,\
  IN_IMG_493,\
  IN_IMG_719,\
  IN_IMG_713,\
  IN_IMG_1050,\
  IN_IMG_170,\
  IN_IMG_128,\
  IN_IMG_3757,\
  IN_IMG_1419,\
  IN_IMG_990,\
  IN_IMG_2611,\
  IN_IMG_377,\
  IN_IMG_1188,\
  IN_IMG_386,\
  IN_IMG_56,\
  IN_IMG_1519,\
  IN_IMG_1379,\
  IN_IMG_876,\
  IN_IMG_403,\
  IN_IMG_841,\
  IN_IMG_1260,\
  IN_IMG_894,\
  IN_IMG_1296,\
  IN_IMG_684,\
  IN_IMG_795,\
  IN_IMG_456,\
  IN_IMG_923,\
  IN_IMG_837,\
  IN_IMG_953,\
  IN_IMG_1380,\
  IN_IMG_746,\
  IN_IMG_1592,\
  IN_IMG_3789,\
  IN_IMG_198,\
  IN_IMG_527,\
  IN_IMG_2218,\
  IN_IMG_1242,\
  IN_IMG_1346,\
  IN_IMG_580,\
  IN_IMG_1398,\
  IN_IMG_721,\
  IN_IMG_1039,\
  IN_IMG_425,\
  IN_IMG_1691,\
  IN_IMG_376,\
  IN_IMG_700,\
  IN_IMG_435,\
  IN_IMG_3839,\
  IN_IMG_119,\
  IN_IMG_1162,\
  IN_IMG_121,\
  IN_IMG_3734,\
  IN_IMG_1527,\
  IN_IMG_858,\
  IN_IMG_680,\
  IN_IMG_1000,
};

const unsigned char * IMAGE_LIST[168] = {\
  IN_IMG_46,\
  IN_IMG_211,\
  IN_IMG_1356,\
  IN_IMG_659,\
  IN_IMG_381,\
  IN_IMG_3532,\
  IN_IMG_171,\
  IN_IMG_1302,\
  IN_IMG_597,\
  IN_IMG_1293,\
  IN_IMG_954,\
  IN_IMG_1474,\
  IN_IMG_274,\
  IN_IMG_1423,\
  IN_IMG_116,\
  IN_IMG_460,\
  IN_IMG_562,\
  IN_IMG_1459,\
  IN_IMG_393,\
  IN_IMG_317,\
  IN_IMG_933,\
  IN_IMG_1931,\
  IN_IMG_1094,\
  IN_IMG_895,\
  IN_IMG_1453,\
  IN_IMG_172,\
  IN_IMG_842,\
  IN_IMG_1360,\
  IN_IMG_365,\
  IN_IMG_870,\
  IN_IMG_1106,\
  IN_IMG_2669,\
  IN_IMG_135,\
  IN_IMG_992,\
  IN_IMG_575,\
  IN_IMG_1089,\
  IN_IMG_574,\
  IN_IMG_963,\
  IN_IMG_973,\
  IN_IMG_3296,\
  IN_IMG_1308,\
  IN_IMG_189,\
  IN_IMG_49,\
  IN_IMG_1373,\
  IN_IMG_646,\
  IN_IMG_3309,\
  IN_IMG_760,\
  IN_IMG_1268,\
  IN_IMG_957,\
  IN_IMG_2904,\
  IN_IMG_400,\
  IN_IMG_1550,\
  IN_IMG_685,\
  IN_IMG_395,\
  IN_IMG_896,\
  IN_IMG_1560,\
  IN_IMG_688,\
  IN_IMG_143,\
  IN_IMG_69,\
  IN_IMG_1390,\
  IN_IMG_2350,\
  IN_IMG_1472,\
  IN_IMG_396,\
  IN_IMG_893,\
  IN_IMG_1167,\
  IN_IMG_1155,\
  IN_IMG_711,\
  IN_IMG_387,\
  IN_IMG_1370,\
  IN_IMG_864,\
  IN_IMG_1456,\
  IN_IMG_488,\
  IN_IMG_1247,\
  IN_IMG_490,\
  IN_IMG_1080,\
  IN_IMG_1022,\
  IN_IMG_2901,\
  IN_IMG_136,\
  IN_IMG_1056,\
  IN_IMG_1366,\
  IN_IMG_1053,\
  IN_IMG_1438,\
  IN_IMG_697,\
  IN_IMG_1538,\
  IN_IMG_1261,\
  IN_IMG_1555,\
  IN_IMG_569,\
  IN_IMG_313,\
  IN_IMG_1530,\
  IN_IMG_988,\
  IN_IMG_927,\
  IN_IMG_1372,\
  IN_IMG_2831,\
  IN_IMG_1427,\
  IN_IMG_1258,\
  IN_IMG_1276,\
  IN_IMG_319,\
  IN_IMG_524,\
  IN_IMG_117,\
  IN_IMG_1440,\
  IN_IMG_340,\
  IN_IMG_1303,\
  IN_IMG_628,\
  IN_IMG_922,\
  IN_IMG_62,\
  IN_IMG_1397,\
  IN_IMG_743,\
  IN_IMG_695,\
  IN_IMG_1110,\
  IN_IMG_1082,\
  IN_IMG_1170,\
  IN_IMG_606,\
  IN_IMG_1262,\
  IN_IMG_14,\
  IN_IMG_601,\
  IN_IMG_1175,\
  IN_IMG_571,\
  IN_IMG_493,\
  IN_IMG_719,\
  IN_IMG_713,\
  IN_IMG_1050,\
  IN_IMG_170,\
  IN_IMG_128,\
  IN_IMG_1419,\
  IN_IMG_990,\
  IN_IMG_2611,\
  IN_IMG_377,\
  IN_IMG_1188,\
  IN_IMG_386,\
  IN_IMG_56,\
  IN_IMG_1519,\
  IN_IMG_1379,\
  IN_IMG_876,\
  IN_IMG_403,\
  IN_IMG_841,\
  IN_IMG_1260,\
  IN_IMG_894,\
  IN_IMG_1296,\
  IN_IMG_684,\
  IN_IMG_795,\
  IN_IMG_456,\
  IN_IMG_923,\
  IN_IMG_837,\
  IN_IMG_953,\
  IN_IMG_1380,\
  IN_IMG_746,\
  IN_IMG_1592,\
  IN_IMG_198,\
  IN_IMG_527,\
  IN_IMG_2218,\
  IN_IMG_1242,\
  IN_IMG_1346,\
  IN_IMG_580,\
  IN_IMG_1398,\
  IN_IMG_721,\
  IN_IMG_1039,\
  IN_IMG_425,\
  IN_IMG_1691,\
  IN_IMG_376,\
  IN_IMG_700,\
  IN_IMG_435,\
  IN_IMG_119,\
  IN_IMG_1162,\
  IN_IMG_121,\
  IN_IMG_1527,\
  IN_IMG_858,\
  IN_IMG_680,\
  IN_IMG_1000\
};*/


//========================================================================================
//
// Other internal functions and routines
//
//========================================================================================

//================================================================================
// Transform MLI FX tensor to float array
//=================================================================================
static mli_status mli_hlp_fx_tensor_to_float (const mli_tensor * src, float *dst, uint32_t dst_size) {
	uint32_t elem_num = mli_hlp_count_elem_num(src, 0);
	if (elem_num > dst_size)
		return MLI_STATUS_LENGTH_ERROR;
	if (elem_num == 0)
		return MLI_STATUS_BAD_TENSOR;

	const float scale_val = 1.0f / (float) (1u << (src->el_params.fx.frac_bits));
	if (src->el_type == MLI_EL_FX_16) {
		int16_t *src_arr = src->data;
		for (int idx = 0; idx < elem_num; idx++)
			dst[idx] = (float) (scale_val * src_arr[idx]);
	} else {
		int8_t *src_arr = src->data;
		for (int idx = 0; idx < elem_num; idx++)
			dst[idx] = (float) (scale_val * src_arr[idx]);
	}
	return MLI_STATUS_OK;
}

//========================================================================================
// Image loading from txt
//========================================================================================

static void load_image(unsigned char * image_, const char * fname) {
	int num;
	int r, i=0;
	FILE * file;
	file = fopen("test_txt/defect/211.txt", "r");
	if (file == NULL) {
    	printf("File does not exist!");
    	exit(1);
	}
	while ((r = fscanf(file, "%d", &num)) != EOF) {
        image_[i] = INQ(num);
        //printf("number %d: %d\n", i, num);
        ++i;
    }
  
    fclose(file);

}

static void load_image_de(unsigned char * image_, const unsigned char * in) {

	int i;
	for(i=0; i<40*20; ++i) {
		image_[i] = in[i];
	}

}

//========================================================================================
// Image pre-processing for net
//========================================================================================
static void image_preprocessing(const void * image_, mli_tensor * net_input_) {
	const unsigned char * in = image_;
	d_type * const dst = (d_type * const)net_input_->data;

	// Copying data  to input tensor with subtraction of average.
	// Data shift may be required depending on tensor format
	if (net_input_->el_params.fx.frac_bits == 7) {
		for (int idx = 0; idx < IN_POINTS; idx++) {
			dst[idx] = (d_type)((int)in[idx]-128);
		}
	} else if (net_input_->el_params.fx.frac_bits > 7) {
		int shift_left = net_input_->el_params.fx.frac_bits - 7;
		for (int idx = 0; idx < IN_POINTS; idx++) {
			dst[idx] = (d_type)((int)in[idx]-128) << shift_left;
		}
	} else {
		int shift_right = 7 - net_input_->el_params.fx.frac_bits;
		for (int idx = 0; idx < IN_POINTS; idx++) {
			dst[idx] = (d_type)((int)in[idx]-128)  >> shift_right; // w/o rounding
		}
	}
}
//================================================================================
// Measure various error metrics between two float vectors
//=================================================================================
static int measure_acc(const float * pred_vec) {
	if(pred_vec[0] > pred_vec[1]) {
		return 0;
	}
	else {
		return 1;
	}
}

int main(void) {
	int i, defect=0, good=0;
	EMBARC_PRINTF("\n\n\n\n");
	EMBARC_PRINTF("\n\n\n\n");
	EMBARC_PRINTF("\n\n\n\n");
	EMBARC_PRINTF("-------------------------------------------------------------------\n");																		
	EMBARC_PRINTF("                       dP        .d888888   888888ba   a88888b.    \n"); 
	EMBARC_PRINTF("                       88       d8'    88   88    `8b d8'   `88    \n"); 
	EMBARC_PRINTF("   .d8888b. 88d8b.d8b. 88d888b. 88aaaaa88a a88aaaa8P' 88           \n"); 
	EMBARC_PRINTF("   88ooood8 88'`88'`88 88'  `88 88     88   88   `8b. 88           \n"); 
	EMBARC_PRINTF("   88.  ... 88  88  88 88.  .88 88     88   88     88 Y8.   .88    \n"); 
	EMBARC_PRINTF("   `88888P' dP  dP  dP 88Y8888' 88     88   dP     dP  Y88888P'    \n"); 
	EMBARC_PRINTF("                                                                   \n"); 
	EMBARC_PRINTF("                           dP.  .dP                                \n"); 
	EMBARC_PRINTF("                            `8bd8'                                 \n"); 
	EMBARC_PRINTF("                            .d88b.                                 \n"); 
	EMBARC_PRINTF("                           dP'  `dP                                \n"); 
	EMBARC_PRINTF("                                                                   \n"); 
	EMBARC_PRINTF(" .88888.   .d888888  .d88888b  dP .d88888b     d8888b. d88  d88888P\n"); 
	EMBARC_PRINTF("d8a   a8b d8a    88  88a    'a 88 88a    ''        a88  88      d8'\n"); 
	EMBARC_PRINTF("88     88 88aaaaa88a 'Y88888b. 88 `Y88888b.     aaad8'  88     d8' \n"); 
	EMBARC_PRINTF("88     88 88     88        `8b 88       `8b        `88  88    d8'  \n"); 
	EMBARC_PRINTF("Y8.   .8P 88     88  d8'   .8P 88 d8'   .8P        .88  88   d8'   \n"); 
	EMBARC_PRINTF(" `8888P'  88     88   Y88888P  dP  Y88888P     d88888P d88P d8'    \n"); 
	EMBARC_PRINTF("                                                                   \n");																			
	EMBARC_PRINTF("-------------------------------------------------------------------\n");
	size_t output_elements = mli_hlp_count_elem_num(defect_net_output, 0);
	float * pred_data = malloc(output_elements * sizeof(float));

	if (pred_data == NULL) {
		printf("ERROR: Can't allocate memory for output\n");
		return -1;
	}
	//load_image(kSingleIn, file);
	for(i=0; i<22; ++i) {
		load_image_de(kSingleIn, IMAGE_LIST[i]);
		image_preprocessing(kSingleIn, defect_net_input);
		printf("\nImage Preprocessing Done!\n");
		defect_net();
		printf("Defect Net Done!\n");
	// Check result
		if (MLI_STATUS_OK == mli_hlp_fx_tensor_to_float(defect_net_output, pred_data, output_elements)) {		
			int result;
			result = measure_acc(pred_data);
			if(result == 0) {
				printf("***********************\n");
				printf("*It is a good product!*\n");
				printf("***********************\n");
				good++;
			}
			else {
				printf("****************************\n");
				printf("*It is a defective product!*\n");
				printf("****************************\n");
				defect++;
			}
		} else {
			printf("ERROR: Can't transform out tensor to float\n");
		}
	}
	printf("Good: %d, Defect: %d\n", good, defect);
	free(pred_data);
	return 0;
}
