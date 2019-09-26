/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

//TODO 
//intermediate data buffer 
//print all the layers output ir_tensor y(next, output) x(largest, input)
//print all model layer (kernal , pad, stride, ...) fc
//do the upsample
#include <stdlib.h>
#include <stdio.h>

#include "embARC.h"
#include "seg_model.h"

//#include <stdint.h>
//#include <stdio.h>
//#include <string.h>
//#include <assert.h>

#include "mli_api.h"
#include "mli_types.h"
#include "mli_config.h"

#include "seg_constants.h"
//#include "tests_aux.h"

#if (MODEL_BIT_DEPTH == MODEL_FX_8)
#define D_EL_TYPE (MLI_EL_FX_8)
#define IN_EL_TYPE (MLI_EL_FX_8)
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
#if defined(MODEL_BIG)
#define IR_BUF_SZ_MOST (16*64*64)//(32*32*32) //32768 //1,382,400
#define IR_BUF_SZ_NEXT (32*32*32)//(32*16*16)  //8192  //576,000
#else
#define IR_BUF_SZ_MOST (64*120*160)//(32*32*32) //32768 //1,382,400 need smaller image?
#define IR_BUF_SZ_NEXT (64*120*160)
#endif
static d_type  _Z    x_mem_buf[IR_BUF_SZ_MOST];
static d_type  _Y    y_mem_buf[IR_BUF_SZ_NEXT];
static d_type  _X    z_mem_buf[IR_BUF_SZ_NEXT];
//static in_type  _X    in_mem_buf[64*64*3];

// Module Input/Output tensors and their's external interface
//============================================================
static mli_tensor input = {
    .data = (void *)x_mem_buf,
    .capacity = sizeof(in_type) * IN_POINTS,
    .shape = {64,64, 3},
    .rank = 3,
    .el_type = IN_EL_TYPE,
    .el_params.fx.frac_bits = 7,
};

static mli_tensor output = {
    .data = (void *)z_mem_buf,
    .capacity = sizeof(d_type) * OUT_POINTS,
    .shape = {2,16,16},
    .rank = 3,
    .el_type = D_EL_TYPE,
    .el_params.fx.frac_bits = 4,
};

// Interface variables: Available to user via main model header
//===========================================================
mli_tensor * const seg_net_input = &input;
mli_tensor * const seg_net_output = &output;


//==============================================================
//  Model description and configuration
//==============================================================
//#pragma Data(".mli_data")

// Configuration objects for layers
//===============================================

static const mli_permute_cfg permute_hwc2chw_cfg = {
        .perm_dim = {2, 0, 1}
};

static const mli_conv2d_cfg shared_conv_cfg = {      // stride = 1
    .stride_height = 1, .stride_width = 1,            
    .padding_bottom = 1, .padding_top = 1,           // TODO padding
    .padding_left = 1, .padding_right = 1,
    .relu.type = MLI_RELU_GEN                        //relu
};

static const mli_conv2d_cfg shared_conv_cfg2 = {     // stride = 2
    .stride_height = 2, .stride_width = 2,
    .padding_bottom = 1, .padding_top = 0,           // TODO
    .padding_left = 0, .padding_right = 1,
    .relu.type = MLI_RELU_GEN
};

static const mli_conv2d_cfg shared_conv_cfg3 = {     // for fully convolution layer
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,           // TODO
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE                       // no relu
};

static const mli_pool_cfg shared_pool_cfg = {
    .kernel_height = 3,	.kernel_width = 3,
    .stride_height = 2, .stride_width = 2,
    .padding_bottom = 1, .padding_top = 0,
    .padding_left = 0, .padding_right = 1
};

//static const mli_relu_cfg relu_cfg = {.type = MLI_RELU_GEN};

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

// Conv 4 Layer related data
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

// Conv 5 Layer related data
//===================================
static mli_tensor L5_conv_wt = {
    .data = (void *)L5_conv_wt_buf,
    .capacity = CONV5_W_ELEMENTS * sizeof(w_type),
    .shape = CONV5_W_SHAPE,
    .rank = CONV5_W_RANK,
    .el_type = W_EL_TYPE,
    .el_params.fx.frac_bits = CONV5_W_FRAQ,
};

static mli_tensor L5_conv_bias = {
    .data = (void *)L5_conv_bias_buf,
    .capacity = CONV5_B_ELEMENTS * sizeof(w_type),
    .shape = CONV5_B_SHAPE,
    .rank = CONV5_B_RANK,
    .el_type = W_EL_TYPE,
    .el_params.fx.frac_bits = CONV5_B_FRAQ,
};

// Conv 6 Layer related data
//===================================
static mli_tensor L6_conv_wt = {
    .data = (void *)L6_conv_wt_buf,
    .capacity = CONV6_W_ELEMENTS * sizeof(w_type),
    .shape = CONV6_W_SHAPE,
    .rank = CONV6_W_RANK,
    .el_type = W_EL_TYPE,
    .el_params.fx.frac_bits = CONV6_W_FRAQ,
};

static mli_tensor L6_conv_bias = {
    .data = (void *)L6_conv_bias_buf,
    .capacity = CONV6_B_ELEMENTS * sizeof(w_type),
    .shape = CONV6_B_SHAPE,
    .rank = CONV6_B_RANK,
    .el_type = W_EL_TYPE,
    .el_params.fx.frac_bits = CONV6_B_FRAQ,
};

// FC7 Layer related data
//===================================
static mli_tensor L7_fc_wt = {
    .data = (void *)L7_fc_wt_buf,
    .capacity = FC7_W_ELEMENTS * sizeof(w_type),
    .shape = FC7_W_SHAPE,
    .rank = FC7_W_RANK,
    .el_type = W_EL_TYPE,
    .el_params.fx.frac_bits = FC7_W_FRAQ,
};

static mli_tensor L7_fc_bias = {
    .data = (void *)L7_fc_bias_buf,
    .capacity = FC7_B_ELEMENTS * sizeof(w_type),
    .shape = FC7_B_SHAPE,
    .rank = FC7_B_RANK,
    .el_type = W_EL_TYPE,
    .el_params.fx.frac_bits = FC7_B_FRAQ,
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

static mli_tensor ir_tensor_Z = {
    .data = (void *)z_mem_buf,
    .capacity = sizeof(z_mem_buf),
    .shape = {0, 0, 0, 0},
    .rank = 4,
    .el_type = D_EL_TYPE,
    .el_params.fx.frac_bits = FRQ_BITS(0, d_type),
};
#pragma Data()


//==============================================================
//  Wrappers on MLI calls to deal with various
//  bit depth configurable in compile time
//==============================================================
static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out);

//static inline mli_status avepool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out);

static inline mli_status elwise_add(const mli_tensor *in1, const mli_tensor *in2, mli_tensor *out);

static inline mli_status softmax(const mli_tensor *in,	mli_tensor *out);

static inline mli_status relu(const mli_tensor *in, const mli_relu_cfg *cfg, mli_tensor *out);

static inline mli_status mli_krn_permute_fx(const mli_tensor *in, const mli_permute_cfg *cfg, mli_tensor *out);

static inline mli_status conv2d_chw(       //stride = 1 & 2
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

//  Check kernel result. Debug function
//==============================================================
/*
static void check_result(
        const char * ir_root,
        const char * ref_file,
        mli_tensor *pred_tsr,
        unsigned cycles,
        mli_status ret_code);
*/

//==============================================================
//
//  CIFAR10 graph based on Caffe example.
//  Layer-by-Layer execution for CHW layput
//
//==============================================================
static mli_status mli_hlp_fx_tensor_to_float (const mli_tensor * src, float *dst, uint32_t dst_size) {
    uint32_t elem_num = mli_hlp_count_elem_num(src, 0);
    if (elem_num > dst_size)
        return MLI_STATUS_LENGTH_ERROR;
    if (elem_num == 0)
        return MLI_STATUS_BAD_TENSOR;

    const float scale_val = 1.0f / (float) (1u << (src->el_params.fx.frac_bits));
    // printf("scale: %f\n", scale_val);
    if (src->el_type == MLI_EL_FX_16) {
        int16_t *src_arr = src->data;
        for (int idx = 0; idx < elem_num; idx++)
            dst[idx] = (float) (scale_val * src_arr[idx]);
    } else {
        int8_t *src_arr = src->data;
        for (int idx = 0; idx < elem_num; idx++){
            //printf("d: %d, ",src_arr[idx]);
            dst[idx] = (float) (scale_val * src_arr[idx]);
            //printf("%f, ", dst[idx]);
        }
    }
    // printf("\nFLOAT DONE\n");
    return MLI_STATUS_OK;
}
void seg_net(void) {
    //if (debug_ir_root == NULL) {
        // Version A: Pure implementation: without return status checking and profiling wrappers
        //========================================================================================
        // LAYER 0: Change RGB Image layout
        //=======================================
        #if defined(MODEL_BIG)
        mli_krn_permute_fx(&input, &permute_hwc2chw_cfg, &ir_tensor_Y);  //input use x_mem
        
        // LAYER 1
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV1_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L1_conv_wt, &L1_conv_bias, &shared_conv_cfg, &ir_tensor_X);
        //relu(&ir_tensor_X, &relu_cfg, &ir_tensor_Y)
        /*size_t output_elements = mli_hlp_count_elem_num(&L1_conv_wt, 0);
        float * wt_data = malloc(output_elements * sizeof(float));
        mli_hlp_fx_tensor_to_float(&L1_conv_wt, wt_data, output_elements);
        //printf("%s\n", "in_data: ");
        
        for (int idx = 0; idx < output_elements; idx++){
            //wt_data[idx] = (float)&L1_conv_wt[idx]/(2^CONV1_W_FRAQ);
            printf("%f, ", wt_data[idx]);
        }*/
        // size_t output_elements = mli_hlp_count_elem_num(&ir_tensor_X, 0);
        // printf("elements: %d\n", output_elements);
        // float * out_data = malloc(output_elements * sizeof(float)); 
        // mli_tensor * out = &ir_tensor_X;
        // d_type * dst = (d_type *)out->data;
        //mli_hlp_fx_tensor_to_float(out, out_data, output_elements);

        ir_tensor_Z.el_params.fx.frac_bits = CONV1_OUT_FRAQ;
        maxpool_chw(&ir_tensor_X, &shared_pool_cfg, &ir_tensor_Z);   //define frac_bit??

        // LAYER 2
        //=======================================
        ir_tensor_Y.el_params.fx.frac_bits = CONV2_OUT_FRAQ;
        conv2d_chw(&ir_tensor_X, &L2_conv_wt, &L2_conv_bias, &shared_conv_cfg2, &ir_tensor_Y);

        // LAYER 3
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV3_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L3_conv_wt, &L3_conv_bias, &shared_conv_cfg, &ir_tensor_X);
        ir_tensor_Y.el_params.fx.frac_bits = 4;
        elwise_add(&ir_tensor_X, &ir_tensor_Z, &ir_tensor_Y);


        // LAYER 4
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L4_conv_wt, &L4_conv_bias, &shared_conv_cfg, &ir_tensor_X);
        //relu(&ir_tensor_X, &relu_cfg, &ir_tensor_Y)
        ir_tensor_Z.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
        maxpool_chw(&ir_tensor_X, &shared_pool_cfg, &ir_tensor_Z);   //define frac_bit??

        // LAYER 5
        //=======================================
        ir_tensor_Y.el_params.fx.frac_bits = CONV5_OUT_FRAQ;
        conv2d_chw(&ir_tensor_X, &L5_conv_wt, &L5_conv_bias, &shared_conv_cfg2, &ir_tensor_Y);

        // LAYER 6
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV6_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L6_conv_wt, &L6_conv_bias, &shared_conv_cfg, &ir_tensor_X);
        ir_tensor_Y.el_params.fx.frac_bits = 4;
        elwise_add(&ir_tensor_X, &ir_tensor_Z, &ir_tensor_Y);

        // TODO upsample 

        // LAYER 7
        //=======================================
        //ir_tensor_Z.el_params.fx.frac_bits = FC7_OUT_FRAQ;
        output.el_params.fx.frac_bits = FC7_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L7_fc_wt, &L7_fc_bias, &shared_conv_cfg3, &output);//
        //output.el_params.fx.frac_bits = 4;
        //softmax(&ir_tensor_Z, &output); //output use y_mem


        #else    //small model(without upsample)
        mli_krn_permute_fx(&input, &permute_hwc2chw_cfg, &ir_tensor_Y);  //input use x_mem

        // LAYER 1
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV1_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L1_conv_wt, &L1_conv_bias, &shared_conv_cfg, &ir_tensor_X);

        // LAYER 2
        //=======================================
        ir_tensor_Y.el_params.fx.frac_bits = CONV2_OUT_FRAQ;
        conv2d_chw(&ir_tensor_X, &L2_conv_wt, &L2_conv_bias, &shared_conv_cfg, &ir_tensor_Y);

        // LAYER 3
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV3_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L3_conv_wt, &L3_conv_bias, &shared_conv_cfg, &ir_tensor_Z);
        elwise_add(&ir_tensor_X, &ir_tensor_Z, &ir_tensor_Y);


        // LAYER 4
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L4_conv_wt, &L4_conv_bias, &shared_conv_cfg, &ir_tensor_X);

        // LAYER 5
        //=======================================
        ir_tensor_Y.el_params.fx.frac_bits = CONV5_OUT_FRAQ;
        conv2d_chw(&ir_tensor_X, &L5_conv_wt, &L5_conv_bias, &shared_conv_cfg, &ir_tensor_Y);

        // LAYER 6
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = CONV6_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L6_conv_wt, &L6_conv_bias, &shared_conv_cfg, &ir_tensor_Z);
        elwise_add(&ir_tensor_X, &ir_tensor_Z, &ir_tensor_Y);

        // LAYER 7
        //=======================================
        ir_tensor_X.el_params.fx.frac_bits = FC7_OUT_FRAQ;
        conv2d_chw(&ir_tensor_Y, &L7_fc_wt, &L7_fc_bias, &shared_conv_cfg3, &ir_tensor_X);
        
        softmax(&ir_tensor_X, &output); //output use y_mem
        #endif


    //} 

}

//==============================================================
//  Checking kernel result. Debug function
//==============================================================
/*static void check_result(
        const char * ir_root,
        const char * ref_file,
        mli_tensor *pred_tsr,
        unsigned cycles,
        mli_status ret_code) {
    if (ret_code != MLI_STATUS_OK) {
        printf("ERROR: MLI Code for %s (%d) is not OK\n", ref_file, ret_code);
        assert(0);
    }

    if (ir_root != NULL) {
        ref_to_pred_output err;
        test_status test_result = measure_ref_to_pred(ir_root, ref_file, *pred_tsr, &err);
        if (test_result == TEST_PASSED) {
            printf("%s: \n\tS/N=%-10.1f (%-4.1f db)\n\t%u cycles\n",
                    ref_file,
                    err.ref_vec_length / err.noise_vec_length,
                    err.ref_to_noise_snr,
                    cycles);
        }
        else if (test_result == TEST_FAILED) {
            printf("ERROR: Test suit returns FAILD code for %s\n", ref_file);
            assert(0);
        } else {
            printf("%s(w/o IR check):\t%u cycles\n", ref_file, cycles);
        }
    }
}*/

//========================================================================================
//  MLI Functions wrappers: Kernels w/o weights
//========================================================================================
#if (MODEL_BIT_DEPTH != MODEL_FX_8)
static inline mli_status elwise_add(const mli_tensor *in1, const mli_tensor *in2, mli_tensor *out) {
    return mli_krn_eltwise_add_fx16(in1, in2, out);
}

static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
    return mli_krn_maxpool_chw_fx16_k3x3_krnpad(in, cfg, out);
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
static inline mli_status elwise_add(const mli_tensor *in1, const mli_tensor *in2, mli_tensor *out) {
    return mli_krn_eltwise_add_fx8(in1, in2, out);
}

static inline mli_status maxpool_chw(const mli_tensor *in, const mli_pool_cfg *cfg, mli_tensor *out) {
    return mli_krn_maxpool_chw_fx8_generic(in, cfg, out);
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

#elif (MODEL_BIT_DEPTH == MODEL_FX_16)
static inline mli_status conv2d_chw(
        const mli_tensor *in,
        const mli_tensor *weights,
        const mli_tensor *bias,
        const mli_conv2d_cfg *cfg,
        mli_tensor *out) {
    return mli_krn_conv2d_chw_fx16_k5x5_str1_krnpad(in, weights, bias, cfg, out);
}

static inline mli_status fully_connected(
        const mli_tensor *in,
        const mli_tensor *weights,
        const mli_tensor *bias,
        mli_tensor *out) {
    return mli_krn_fully_connected_fx16(in, weights, bias, out);
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
