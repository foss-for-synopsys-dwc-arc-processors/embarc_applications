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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "process.h"
#include "load_identification_model.h"
#include "input_buffer.h"


void load_identification_preprocessing(const void * data_, mli_tensor * net_input_) {
	const unsigned char * in = data_;
	d_type * const dst = (d_type * const)net_input_->data;

	// Copying data  to input tensor with subtraction of average.
	// Data shift may be required depending on tensor format
	if (net_input_->el_params.fx.frac_bits == 7) {
		for (int idx = 0; idx < IN_POINTS; idx++)
			dst[idx] = (d_type)((int)in[idx] - 128);
	} else if (net_input_->el_params.fx.frac_bits > 7) {
		int shift_left = net_input_->el_params.fx.frac_bits - 7;
		for (int idx = 0; idx < IN_POINTS; idx++)
			dst[idx] = (d_type)((int)in[idx] - 128) << shift_left;
	} else {
		int shift_right = 7 - net_input_->el_params.fx.frac_bits;
		for (int idx = 0; idx < IN_POINTS; idx++)
			dst[idx] = (d_type)((int)in[idx] - 128)  >> shift_right; // w/o rounding
	}
}



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

static RESULT get_result(float *dst,uint32_t dst_size){
	float tmp = dst[0];
	float tmp_1=dst[1],tmp_2=dst[2],tmp_3=dst[3],tmp_4=dst[4],tmp_5=dst[5];
	int max_idx = 0;
	RESULT _result = 0;
	for(int idx = 1; idx < dst_size; idx++){
		if(tmp < dst[idx]){
			tmp = dst[idx];
			max_idx = idx;
		}	
	}
	switch(max_idx)
	{
		case 0:
			_result = 1;
			break;
		case 1:
			_result = 2;
			break;
		case 2:
			_result = 3;
			break;
		case 3:
			_result = 4;
			break;
		case 4:
			_result = 5;
			break;
		case 5:
			_result = 6;
			break;
		case 6:
			//_result = 7;
			break;
		case 7:
			//_result = 8;
			break;
		case 8:
			//_result = 9;
			break;
		case 9:
			//_result = 10;
			break;
		default: _result = 0;
	}
	return _result;
}

RESULT Cnn_Net_Classify(void){
	load_identification_preprocessing(input_buffer, load_identification_net_input);
	load_identification_net();
	size_t output_elements = mli_hlp_count_elem_num(load_identification_net_output, 0);
	float * pred_data = malloc(output_elements * sizeof(float));
	mli_hlp_fx_tensor_to_float(load_identification_net_output, pred_data, output_elements);
	
	return get_result(pred_data,output_elements);
}
































