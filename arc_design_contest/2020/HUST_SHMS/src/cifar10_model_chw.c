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

#include "mli_api.h"
#include "mli_types.h"
#include "mli_config.h"

#include "cifar10_constants.h"
#include<stdio.h>

#if (MODEL_BIT_DEPTH == MODEL_FX_8)
#define D_EL_TYPE (MLI_EL_FX_8)
#else
#define D_EL_TYPE (MLI_EL_FX_16)
#endif

//==============================================================
//
//
// Data related to the Module
//
//
//==============================================================

// Intermediate data buffers (enough size for max intermediate results)
//==============================
#define IR_BUF_SZ_MOST (67008) //8*8*30*30+28*28*(8+4)
#define IR_BUF_SZ_NEXT (18816)  //37632/2
static d_type  _Z    x_mem_buf[IR_BUF_SZ_MOST-4706];//DCCM 128K
static d_type  _Y    y_mem_buf[IR_BUF_SZ_NEXT];//YCCM 32K
static d_type  _X    z_mem_buf[IR_BUF_SZ_NEXT]; //XCCM 32K


// Module Input/Output tensors and their's external interface
//============================================================
static mli_tensor input = {
	.data = (void *)  y_mem_buf,
	.capacity = sizeof(d_type) * (10*32*32),// input video stored in YCCM
	.shape = {10, 32, 32},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 7,
};

static mli_tensor output = {
	.data = (void *)z_mem_buf,
	.capacity = sizeof(d_type) * OUT_POINTS,// output stored in XCCM
	.shape = {2},
	.rank = 1,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 0,
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
	.perm_dim = {1, 0, 2, 3}
};

static const mli_conv2d_cfg shared_conv_cfg1 = {
	.stride_height = 1, .stride_width = 1,
	.padding_bottom = 0, .padding_top = 0,
	.padding_left = 0, .padding_right = 0,
	.relu.type = MLI_RELU_GEN
};

static const mli_conv2d_cfg shared_conv_cfg2 = {
	.stride_height = 1, .stride_width = 1,
	.padding_bottom = 0, .padding_top = 0,
	.padding_left = 0, .padding_right = 0,
	.relu.type = MLI_RELU_NONE
};

static const mli_pool_cfg shared_pool_cfg = {
	.kernel_height = 2,	.kernel_width = 2,
	.stride_height = 2, .stride_width = 2,
	.padding_bottom = 0, .padding_top = 0,
	.padding_left = 0, .padding_right = 0
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
//输入的小数位情况要具体设定==================================================================================
static mli_tensor input_c1 = {
	.data = (void *)y_mem_buf,
	.capacity = sizeof(d_type) * (3*32*32),// input video stored in YCCM
	.shape = {3, 32, 32},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 7,
};

static mli_tensor r_L1_conv_wt = {
	.data = (void *)L1_conv_wt_buf,
	.capacity = (CONV1_W_ELEMENTS/8) * sizeof(w_type),
	.shape = {1,3,3,3},
	.rank = CONV1_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV1_W_FRAQ,
};

static mli_tensor r_L1_conv_bias = {
	.data = (void *)L1_conv_bias_buf,
	.capacity = (CONV1_B_ELEMENTS/8) * sizeof(w_type),
	.shape = {1},
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

static mli_tensor input_c2 = {
	.data = (void *)x_mem_buf,
	.capacity = sizeof(d_type) * (3*30*30),
	.shape = {3, 30, 30},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV1_OUT_FRAQ,
};

static mli_tensor output_c2 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30),
	.capacity = sizeof(d_type) * (1*28*28),
	.shape = {1, 28, 28},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 6,
};

static mli_tensor r_L2_conv_wt = {
	.data = (void *)L2_conv_wt_buf,
	.capacity = (1*3*3*3) * sizeof(w_type),
	.shape = {1,3,3,3},
	.rank = CONV2_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_W_FRAQ,
};

static mli_tensor r_L2_conv_bias = {
	.data = (void *)L2_conv_bias_buf,
	.capacity = 1 * sizeof(w_type),
	.shape = {1},
	.rank = CONV2_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_B_FRAQ,
};

static mli_tensor addc2_in1 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*8*8*30*30,
	.capacity = (1*28*28) * sizeof(d_type),
	.shape = {1,28,28},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 6,
};

static mli_tensor addc2_in2 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30+1*28*28),
	.capacity = (1*28*28) * sizeof(d_type),
	.shape = {1,28,28},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 6,
};

static mli_tensor addc2_out = {
	.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30+2*28*28),
	.capacity = (1*28*28) * sizeof(d_type),
	.shape = {1,28,28},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 6,
};

static mli_tensor reluc2_in1 = {
	.data = (void *)y_mem_buf,
	.capacity = (4*6*28*28) * sizeof(d_type),
	.shape = {4,6,28,28},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_OUT_FRAQ,
};

static mli_tensor reluc2_in2 = {
	.data = (void *)z_mem_buf,
	.capacity = (4*6*28*28) * sizeof(d_type),
	.shape = {4,6,28,28},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_OUT_FRAQ,
};

static mli_tensor reluc2_out = {
	.data = (void *)x_mem_buf,
	.capacity = (4*6*28*28) * sizeof(d_type),
	.shape = {4,6,28,28},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_OUT_FRAQ,
};

static mli_tensor poolc2_in = {
	.data = (void *)x_mem_buf,
	.capacity = (8*6*28*28) * sizeof(d_type),
	.shape = {8*6,28,28},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_OUT_FRAQ,
};

static const mli_relu_cfg relu_cfg = {
	.type = MLI_RELU_GEN
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

static mli_tensor input_c3 = {
	.data = (void *)y_mem_buf,
	.capacity = sizeof(d_type) * (3*14*14),
	.shape = {3, 14, 14},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV2_OUT_FRAQ,
};

static mli_tensor output_c3 = {
	.data = (void *)x_mem_buf,
	.capacity = sizeof(d_type) * (16*12*12),
	.shape = {16, 12, 12},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 5,
};

static mli_tensor r_L3_conv_wt = {
	.data = (void *)L3_conv_wt_buf,
	.capacity = (16*3*3*3) * sizeof(w_type),
	.shape = {16,3,3,3},
	.rank = CONV3_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV3_W_FRAQ,
};

static mli_tensor r_L3_conv_bias = {
	.data = (void *)L3_conv_bias_buf,
	.capacity = 16 * sizeof(w_type),
	.shape = {16},
	.rank = CONV3_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV3_B_FRAQ,
};

static mli_tensor addc3_in1 = {
	.data = (void *)x_mem_buf,
	.capacity = (4*16*12*12) * sizeof(d_type),
	.shape = {4,16,12,12},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 5,
};

static mli_tensor addc3_in2 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*4*16*12*12,
	.capacity = (4*16*12*12) * sizeof(d_type),
	.shape = {4,16,12,12},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 5,
};

static mli_tensor addc3_out = {
	.data = (void *)x_mem_buf+sizeof(d_type)*8*16*12*12,
	.capacity = (4*16*12*12) * sizeof(d_type),
	.shape = {4,16,12,12},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

static mli_tensor addc3_out1 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*12*16*12*12,
	.capacity = (4*16*12*12) * sizeof(d_type),
	.shape = {4,16,12,12},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

//Conv 4 Layer related tensor
//===================================
static mli_tensor L4_conv_wt = {
	.data = (void *)L4_conv_wt_buf,
	.capacity = CONV4_W_ELEMENTS * sizeof(w_type),
	.shape = CONV4_W_SHAPE,
	.rank = CONV4_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV4_W_FRAQ,
};

static mli_tensor L4_conv_bias = {
	.data = (void *)L4_conv_bias_buf,
	.capacity = CONV4_B_ELEMENTS * sizeof(w_type),
	.shape = CONV4_B_SHAPE,
	.rank = CONV4_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV4_B_FRAQ,
};

static mli_tensor input_c4 = {
	.data = (void *)y_mem_buf,
	.capacity = sizeof(d_type) * (3*12*12),
	.shape = {3, 12, 12},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV3_OUT_FRAQ,
};

static mli_tensor output_c4 = {
	.data = (void *)x_mem_buf,
	.capacity = sizeof(d_type) * (16*10*10),
	.shape = {16, 10, 10},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

static mli_tensor r_L4_conv_wt = {
	.data = (void *)L4_conv_wt_buf,
	.capacity = (16*3*3*3) * sizeof(w_type),
	.shape = {16,3,3,3},
	.rank = CONV4_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV4_W_FRAQ,
};

static mli_tensor r_L4_conv_bias = {
	.data = (void *)L4_conv_bias_buf,
	.capacity = 16 * sizeof(w_type),
	.shape = {16},
	.rank = CONV4_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = CONV4_B_FRAQ,
};

static mli_tensor addc4_in1 = {
	.data = (void *)x_mem_buf,
	.capacity = (2*16*10*10) * sizeof(d_type),
	.shape = {2,16,10,10},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

static mli_tensor addc4_in2 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*2*16*10*10,
	.capacity = (2*16*10*10) * sizeof(d_type),
	.shape = {2,16,10,10},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

static mli_tensor addc4_out = {
	.data = (void *)x_mem_buf+sizeof(d_type)*4*16*10*10,
	.capacity = (2*16*10*10) * sizeof(d_type),
	.shape = {2,16,10,10},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

static mli_tensor addc4_out1 = {
	.data = (void *)x_mem_buf+sizeof(d_type)*6*16*10*10,
	.capacity = (2*16*10*10) * sizeof(d_type),
	.shape = {2,16,10,10},
	.rank = 4,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = 4,
};

static mli_tensor poolc4_in = {
	.data = (void *)y_mem_buf,
	.capacity = (2*16*10*10) * sizeof(d_type),
	.shape = {2*16,10,10},
	.rank = 3,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = CONV4_OUT_FRAQ,
};


// FC5 Layer related data
//===================================
static mli_tensor L5_fc_wt = {
	.data = (void *)L5_fc_wt_buf,
	.capacity = FC5_W_ELEMENTS * sizeof(w_type),
	.shape = FC5_W_SHAPE,
	.rank = FC5_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = FC5_W_FRAQ,
};

static mli_tensor L5_fc_bias = {
	.data = (void *)L5_fc_bias_buf,
	.capacity = FC5_B_ELEMENTS * sizeof(w_type),
	.shape = FC5_B_SHAPE,
	.rank = FC5_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = FC5_B_FRAQ,
};

static mli_tensor reluf5_in = {
	.data = (void *)y_mem_buf,
	.capacity = (32) * sizeof(d_type),
	.shape = {32},
	.rank = 1,
	.el_type = D_EL_TYPE,
	.el_params.fx.frac_bits = FC5_OUT_FRAQ,
};


// FC6 Layer related data
//===================================
static mli_tensor L6_fc_wt = {
	.data = (void *)L6_fc_wt_buf,
	.capacity = FC6_W_ELEMENTS * sizeof(w_type),
	.shape = FC6_W_SHAPE,
	.rank = FC6_W_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = FC6_W_FRAQ,
};

static mli_tensor L6_fc_bias = {
	.data = (void *)L6_fc_bias_buf,
	.capacity = FC6_B_ELEMENTS * sizeof(w_type),
	.shape = FC6_B_SHAPE,
	.rank = FC6_B_RANK,
	.el_type = W_EL_TYPE,
	.el_params.fx.frac_bits = FC6_B_FRAQ,
};


// Intermediate result tensors
//===============================================
static mli_tensor ir_tensor_X = {
	.data = (void *)x_mem_buf,
	.capacity = sizeof(x_mem_buf),
	.shape = {0, 0, 0, 0},          //functional when this tensor used as input container
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

static mli_tensor ir_tensor_Z = {
	.data = (void *)z_mem_buf,
	.capacity = sizeof(z_mem_buf),
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

static inline mli_status eltwise_add(
	const mli_tensor *in1,
	const mli_tensor *in2,
	mli_tensor *out) ;

static inline mli_status relu_3d(
	const mli_tensor *in,
	const mli_relu_cfg *cfg,
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
	//printf("start forward inference\n");
	// LAYER 1
	//=======================================
	ir_tensor_X.el_params.fx.frac_bits = CONV1_OUT_FRAQ;
	int size_c1ir = (1*30*30);
	ir_tensor_X.capacity = sizeof(d_type)*(size_c1ir);

	for (int num_c1w=0;num_c1w<8;num_c1w++)
	{
		//finish one of the output channnel
		r_L1_conv_wt.data = (void *)L1_conv_wt_buf+sizeof(w_type)*(1*3*3*3)*num_c1w;
		r_L1_conv_bias.data = (void *)L1_conv_bias_buf+sizeof(w_type)*(1)*num_c1w;
		for (int num_c1ir=0;num_c1ir<8;num_c1ir++)
		{
			conv2d_chw(&input_c1, &r_L1_conv_wt, &r_L1_conv_bias, &shared_conv_cfg1, &ir_tensor_X);
			input_c1.data = input_c1.data+sizeof(d_type)*(32*32);
			ir_tensor_X.data = ir_tensor_X.data+sizeof(d_type)*size_c1ir;
		}
		input_c1.data=(void *)y_mem_buf;
	}
	ir_tensor_X.data=(void *)x_mem_buf;
	ir_tensor_X.capacity=sizeof(x_mem_buf);     //recover ir_tensor_X
/*
	//==================================
	printf("\n output of C1\n");
	int8_t* test5 = x_mem_buf;
	for (int sum=0;sum<8*8*30*30;sum++)
	{
		if(abs(*(test5+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test5+sum));
		}


	}
	printf("\n");
	//=================================
*/

	// LAYER 2
	//=======================================
	ir_tensor_Y.el_params.fx.frac_bits = CONV2_OUT_FRAQ;
	ir_tensor_Z.el_params.fx.frac_bits = CONV2_OUT_FRAQ;
	int size_c2ir = (1*28*28);
	ir_tensor_Y.capacity = sizeof(d_type)*(size_c2ir);
	ir_tensor_Z.capacity = sizeof(d_type)*(size_c2ir);
	for(int num_c2wo=0; num_c2wo<8; num_c2wo++)
	{
		r_L2_conv_bias.data = (void *)L2_conv_bias_buf+sizeof(w_type)*num_c2wo;
		for(int num_fr=0;num_fr<6;num_fr++)
		{
			for(int num_c2ad=0;num_c2ad<4;num_c2ad++)
			{
				for(int num_c2wi=0; num_c2wi<2; num_c2wi++)
				{
					r_L2_conv_wt.data = (void *)L2_conv_wt_buf+sizeof(w_type)*num_c2wo*(3*3*3)+sizeof(w_type)*(8*3*3*3)*(2*num_c2ad+num_c2wi);
					input_c2.data = (void *)x_mem_buf+sizeof(d_type)*num_fr*30*30+sizeof(d_type)*(8*30*30)*(2*num_c2ad+num_c2wi);
					output_c2.data =(void *)x_mem_buf+sizeof(d_type)*(8*8*30*30)+sizeof(d_type)*num_c2wi*size_c2ir;
					conv2d_chw(&input_c2, &r_L2_conv_wt, &r_L2_conv_bias, &shared_conv_cfg2, &output_c2); //bias need /8

				}
				addc2_out.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30+2*28*28)+sizeof(d_type)*num_c2ad*size_c2ir;
				eltwise_add(&addc2_in1,&addc2_in2,&addc2_out);
			}
/*
			//==================================
			printf("\n output of C2\n");
			int8_t* test7 = x_mem_buf+sizeof(d_type)*(8*8*30*30);
			for (int sum=0;sum<8*28*28;sum++)
			{
				if(abs(*(test7+sum))>=0x40)    //value>2
				{
					printf("%x,",*(test7+sum));
				}


			}
			printf("\n");
			//=================================
*/

			for(int time=0; time<2; time++)
			{
				addc2_in1.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30+2*28*28)+sizeof(d_type)*time*28*28;
				addc2_in2.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30+4*28*28)+sizeof(d_type)*time*28*28;
				addc2_out.data = (void *)x_mem_buf+sizeof(d_type)*8*8*30*30+sizeof(d_type)*time*28*28;
				eltwise_add(&addc2_in1,&addc2_in2,&addc2_out);
			}

/*
			//==================================
			//printf("\n output of C2\n");
			int8_t* test8 = x_mem_buf+sizeof(d_type)*8*8*30*30+sizeof(d_type)*8*28*28;
			for (int sum=0;sum<4*28*28;sum++)
			{
				if(abs(*(test8+sum))>=0x40)    //value>1
				{
					printf("%x,",*(test8+sum));
				}


			}
			printf("\n");
			//=================================
*/

/*
			int8_t* c2rshift1 = x_mem_buf+sizeof(d_type)*(8*8*30*30+8*28*28);
			for(int sum=0;sum<4*28*28;sum++)
			{
				*(c2rshift1+sum)=*(c2rshift1+sum)>>1;
			}
			addc2_in1.el_params.fx.frac_bits=5;
			addc2_in2.el_params.fx.frac_bits=5;
*/

			addc2_in1.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30);
			addc2_in2.data = (void *)x_mem_buf+sizeof(d_type)*(8*8*30*30+1*28*28);
/*
			//==================================
			printf("\n output of C2\n");
			printf(" %d \n",addc2_in1.el_params.fx.frac_bits);
			int8_t* test8 = x_mem_buf+sizeof(d_type)*8*8*30*30;
			for (int sum=0;sum<2*28*28;sum++)
			{
				if(abs(*(test8+sum))>=0x40)    //value>1
				{
					printf("%x,",*(test8+sum));
				}


			}
			printf("\n");
			//=================================
*/


//for right shift
			int8_t* c2rshift = x_mem_buf+sizeof(d_type)*(8*8*30*30);
			for(int sum=0;sum<2*28*28;sum++)
			{
				*(c2rshift+sum)=*(c2rshift+sum)>>1;
			}
			addc2_in1.el_params.fx.frac_bits=5;
			addc2_in2.el_params.fx.frac_bits=5;
			if(num_c2wo<4)
			{
				eltwise_add(&addc2_in1,&addc2_in2,&ir_tensor_Y);
				ir_tensor_Y.data = ir_tensor_Y.data+sizeof(d_type)*1*28*28;
				addc2_in1.el_params.fx.frac_bits=6;
				addc2_in2.el_params.fx.frac_bits=6;
			}
			else
			{
				eltwise_add(&addc2_in1,&addc2_in2,&ir_tensor_Z);
				ir_tensor_Z.data = ir_tensor_Z.data+sizeof(d_type)*1*28*28;
				addc2_in1.el_params.fx.frac_bits=6;
				addc2_in2.el_params.fx.frac_bits=6;
			}
		}
	}
/*
	//==================================
	printf("\n output of C2\n");
	int8_t* test7 = y_mem_buf;
	for (int sum=0;sum<4*6*28*28;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}


	}
	printf("\n");
	//=================================
*/
	//pointer revert
	ir_tensor_Y.data = (void *)y_mem_buf;
	ir_tensor_Z.data = (void *)z_mem_buf;
/*
	//==================================
	printf("\n output of C2\n");
	int8_t* test6 = y_mem_buf;
	for (int sum=0;sum<6*28*28;sum++)
	{
		printf("%x,",*(test6 + sum));
	}
	printf("\n");
	//=================================
*/

	relu_3d(&reluc2_in1,&relu_cfg,&reluc2_out);
	reluc2_out.data = reluc2_out.data+sizeof(d_type)*4*6*28*28;
	relu_3d(&reluc2_in2,&relu_cfg,&reluc2_out);
	reluc2_out.data = (void *)x_mem_buf;
//	printf(" %d,%d,%d,%d \n",reluc2_out.shape[0],reluc2_out.shape[1],reluc2_out.shape[2],reluc2_out.shape[3]);
/*
	//==================================
	printf("\n output of C2\n");
	int8_t* test7 = x_mem_buf;
	for (int sum=0;sum<8*6*28*28;sum++)
	{
		if(*(test7+sum)>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/
	ir_tensor_Y.capacity = sizeof(d_type)*8*6*14*14;
	maxpool_chw(&poolc2_in, &shared_pool_cfg, &ir_tensor_Y);


/*
	//==================================
	printf("\n output of C2\n");
	int8_t* test7 = y_mem_buf;
	for (int sum=0;sum<8*6*14*14;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/

	// LAYER 3  input channel 8   output channel 16
	//=======================================
	for(int time=0;time<4;time++)
	{
		for(int num_c3wi=0;num_c3wi<2;num_c3wi++)
		{
			input_c3.data = (void *)y_mem_buf+(num_c3wi+time*2)*sizeof(d_type)*6*14*14;
			r_L3_conv_wt.data = (void *)L3_conv_wt_buf+(num_c3wi+time*2)*r_L3_conv_wt.capacity;
			for(int num_c3ir=0;num_c3ir<4;num_c3ir++)
			{
				conv2d_chw(&input_c3, &r_L3_conv_wt, &r_L3_conv_bias, &shared_conv_cfg2, &output_c3); //bias need /8
				input_c3.data = input_c3.data+sizeof(d_type)*14*14;
				output_c3.data = output_c3.data+output_c3.capacity;
			}
		}
		output_c3.data = (void *)x_mem_buf;
/*
		//==================================
		printf("\n output of C3\n");
		int8_t* test7 = x_mem_buf;
		for (int sum=0;sum<2*4*16*12*12;sum++)
		{
			if(abs(*(test7+sum))>=0x40)    //value>2
			{
				printf("%x,",*(test7+sum));
			}
		}
		printf("\n");
		//=================================
*/
//right shift
		int8_t* c2rshift1 = x_mem_buf;
		for(int sum=0;sum<8*16*12*12;sum++)
		{
			*(c2rshift1+sum)=*(c2rshift1+sum)>>1;
		}
		addc3_in1.el_params.fx.frac_bits=4;
		addc3_in2.el_params.fx.frac_bits=4;

		eltwise_add(&addc3_in1,&addc3_in2,&addc3_out);
		addc3_out.data = addc3_out.data+addc3_out.capacity;

	}
/*
	//==================================
	printf("\n output of C3\n");
	int8_t* test7 = x_mem_buf+sizeof(d_type)*8*16*12*12;
	for (int sum=0;sum<4*4*16*12*12;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/

	addc3_out.data = (void *)x_mem_buf+sizeof(d_type)*8*16*12*12;
	eltwise_add(&addc3_out,&addc3_out1,&addc3_in1);
	addc3_out.data = addc3_out.data+2*addc3_out.capacity;
	addc3_out1.data = addc3_out1.data+2*addc3_out1.capacity;
	eltwise_add(&addc3_out,&addc3_out1,&addc3_in2);
/*
	//==================================
	printf("\n output of C3\n");
	int8_t* test7 = x_mem_buf;
	for (int sum=0;sum<2*4*16*12*12;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/
	addc3_out.data = (void *)x_mem_buf+sizeof(d_type)*8*16*12*12;
	addc3_out1.data = (void *)x_mem_buf+sizeof(d_type)*12*16*12*12;

//right shift
	int8_t* c2rshift2 = x_mem_buf;
	for(int sum=0;sum<8*16*12*12;sum++)
	{
		*(c2rshift2+sum)=*(c2rshift2+sum)>>1;
	}
	addc3_in1.el_params.fx.frac_bits=3;
	addc3_in2.el_params.fx.frac_bits=3;

	eltwise_add(&addc3_in1,&addc3_in2,&addc3_out);

	relu_3d(&addc3_out,&relu_cfg,&addc3_out1);

	ir_tensor_Y.capacity = sizeof(d_type)*(16*4*12*12);
	ir_tensor_Y.el_params.fx.frac_bits = CONV3_OUT_FRAQ;
	mli_krn_permute_fx(&addc3_out1, &permute_hwc2chw_cfg, &ir_tensor_Y);

	addc3_in1.el_params.fx.frac_bits=5;
	addc3_in2.el_params.fx.frac_bits=5;//recover
/*
	//==================================
	printf("\n output of C3\n");
	int8_t* test7 = y_mem_buf;
	for (int sum=0;sum<4*16*12*12;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/

	// LAYER 4
	//=======================================
	for(int time=0;time<8;time++)
	{
			for(int num_c4wi=0;num_c4wi<2;num_c4wi++)
			{
				input_c4.data = (void *)y_mem_buf+(num_c4wi+time*2)*sizeof(d_type)*4*12*12;
				r_L4_conv_wt.data = (void *)L4_conv_wt_buf+(num_c4wi+time*2)*r_L4_conv_wt.capacity;
				for(int num_c4ir=0;num_c4ir<2;num_c4ir++)
				{
					conv2d_chw(&input_c4, &r_L4_conv_wt, &r_L4_conv_bias, &shared_conv_cfg2, &output_c4); //bias need /16
					input_c4.data = input_c4.data+sizeof(d_type)*12*12;
					output_c4.data = output_c4.data+output_c4.capacity;
				}
			}
			output_c4.data = (void *)x_mem_buf;
/*
			//==================================
			printf("\n output of C4\n");
			int8_t* test7 = x_mem_buf;
			for (int sum=0;sum<2*2*16*10*10;sum++)
			{
				if(abs(*(test7+sum))>=0x40)    //value>2
				{
					printf("%x,",*(test7+sum));
				}
			}
			printf("\n");
			//=================================
*/
			//right shift
			int8_t* c2rshift3 = x_mem_buf;
			for(int sum=0;sum<2*2*16*10*10;sum++)
			{
				*(c2rshift3+sum)=*(c2rshift3+sum)>>1;
			}
			addc4_in1.el_params.fx.frac_bits=3;
			addc4_in2.el_params.fx.frac_bits=3;

			eltwise_add(&addc4_in1,&addc4_in2,&addc4_out);
			addc4_out.data = addc4_out.data+addc4_out.capacity;
	}
/*
	//==================================
	printf("\n output of C3\n");
	int8_t* test7 = x_mem_buf+sizeof(d_type)*4*16*10*10;
	for (int sum=0;sum<8*2*16*10*10;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/
	//right shift
	int8_t* c2rshift4 = x_mem_buf+sizeof(d_type)*2*2*16*10*10;
	for(int sum=0;sum<8*2*16*10*10;sum++)
	{
		*(c2rshift4+sum)=*(c2rshift4+sum)>>1;
	}
	addc4_out.el_params.fx.frac_bits=2;
	addc4_out1.el_params.fx.frac_bits=addc4_out.el_params.fx.frac_bits;

	addc4_out.data = (void *)x_mem_buf+sizeof(d_type)*4*16*10*10;
	eltwise_add(&addc4_out,&addc4_out1,&addc4_in1);
	addc4_out.data = addc4_out.data+2*addc4_out.capacity;
	addc4_out1.data = addc4_out1.data+2*addc4_out1.capacity;
	eltwise_add(&addc4_out,&addc4_out1,&addc4_in2);
	addc4_in1.data = addc4_in1.data+2*addc4_in1.capacity;
	addc4_in2.data = addc4_in2.data+2*addc4_in2.capacity;
	addc4_out.data = addc4_out.data+2*addc4_out.capacity;
	addc4_out1.data = addc4_out1.data+2*addc4_out1.capacity;
	eltwise_add(&addc4_out,&addc4_out1,&addc4_in1);
	addc4_out.data = addc4_out.data+2*addc4_out.capacity;
	addc4_out1.data = addc4_out1.data+2*addc4_out1.capacity;
	eltwise_add(&addc4_out,&addc4_out1,&addc4_in2);

	eltwise_add(&addc4_in1,&addc4_in2,&addc4_out);
	addc4_in1.data = (void *)x_mem_buf;
	addc4_in2.data = (void *)x_mem_buf+sizeof(d_type)*2*16*10*10;
	eltwise_add(&addc4_in1,&addc4_in2,&addc4_out1);

	//right shift
	int8_t* c2rshift5 = x_mem_buf+sizeof(d_type)*8*2*16*10*10;
	for(int sum=0;sum<2*2*16*10*10;sum++)
	{
		*(c2rshift5+sum)=*(c2rshift5+sum)>>1;
	}
	addc4_out.el_params.fx.frac_bits=1;
	addc4_out1.el_params.fx.frac_bits=addc4_out.el_params.fx.frac_bits;

	eltwise_add(&addc4_out,&addc4_out1,&addc4_in1);

	addc4_out.data = (void *)x_mem_buf+sizeof(d_type)*4*16*10*10;
	addc4_out1.data = (void *)x_mem_buf+sizeof(d_type)*6*16*10*10;
	relu_3d(&addc4_in1,&relu_cfg,&addc4_in2);
/*
	//==================================
	printf("\n output of C4\n");
	int8_t* test7 = x_mem_buf+sizeof(d_type)*2*16*10*10;
	for (int sum=0;sum<2*16*10*10;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/
	ir_tensor_Y.capacity = sizeof(d_type)*(16*2*10*10);
	ir_tensor_Z.capacity = sizeof(d_type)*(16*2*5*5);
	ir_tensor_Y.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
	ir_tensor_Z.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
	mli_krn_permute_fx(&addc4_in2, &permute_hwc2chw_cfg, &ir_tensor_Y);
	maxpool_chw(&poolc4_in, &shared_pool_cfg, &ir_tensor_Z);


	// LAYER 5   fc5 and fc6
	//=======================================
	ir_tensor_Y.capacity = sizeof(d_type)*32;
	ir_tensor_Y.el_params.fx.frac_bits = FC5_OUT_FRAQ;
	ir_tensor_Z.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
	fully_connected(&ir_tensor_Z, &L5_fc_wt, &L5_fc_bias, &ir_tensor_Y);
/*
	//==================================
	printf("\n output of FC5\n");
	int8_t* test7 = y_mem_buf;
	for (int sum=0;sum<32;sum++)
	{
		if(abs(*(test7+sum))>=0x40)    //value>2
		{
			printf("%x,",*(test7+sum));
		}
	}
	printf("\n");
	//=================================
*/
	ir_tensor_Z.capacity = sizeof(d_type)*32;
	ir_tensor_Z.el_params.fx.frac_bits = FC5_OUT_FRAQ;
	relu_3d(&reluf5_in,&relu_cfg,&ir_tensor_Z);

	ir_tensor_Y.capacity = sizeof(d_type)*2;
	ir_tensor_Y.el_params.fx.frac_bits = FC6_OUT_FRAQ;

	fully_connected(&ir_tensor_Z, &L6_fc_wt, &L6_fc_bias, &ir_tensor_Y);
	//==================================
	/*printf("\n before softmax\n");
	int8_t* test4 = y_mem_buf;
	for (int sum=0;sum<2;sum++)
	{
		printf("%x  ",*(test4+sum));
	}
	printf("\n");*/
	//=================================

	softmax(&ir_tensor_Y, &output);//OUTPUT.el_params.fx.frac_bits?????????????????????????????????????
	/*printf("%d ",output.el_params.fx.frac_bits);*/
	//==================================
	/*printf("\n after softmax\n");
	int8_t* test = z_mem_buf;
	for (int sum=0;sum<2;sum++)
	{
		printf("%x  ",*(test+sum));
	}
	printf("\n");*/
	//=================================

	ir_tensor_Y.capacity = sizeof(y_mem_buf);
	ir_tensor_Z.capacity = sizeof(z_mem_buf);
	//printf("forward inference done\n");


}

//========================================================================================
//  MLI Functions wrappers: Kernels w/o weights
//========================================================================================
#if (MODEL_BIT_DEPTH != MODEL_FX_8)
static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
	return mli_krn_maxpool_chw_fx16_k2x2(in, cfg, out);
}

static inline mli_status avepool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
	return mli_krn_avepool_chw_fx16_k3x3_krnpad(in, cfg, out);
}

static inline mli_status softmax(const mli_tensor *in,	mli_tensor *out) {
	return mli_krn_softmax_fx16(in, out);
}

static inline mli_status relu(const mli_tensor *in, const mli_relu_cfg *cfg, mli_tensor *out) {
	return mli_krn_relu_fx16(in, cfg, out);
}

static inline mli_status mli_krn_permute_fx(const mli_tensor *in, const mli_permute_cfg *cfg, mli_tensor *out) {
	return mli_krn_permute_fx16(in, cfg, out);
}

#else // MODEL_BIT_DEPTH == (MODEL_FX_8W16D || MODEL_FX_8W16D)
static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
	return mli_krn_maxpool_chw_fx8_k2x2(in, cfg, out);
}

static inline mli_status avepool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
	return mli_krn_avepool_chw_fx8_k3x3_krnpad(in, cfg, out);
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

#endif

//========================================================================================
//  MLI Functions wrappers: Kernels with weights
//========================================================================================
#if (MODEL_BIT_DEPTH == MODEL_FX_8)
static inline mli_status conv2d_chw(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	const mli_conv2d_cfg *cfg,
	mli_tensor *out) {
	return mli_krn_conv2d_chw_fx8_generic(in, weights, bias, cfg, out);
}

static inline mli_status fully_connected(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	mli_tensor *out) {
	return mli_krn_fully_connected_fx8(in, weights, bias, out);
}

static inline mli_status eltwise_add(
	const mli_tensor *in1,
	const mli_tensor *in2,
	mli_tensor *out) {
	return mli_krn_eltwise_add_fx8(in1, in2, out);
}

static inline mli_status relu_3d(
	const mli_tensor *in,
	const mli_relu_cfg *cfg,
	mli_tensor *out) {
	return mli_krn_relu_fx8(in, cfg, out);
}

#elif (MODEL_BIT_DEPTH == MODEL_FX_16)
static inline mli_status conv2d_chw(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	const mli_conv2d_cfg *cfg,
	mli_tensor *out) {
	return mli_krn_conv2d_chw_fx16_generic(in, weights, bias, cfg, out);
}

static inline mli_status fully_connected(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	mli_tensor *out) {
	return mli_krn_fully_connected_fx16(in, weights, bias, out);
}

static inline mli_status eltwise_add(
	const mli_tensor *in1,
	const mli_tensor *in2,
	mli_tensor *out) {
	return mli_krn_eltwise_add_fx16(in1, in2, out);
}

static inline mli_status relu_3d(
	const mli_tensor *in,
	const mli_relu_cfg *cfg,
	mli_tensor *out) {
	return mli_krn_relu_fx16(in, cfg, out);
}

#else // MODEL_BIT_DEPTH == MODEL_FX_8W16D
static inline mli_status conv2d_chw(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	const mli_conv2d_cfg *cfg,
	mli_tensor *out) {
	return mli_krn_conv2d_chw_fx8w16d(in, weights, bias, cfg, out);
}

static inline mli_status fully_connected(
	const mli_tensor *in,
	const mli_tensor *weights,
	const mli_tensor *bias,
	mli_tensor *out) {
	return mli_krn_fully_connected_fx8w16d(in, weights, bias, out);
}
#endif
