/* ------------------------------------------
 * Copyright (c) 2019, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_assert.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif
#include "mbedtls/aes.h"
#include "mbedtls/md.h"
#include "mbedtls/platform_util.h"

#include "crypt_container.h"
#include "keys.h"

#define ENCRYPT_TYPE ENCRYPT_AES_128

CONTAINER_BSS(crypt_container) uint32_t crypt_container_init = 0;

CONTAINER_BSS(crypt_container) uint8_t *key;
CONTAINER_BSS(crypt_container) uint32_t keylen;
CONTAINER_BSS(crypt_container) uint32_t data_size;
CONTAINER_BSS(crypt_container) unsigned char buffer[32];

CONTAINER_BSS(crypt_container) mbedtls_aes_context aes_ctx;

int32_t init_crypt(void)
{
	key = NULL;
	keylen = 0;
	data_size = 0;

	memset(buffer, 0, sizeof(buffer));
	mbedtls_aes_init(&aes_ctx);

	for (uint32_t i = 0; i < keys_num; i++) {
		if (ENCRYPT_TYPE == keys[i].type) {
			key = keys[i].key;
			keylen = keys[i].len;
			data_size = keys[i].data_size;
			break;
		}
	}
	if (key == NULL) {
		EMBARC_PRINTF("encrypt container init failed! \r\n");
		return -1;
	}

	crypt_container_init = 1;

	return 0;
}

int32_t close_crypt(void)
{
	key = NULL;
	keylen = 0;
	data_size = 0;

	memset(buffer, 0, sizeof(buffer));
	mbedtls_aes_free(&aes_ctx);

	crypt_container_init = 0;

	return 0;
}

int32_t operate_encrypt(uint8_t *input, uint32_t input_len,
						uint8_t *output, uint32_t *output_len)
{
	uint32_t offset, n;

	if (crypt_container_init == 0)
		return -1;

	mbedtls_aes_setkey_enc(&aes_ctx, key, keylen);
	*output_len = 0;

	for(offset = 0; offset < input_len; offset += data_size) {
		n = (input_len - offset > data_size) ? data_size : (uint32_t)
			(input_len - offset);
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, input + offset, n);
		mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, buffer, buffer);
		memcpy(output + offset, buffer, data_size);
		*output_len += data_size;
	}

	return 0;
}

int32_t operate_decrypt(uint8_t *input, uint32_t input_len,
						uint8_t *output, uint32_t *output_len)
{
	uint32_t offset;

	if (crypt_container_init == 0)
		return -1;

	mbedtls_aes_setkey_dec(&aes_ctx, key, keylen);
	*output_len = 0;

	for(offset = 0; offset < input_len; offset += data_size) {
		memset(buffer, 0, sizeof(buffer));
		memcpy(buffer, input + offset, data_size);
		mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, buffer, buffer);
		memcpy(output + offset, buffer, data_size);
		*output_len += data_size;
	}

	return 0;
}
