#include "NCKU_facedetection.h"
#include "NCKU_facedetection_coefficients.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "mli_api.h"
#include "mli_types.h"
#include "mli_config.h"

// Intermediate data buffers (enough size for max intermediate results)
//=============================

#define _Wdata_attr __attribute__((section(".mli_model")))
#define _W __xy _Wdata_attr

// Bank X (XCCM) attribute
#define __Xdata_attr __attribute__((section(".Xdata")))
#define _X __xy __Xdata_attr

// Bank Y (YCCM) attribute
#define __Ydata_attr __attribute__((section(".Ydata")))
#define _Y __xy __Ydata_attr

// Bank Z (DCCM) attribute
#define __Zdata_attr __attribute__((section(".Zdata")))
#define _Z __xy __Zdata_attr

#define IR_BUF_SZ_MOST (32*32*16) //32768
#define IR_BUF_SZ_NEXT (32*32*16) //

static int16_t  _X    x_mem_buf[IR_BUF_SZ_MOST];
static int16_t  _Y    y_mem_buf[IR_BUF_SZ_NEXT];

extern w_type  _W  LR_conv_wt_buf[];
extern w_type  _W  LR_conv_bias_buf[];

extern w_type  _W  L0_conv_wt_buf[];
extern w_type  _W  L0_conv_bias_buf[];

extern w_type  _W  L1_conv_wt_buf[];
extern w_type  _W  L1_conv_bias_buf[];
extern w_type  _W  L1_conv_prelu_buf[];

extern w_type  _W  L2_conv_wt_buf[];
extern w_type  _W  L2_conv_bias_buf[];
extern w_type  _W  L2_conv_prelu_buf[];

extern w_type  _W  L3_conv_wt_buf[];
extern w_type  _W  L3_conv_bias_buf[];
extern w_type  _W  L3_conv_prelu_buf[];

extern w_type  _W  L4_conv_wt_buf[];
extern w_type  _W  L4_conv_bias_buf[];
extern w_type  _W  L4_conv_prelu_buf[];

extern w_type  _W  L5_conv_wt_buf[];
extern w_type  _W  L5_conv_bias_buf[];
extern w_type  _W  L5_conv_prelu_buf[];

extern w_type  _W  L6_conv_wt_buf[];
extern w_type  _W  L6_conv_bias_buf[];
extern w_type  _W  L6_conv_prelu_buf[];

extern w_type  _W  L7_conv_wt_buf[];
extern w_type  _W  L7_conv_bias_buf[];

//------------------
//Layer Function
//------------------
static mli_conv2d_cfg LR_conv_cfg = {
    .stride_height = 3, .stride_width = 3,
    .padding_bottom = 2, .padding_top = 2,
    .padding_left = 2, .padding_right = 2,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L0_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L1_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_pool_cfg L1_pool_cfg = {
    .kernel_height = 2, .kernel_width = 2,
    .stride_height = 2, .stride_width = 2,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0
};

static mli_conv2d_cfg L2_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L3_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L4_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L5_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L6_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

static mli_conv2d_cfg L7_conv_cfg = {
    .stride_height = 1, .stride_width = 1,
    .padding_bottom = 0, .padding_top = 0,
    .padding_left = 0, .padding_right = 0,
    .relu.type = MLI_RELU_NONE
};

//--------------
//I/O tensor
//--------------
static mli_tensor ir_tensor_X = {
    .data = (void *)x_mem_buf,
    .capacity = sizeof(x_mem_buf),
};

static mli_tensor ir_tensor_Y = {
    .data = (void *)y_mem_buf,
    .capacity = sizeof(y_mem_buf),
};

//------------------
//Layer Tensors
//------------------
//RESIZE
//------------------
// RESIZE Layer related tensors
//===================================
static mli_tensor LR_conv_wt = {
    .data = (void *)LR_conv_wt_buf,
    .capacity = RESIZE_W_ELEMENTS * sizeof(w_type),
    .shape = RESIZE_W_SHAPE,
    .rank = RESIZE_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = RESIZE_W_FRAQ,
};

static mli_tensor LR_conv_bias = {
    .data = (void *)LR_conv_bias_buf,
    .capacity = RESIZE_B_ELEMENTS * sizeof(w_type),
    .shape = RESIZE_B_SHAPE,
    .rank = RESIZE_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = RESIZE_B_FRAQ,
};
//CONV0
//------------------
// Conv 0 Layer related tensors
//===================================
static mli_tensor L0_conv_wt = {
    .data = (void *)L0_conv_wt_buf,
    .capacity = CONV0_W_ELEMENTS * sizeof(w_type),
    .shape = CONV0_W_SHAPE,
    .rank = CONV0_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV0_W_FRAQ,
};

static mli_tensor L0_conv_bias = {
    .data = (void *)L0_conv_bias_buf,
    .capacity = CONV0_B_ELEMENTS * sizeof(w_type),
    .shape = CONV0_B_SHAPE,
    .rank = CONV0_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV0_B_FRAQ,
};
//CONV1
//------------------
// Conv 1 Layer related tensors
//===================================
static mli_tensor L1_conv_wt = {
    .data = (void *)L1_conv_wt_buf,
    .capacity = CONV1_W_ELEMENTS * sizeof(w_type),
    .shape = CONV1_W_SHAPE,
    .rank = CONV1_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV1_W_FRAQ,
};

static mli_tensor L1_conv_bias = {
    .data = (void *)L1_conv_bias_buf,
    .capacity = CONV1_B_ELEMENTS * sizeof(w_type),
    .shape = CONV1_B_SHAPE,
    .rank = CONV1_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV1_B_FRAQ,
};

static mli_tensor L1_conv_prelu = {
    .data = (void *)L1_conv_prelu_buf,
    .capacity = CONV1_P_ELEMENTS * sizeof(d_type),
    .shape = CONV1_P_SHAPE,
    .rank = CONV1_P_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV1_P_FRAQ,
};
//CONV2
//------------------
static mli_tensor L2_conv_wt = {
    .data = (void *)L2_conv_wt_buf,
    .capacity = CONV2_W_ELEMENTS * sizeof(w_type),
    .shape = CONV2_W_SHAPE,
    .rank = CONV2_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV2_W_FRAQ,
};

static mli_tensor L2_conv_bias = {
    .data = (void *)L2_conv_bias_buf,
    .capacity = CONV2_B_ELEMENTS * sizeof(w_type),
    .shape = CONV2_B_SHAPE,
    .rank = CONV2_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV2_B_FRAQ,
};

static mli_tensor L2_conv_prelu = {
    .data = (void *)L2_conv_prelu_buf,
    .capacity = CONV2_P_ELEMENTS * sizeof(d_type),
    .shape = CONV2_P_SHAPE,
    .rank = CONV2_P_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV2_P_FRAQ,
};
//CONV3
//------------------
static mli_tensor L3_conv_wt = {
    .data = (void *)L3_conv_wt_buf,
    .capacity = CONV3_W_ELEMENTS * sizeof(w_type),
    .shape = CONV3_W_SHAPE,
    .rank = CONV3_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV3_W_FRAQ,
};

static mli_tensor L3_conv_bias = {
    .data = (void *)L3_conv_bias_buf,
    .capacity = CONV3_B_ELEMENTS * sizeof(w_type),
    .shape = CONV3_B_SHAPE,
    .rank = CONV3_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV3_B_FRAQ,
};

static mli_tensor L3_conv_prelu = {
    .data = (void *)L3_conv_prelu_buf,
    .capacity = CONV3_P_ELEMENTS * sizeof(d_type),
    .shape = CONV3_P_SHAPE,
    .rank = CONV3_P_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV3_P_FRAQ,
};
//CONV4
//------------------
static mli_tensor L4_conv_wt = {
    .data = (void *)L4_conv_wt_buf,
    .capacity = CONV4_W_ELEMENTS * sizeof(w_type),
    .shape = CONV4_W_SHAPE,
    .rank = CONV4_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV4_W_FRAQ,
};

static mli_tensor L4_conv_bias = {
    .data = (void *)L4_conv_bias_buf,
    .capacity = CONV4_B_ELEMENTS * sizeof(w_type),
    .shape = CONV4_B_SHAPE,
    .rank = CONV4_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV4_B_FRAQ,
};

static mli_tensor L4_conv_prelu = {
    .data = (void *)L4_conv_prelu_buf,
    .capacity = CONV4_P_ELEMENTS * sizeof(d_type),
    .shape = CONV4_P_SHAPE,
    .rank = CONV4_P_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV4_P_FRAQ,
};
//CONV5
//------------------
static mli_tensor L5_conv_wt = {
    .data = (void *)L5_conv_wt_buf,
    .capacity = CONV5_W_ELEMENTS * sizeof(w_type),
    .shape = CONV5_W_SHAPE,
    .rank = CONV5_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV5_W_FRAQ,
};

static mli_tensor L5_conv_bias = {
    .data = (void *)L5_conv_bias_buf,
    .capacity = CONV5_B_ELEMENTS * sizeof(w_type),
    .shape = CONV5_B_SHAPE,
    .rank = CONV5_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV5_B_FRAQ,
};

static mli_tensor L5_conv_prelu = {
    .data = (void *)L5_conv_prelu_buf,
    .capacity = CONV5_P_ELEMENTS * sizeof(d_type),
    .shape = CONV5_P_SHAPE,
    .rank = CONV5_P_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV5_P_FRAQ,
};
//CONV6
//------------------
static mli_tensor L6_conv_wt = {
    .data = (void *)L6_conv_wt_buf,
    .capacity = CONV6_W_ELEMENTS * sizeof(w_type),
    .shape = CONV6_W_SHAPE,
    .rank = CONV6_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV6_W_FRAQ,
};

static mli_tensor L6_conv_bias = {
    .data = (void *)L6_conv_bias_buf,
    .capacity = CONV6_B_ELEMENTS * sizeof(w_type),
    .shape = CONV6_B_SHAPE,
    .rank = CONV6_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV6_B_FRAQ,
};

static mli_tensor L6_conv_prelu = {
    .data = (void *)L6_conv_prelu_buf,
    .capacity = CONV6_P_ELEMENTS * sizeof(d_type),
    .shape = CONV6_P_SHAPE,
    .rank = CONV6_P_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV6_P_FRAQ,
};
//CONV7
//------------------
static mli_tensor L7_conv_wt = {
    .data = (void *)L7_conv_wt_buf,
    .capacity = CONV7_W_ELEMENTS * sizeof(w_type),
    .shape = CONV7_W_SHAPE,
    .rank = CONV7_W_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV7_W_FRAQ,
};

static mli_tensor L7_conv_bias = {
    .data = (void *)L7_conv_bias_buf,
    .capacity = CONV7_B_ELEMENTS * sizeof(w_type),
    .shape = CONV7_B_SHAPE,
    .rank = CONV7_B_RANK,
    .el_type = MLI_EL_FX_16,
    .el_params.fx.frac_bits = CONV7_B_FRAQ,
};
//-----------------------------
//Face Detection Model Trigger
//-----------------------------

static void user_custom_preprocessing(const uint8_t *in_image64x64, mli_tensor *output)
{
    const int rows_out = 64;
    const int columns_out = 64;
    int16_t * vec_out = (int16_t *)(output->data);
    int16_t val;

    for(int i = 0; i < (64*64*3); i=i+3) {
        val = in_image64x64[i]*0.299*8 + (in_image64x64[i]+in_image64x64[i+2])*0.587*4 + in_image64x64[i+2]*0.114*8;
        vec_out[i/3] = IQ(val);
    }

    // Fill output tensor
    output->shape[FMAP_C_DIM_CHW] = 3;
    output->shape[FMAP_H_DIM_CHW] = rows_out;
    output->shape[FMAP_W_DIM_CHW] = columns_out;
    output->rank = 3;
    output->el_type = MLI_EL_FX_16;
    output->el_params.fx.frac_bits = 7;
}

int mli_face_detection(uint8_t *input_buffer, const char * debug_ir_root)
{

    //-------------------------PURELY INFERENCE VERSION--------------------------//

    //Put Data into ir_tensor_Y
    //printf("Image Processing.\n");
    user_custom_preprocessing(input_buffer, &ir_tensor_Y);

    //RESIZE
    //printf("Resize. \n");
    ir_tensor_X.el_params.fx.frac_bits = RESIZE_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_Y, &LR_conv_wt, &LR_conv_bias, &LR_conv_cfg, &ir_tensor_X);

    //Layer0
    //--------------------------------------
    //printf("L0. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV0_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L0_conv_wt, &L0_conv_bias, &L0_conv_cfg, &ir_tensor_Y);

    //Layer1
    //--------------------------------------
    //printf("L1. \n");
    ir_tensor_X.el_params.fx.frac_bits = CONV1_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_Y, &L1_conv_wt, &L1_conv_bias, &L1_conv_cfg, &ir_tensor_X);
    ir_tensor_Y.el_params.fx.frac_bits = CONV1_POUT_FRAQ;
    mli_krn_leaky_relu_fx16(&ir_tensor_X, &L1_conv_prelu, &ir_tensor_Y);
    mli_krn_maxpool_chw_fx16(&ir_tensor_Y, &L1_pool_cfg, &ir_tensor_X);

    //Layer2
    //--------------------------------------
    //printf("L2. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV2_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L2_conv_wt, &L2_conv_bias, &L2_conv_cfg, &ir_tensor_Y);
    ir_tensor_X.el_params.fx.frac_bits = CONV2_POUT_FRAQ;
    mli_krn_leaky_relu_fx16(&ir_tensor_Y, &L2_conv_prelu, &ir_tensor_X);

    //Layer3
    //--------------------------------------
    //printf("L3. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV3_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L3_conv_wt, &L3_conv_bias, &L3_conv_cfg, &ir_tensor_Y);
    ir_tensor_X.el_params.fx.frac_bits = CONV3_POUT_FRAQ;
    mli_krn_leaky_relu_fx16(&ir_tensor_Y, &L3_conv_prelu, &ir_tensor_X);

    //Layer4
    //--------------------------------------
    //printf("L4. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV4_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L4_conv_wt, &L4_conv_bias, &L4_conv_cfg, &ir_tensor_Y);
    ir_tensor_X.el_params.fx.frac_bits = CONV4_POUT_FRAQ;
    mli_krn_leaky_relu_fx16(&ir_tensor_Y, &L4_conv_prelu, &ir_tensor_X);

    //Layer5
    //--------------------------------------
    //printf("L5. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV5_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L5_conv_wt, &L5_conv_bias, &L5_conv_cfg, &ir_tensor_Y);
    ir_tensor_X.el_params.fx.frac_bits = CONV5_POUT_FRAQ;
    mli_krn_leaky_relu_fx16(&ir_tensor_Y, &L5_conv_prelu, &ir_tensor_X);

    //Layer6
    //--------------------------------------
    //printf("L6. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV6_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L6_conv_wt, &L6_conv_bias, &L6_conv_cfg, &ir_tensor_Y);
    ir_tensor_X.el_params.fx.frac_bits = CONV6_POUT_FRAQ;
    mli_krn_leaky_relu_fx16(&ir_tensor_Y, &L6_conv_prelu, &ir_tensor_X);

    //Layer7
    //--------------------------------------
    //printf("L7. \n");
    ir_tensor_Y.el_params.fx.frac_bits = CONV7_OUT_FRAQ;
    mli_krn_conv2d_chw_fx16(&ir_tensor_X, &L7_conv_wt, &L7_conv_bias, &L7_conv_cfg, &ir_tensor_Y);

    int16_t pos_score, neg_score;

    pos_score = (((int16_t *)ir_tensor_Y.data)[0]);
    neg_score = (((int16_t *)ir_tensor_Y.data)[1]);

    //printf("Pos = %d\n", pos_score);
    //printf("Neg = %d\n", neg_score);

    return (pos_score > neg_score)? 1: 0;
}
































