/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#include "embARC.h"
#include "cifar10_model.h"
#include <stdio.h>
#include "mli_api.h"
#include "mli_types.h"
#include "mli_config.h"

#include "cifar10_constants.h"

#define D_EL_TYPE (MLI_EL_FX_8)

//==============================================================
//
//
// Data related to the Module
//
//
//==============================================================

// Intermediate data buffers (enough size for max intermediate results)
//==============================
#define IR_BUF_SZ_MOST (32*144*9)
#define IR_BUF_SZ_NEXT (32*72*9)
static d_type  _Z    x_mem_buf[IR_BUF_SZ_MOST];
static d_type  _Y    y_mem_buf[IR_BUF_SZ_NEXT];

// Module Input/Output tensors and their's external interface
//============================================================
static mli_tensor input = {
	.data = (void *)x_mem_buf,
	.capacity = sizeof(d_type) * IN_POINTS,
	.shape = {144, 9, 1},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 7,//Number of decimal places
};

static mli_tensor output = {
	.data = (void *)y_mem_buf,
	.capacity = sizeof(d_type) * OUT_POINTS,
	.shape = {36},
	.rank = 1,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 7,
};

// Interface variables: Available to user via main model header
//===========================================================
mli_tensor * const cifar10_cf_net_input = &input;
mli_tensor * const cifar10_cf_net_output = &output;

//==============================================================
//  Model description and configuration
//==============================================================


// Configuration objects for layers
//===============================================

static const mli_permute_cfg permute_hwc2chw_cfg = {
	.perm_dim = {2, 0, 1} //hwc to chw
};

static const mli_conv2d_cfg shared_conv_cfg = {
	.stride_height = 1, .stride_width = 1,
	.padding_bottom = 2, .padding_top = 2,
	.padding_left = 2, .padding_right = 2,
	.relu.type = MLI_RELU_GEN
};

static const mli_pool_cfg shared_maxpool_cfg = {
	.kernel_height = 2,	.kernel_width = 3,
	.stride_height = 2, .stride_width = 1,
	.padding_bottom = 0, .padding_top = 0,
	.padding_left = 1, .padding_right = 1
};

static const mli_pool_cfg shared_avgpool_cfg = {
	.kernel_height = 2,	.kernel_width = 3,
	.stride_height = 2, .stride_width = 1,
	.padding_bottom = 0, .padding_top = 0,
	.padding_left = 1, .padding_right = 1
};

// Conv 1 Layer related tensors
//===================================
static const mli_tensor L1_conv_wt = {
	.data = (void *)L1_conv_wt_buf,
	.capacity = CONV1_W_ELEMENTS * sizeof(w_type),
	.shape = CONV1_W_SHAPE,
	.rank = CONV1_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV1_W_FRAQ,
};


static const mli_tensor L1_conv_bias = {
	.data = (void *)L1_conv_bias_buf,
	.capacity = CONV1_B_ELEMENTS * sizeof(w_type),
	.shape = CONV1_B_SHAPE,
	.rank = CONV1_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV1_B_FRAQ,
};


// Conv 2 Layer related data
//===================================
static mli_tensor L2_conv_wt = {
	.data = (void *)L2_conv_wt_buf,
	.capacity = CONV2_W_ELEMENTS * sizeof(w_type),
	.shape = CONV2_W_SHAPE,
	.rank = CONV2_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_W_FRAQ,
};

static mli_tensor L2_conv_bias = {
	.data = (void *)L2_conv_bias_buf,
	.capacity = CONV2_B_ELEMENTS * sizeof(w_type),
	.shape = CONV2_B_SHAPE,
	.rank = CONV2_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_B_FRAQ,
};


// Conv 3 Layer related data
//===================================
static mli_tensor L3_conv_wt = {
	.data = (void *)L3_conv_wt_buf,
	.capacity = CONV3_W_ELEMENTS * sizeof(w_type),
	.shape = CONV3_W_SHAPE,
	.rank = CONV3_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV3_W_FRAQ,
};

static mli_tensor L3_conv_bias = {
	.data = (void *)L3_conv_bias_buf,
	.capacity = CONV3_B_ELEMENTS * sizeof(w_type),
	.shape = CONV3_B_SHAPE,
	.rank = CONV3_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV3_B_FRAQ,
};


// FC4 Layer related data
//===================================
static mli_tensor L4_fc_wt = {
	.data = (void *)L4_fc_wt_buf,
	.capacity = FC4_W_ELEMENTS * sizeof(w_type),
	.shape = FC4_W_SHAPE,
	.rank = FC4_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = FC4_W_FRAQ,
};

static mli_tensor L4_fc_bias = {
	.data = (void *)L4_fc_bias_buf,
	.capacity = FC4_B_ELEMENTS * sizeof(w_type),
	.shape = FC4_B_SHAPE,
	.rank = FC4_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = FC4_B_FRAQ,
};


// Intermediate result tensors
//===============================================
static mli_tensor ir_tensor_X = {
	.data = (void *)x_mem_buf,
	.capacity = sizeof(x_mem_buf),
	.shape = {0, 0, 0, 0},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = FRQ_BITS(0, d_type),
};

static mli_tensor ir_tensor_Y = {
	.data = (void *)y_mem_buf,
	.capacity = sizeof(y_mem_buf),
	.shape = {0, 0, 0, 0},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = FRQ_BITS(0, d_type),
};


//==============================================================
//  Wrappers on MLI calls to deal with various
//  bit depth configurable in compile time
//==============================================================
static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out);

static inline mli_status avepool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out);

static inline mli_status softmax(const mli_tensor *in,	mli_tensor *out);

static inline mli_status relu(const mli_tensor *in, const mli_relu_cfg *cfg, mli_tensor *out);

static inline mli_status mli_krn_permute_fx(const mli_tensor *in, const mli_permute_cfg *cfg, mli_tensor *out);

static inline mli_status conv2d_chw(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	const mli_conv2d_cfg *cfg,
	mli_tensor *out);

static inline mli_status fully_connected(
	const mli_tensor *in,
	const mli_tensor  *weights,
	const mli_tensor  *bias,
	mli_tensor *out);


//==============================================================
//
//  CIFAR10 graph based on Caffe example.
//  Layer-by-Layer execution for CHW layput
//
//==============================================================
void cifar10_cf_net(void) {
	// Version A: Pure implementation: without return status checking and profiling wrappers
	//========================================================================================
	// LAYER 0: Change RGB Image layout
	//=======================================
		 					// printf("cifar10_cf_n test 1\r\n");
	mli_krn_permute_fx(&input, &permute_hwc2chw_cfg, &ir_tensor_Y);
		 					// printf("cifar10_cf_n test 2\r\n");

	// LAYER 1
	//=======================================
	ir_tensor_X.el_params.fx.frac_bits = CONV1_OUT_FRAQ;
		 					// printf("cifar10_cf_n test 23\r\n");

	conv2d_chw(&ir_tensor_Y, &L1_conv_wt, &L1_conv_bias, &shared_conv_cfg, &ir_tensor_X);
	// int8_t *in = ir_tensor_X.data;
	// for(int i=0;i<27;i++){
	// 	printf("input[%d]: %d\n", i, in[i]);
	// }
	// for(int i=((32*144*9)-27);i<(32*144*9);i++){
	// 	printf("input[%d]: %d\n", i, in[i]);
	// }
		 					// printf("cifar10_cf_n test 233\r\n");

	maxpool_chw(&ir_tensor_X, &shared_maxpool_cfg, &ir_tensor_Y);
		 					// printf("cifar10_cf_n test 3\r\n");

	// LAYER 2
	//=======================================
	ir_tensor_X.el_params.fx.frac_bits = CONV2_OUT_FRAQ;
	conv2d_chw(&ir_tensor_Y, &L2_conv_wt, &L2_conv_bias, &shared_conv_cfg, &ir_tensor_X);
	avepool_chw(&ir_tensor_X, &shared_avgpool_cfg, &ir_tensor_Y);
		 					// printf("cifar10_cf_n test 4\r\n");

	// LAYER 3
	//=======================================
	ir_tensor_X.el_params.fx.frac_bits = CONV3_OUT_FRAQ;
	conv2d_chw(&ir_tensor_Y, &L3_conv_wt, &L3_conv_bias, &shared_conv_cfg, &ir_tensor_X);
	avepool_chw(&ir_tensor_X, &shared_avgpool_cfg, &ir_tensor_Y);
		 					// printf("cifar10_cf_n test 5\r\n");

	// LAYER 4
	//=======================================
	ir_tensor_X.el_params.fx.frac_bits = FC4_OUT_FRAQ;
		 					// printf("cifar10_cf_n test 56\r\n");

	fully_connected(&ir_tensor_Y, &L4_fc_wt, &L4_fc_bias, &ir_tensor_X);
		 					// printf("cifar10_cf_n test 6\r\n");

	softmax(&ir_tensor_X, &output);
		 					// printf("cifar10_cf_n test 7\r\n");

}

//========================================================================================
//  MLI Functions wrappers: Kernels w/o weights
//========================================================================================
static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
	return mli_krn_maxpool_chw_fx8_generic(in, cfg, out);
}

static inline mli_status avepool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
	return mli_krn_avepool_chw_fx8_generic(in, cfg, out);
}

static inline mli_status softmax(const mli_tensor *in,	mli_tensor *out) {
	return mli_krn_softmax_fx8(in, out);
}

static inline mli_status relu(const mli_tensor *in, const mli_relu_cfg *cfg, mli_tensor *out) {
	return mli_krn_relu_fx8(in, cfg, out);
}

static inline mli_status mli_krn_permute_fx(const mli_tensor *in, const mli_permute_cfg *cfg, mli_tensor *out) {
	return mli_krn_permute_fx8(in, cfg, out);
}

//========================================================================================
//  MLI Functions wrappers: Kernels with weights
//========================================================================================
static inline mli_status conv2d_chw(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	const mli_conv2d_cfg *cfg,
	mli_tensor *out) {
	return mli_krn_conv2d_chw_fx8_k5x5_str1_krnpad(in, weights, bias, cfg, out);
}

static inline mli_status fully_connected(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	mli_tensor *out) {
	return mli_krn_fully_connected_fx8(in, weights, bias, out);
}
