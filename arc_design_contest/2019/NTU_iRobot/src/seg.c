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
#include <stdio.h>
#include "mli_api.h"

#include "seg_model.h"
#include "seg.h"



//========================================================================================
//
// Other internal functions and routines
//
//========================================================================================

//================================================================================
// Transform MLI FX tensor to float array
//=================================================================================
static uint8_t fromFloat(float x) {
    if (x < 0) return 0;
    if (x > 1e-7) return 255;
    return 255.0e7 * x; // this truncates; add 0.5 to round instead
}

static mli_status mli_hlp_fx_tensor_to_float (const mli_tensor * src, uint8_t *dst, uint32_t dst_size) {
	uint32_t elem_num = mli_hlp_count_elem_num(src, 0);
	if (elem_num > dst_size)
		return MLI_STATUS_LENGTH_ERROR;
	if (elem_num == 0)
		return MLI_STATUS_BAD_TENSOR;

	const float scale_val = 1.0f / (float) (1u << (src->el_params.fx.frac_bits));
	if (src->el_type == MLI_EL_FX_16) {
		int16_t *src_arr = src->data;
		
		for (int idx = 0; idx < elem_num; idx++){
			dst[idx] = (uint8_t)((float) (scale_val * src_arr[idx]));
		}
	} else {
		int8_t *src_arr = src->data;
		for (int idx = 0; idx < elem_num; idx++){
			dst[idx] = (uint8_t)((float) (scale_val * src_arr[idx]));
		}
	}
	
	// printf("\nfloat DONE\n");
	return MLI_STATUS_OK;
}

#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1) //sign bit / sizeof = byte

//========================================================================================
// Image pre-processing for CIFAR-10 net
//========================================================================================
static void cifar10_preprocessing(const void * image_, mli_tensor * net_input_) {
	const unsigned char * in = image_;
	d_type * const dst = (d_type * const)net_input_->data;

	// EMBARC_PRINTF("%s\n", "origin_data: ");
	if (net_input_->el_params.fx.frac_bits == 7) {
		for (int idx = 0; idx < IN_POINTS; idx++){
			dst[idx] = (d_type)(int)in[idx];
			// EMBARC_PRINTF("%d, ", in[idx]);
			dst[idx] = (d_type)((int)in[idx] - 128);
		}
		
	} else if (net_input_->el_params.fx.frac_bits > 7) {
		int shift_left = net_input_->el_params.fx.frac_bits - 7;
		for (int idx = 0; idx < IN_POINTS; idx++)
			dst[idx] = (d_type)((int)in[idx] - 128) << shift_left;
	} else {
		int shift_right = 7 - net_input_->el_params.fx.frac_bits;
		for (int idx = 0; idx < IN_POINTS; idx++)
			dst[idx] = (d_type)((int)in[idx] - 128)  >> shift_right; // w/o rounding
	}
	// EMBARC_PRINTF("\nINPUT DONE\n");
}
 
bool segImage(unsigned char* image, DEV_UART* console_uart) {
	size_t output_elements = mli_hlp_count_elem_num(seg_net_output, 0);
	// EMBARC_PRINTF("NNumber of Output %d\n", output_elements);
	uint8_t* pred_data = malloc(output_elements * sizeof(uint8_t));
	if (pred_data == NULL) {
		EMBARC_PRINTF("ERROR: Can't allocate memory for output\n");
		return false;
	}

	cifar10_preprocessing(image, seg_net_input);

	seg_net();

	// Check result
	if (MLI_STATUS_OK == mli_hlp_fx_tensor_to_float(seg_net_output, pred_data, output_elements)) {
		ref_to_pred_output err;
		// EMBARC_PRINTF("\n%s\n", "pred_data: ");
		for (int idx = 0; idx < 512; idx++){
		    // EMBARC_PRINTF("%d, ", pred_data[idx]);
			console_uart->uart_write((void*)pred_data[idx], 1);
		}
	} else {
		printf("ERROR: Can't transform out tensor to float\n");
		return false;
	}
	free(pred_data);
	return true;
}
