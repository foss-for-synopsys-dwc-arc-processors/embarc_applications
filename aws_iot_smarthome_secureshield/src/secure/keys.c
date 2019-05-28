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

#include "keys.h"

CONTAINER_BSS(crypt_container) uint8_t aes_128_key_1[] = {
	0xEA,0x3A,0xD2,0xC4,0xCD,0xF3,0xBB,0x21,
	0xE9,0x89,0x08,0x7D,0x65,0x19,0xAA,0xA0
};

CONTAINER_BSS(crypt_container) uint8_t aes_128_key_2[] = {
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
	0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x00
};

CONTAINER_BSS(crypt_container) uint8_t aes_128_key_3[] = {
	0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31,
	0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31
};


CONTAINER_BSS(crypt_container) KEY_UNIT keys[] = {
	{
		.key = aes_128_key_1,
		.type = ENCRYPT_AES_128,
		.len = ENCRYPT_AES_128_KEY_LEN,
		.data_size = ENCRYPT_AES_SIZE
	}
};

CONTAINER_BSS(crypt_container) uint32_t keys_num = sizeof(keys)/sizeof(keys[0]);

CONTAINER_BSS(crypt_container) uint32_t exc_nest_count;
CONTAINER_BSS(crypt_container) EXC_HANDLER exc_int_handler_table[] = {0};
