/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

//TODO 
// model weights attribute
// what is the size of the output of classifier (19, (128****))
//Tensor's Integer bits per layer definitions

#ifndef _SEG_CONSTANTS_H_
#define _SEG_CONSTANTS_H_

#include "mli_config.h"
//#include "embarc_mli_sections.h"
#include "seg_model.h"

// Defining weight data type
//===================================
#if (MODEL_BIT_DEPTH != MODEL_FX_16)
#define W_EL_TYPE (MLI_EL_FX_8)
typedef int8_t w_type;
#else
#define W_EL_TYPE (MLI_EL_FX_16)
typedef int16_t w_type;
#endif


// Defining data sections attributes
//===================================
#if (ARC_PLATFORM == V2DSP_XY)
#if defined (__GNUC__) && !defined (__CCAC__)
// ARC GNU tools
// Model Weights attribute
#define _Wdata_attr __attribute__((section(".mli_model")))
#define _W  _Wdata_attr

// Model Weights (part 2) attribute 
#define _W2data_attr __attribute__((section(".mli_model_p2")))
#define _W2  _W2data_attr

// Bank X (XCCM) attribute
#define __Xdata_attr __attribute__((section(".Xdata")))
#define _X  __Xdata_attr

// Bank Y (YCCM) attribute
#define __Ydata_attr __attribute__((section(".Ydata")))
#define _Y  __Ydata_attr

// Bank Z (DCCM) attribute
#define __Zdata_attr __attribute__((section(".Zdata")))
#define _Z  __Zdata_attr

#else	
// Metaware tools
// Model Weights attribute
#define _Wdata_attr __attribute__((section(".mli_model")))
#define _W __xy _Wdata_attr

// Model Weights (part 2) attribute 
#define _W2data_attr __attribute__((section(".mli_model_p2")))
#define _W2 __xy _W2data_attr

// Bank X (XCCM) attribute
#define __Xdata_attr __attribute__((section(".Xdata")))
#define _X __xy __Xdata_attr

// Bank Y (YCCM) attribute
#define __Ydata_attr __attribute__((section(".Ydata")))
#define _Y __xy __Ydata_attr

// Bank Z (DCCM) attribute
#define __Zdata_attr __attribute__((section(".Zdata")))
#define _Z __xy __Zdata_attr
#endif // if defined (__GNUC__) && !defined (__CCAC__)

#else
#define _X __attribute__((section(".mli_ir_buf")))
#define _Y __attribute__((section(".mli_ir_buf")))
#define _Z __attribute__((section(".mli_ir_buf")))
#define _W __attribute__((section(".mli_model")))
#define _W2 __attribute__((section(".mli_model")))
#endif
//======================================================
//
// Common data transform (Qmn) defines
//
//======================================================
//1u = unsigned int
#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1) //sign bit / sizeof = byte

//======================================================
//
// Common data transform (Qmn) defines
//
//======================================================

extern const w_type  _W2  L1_conv_wt_buf[];
extern const w_type  _W2  L1_conv_bias_buf[];

extern const w_type  _W2  L2_conv_wt_buf[];
extern const w_type  _W2  L2_conv_bias_buf[];

extern const w_type  _W2  L3_conv_wt_buf[];
extern const w_type  _W2  L3_conv_bias_buf[];

extern const w_type  _W2  L4_conv_wt_buf[];
extern const w_type  _W2  L4_conv_bias_buf[];

extern const w_type  _W2  L5_conv_wt_buf[];
extern const w_type  _W2  L5_conv_bias_buf[];

extern const w_type  _W2  L6_conv_wt_buf[];
extern const w_type  _W2  L6_conv_bias_buf[];

extern const w_type  _W2  L7_fc_wt_buf[];
extern const w_type  _W2  L7_fc_bias_buf[];

//#if defined(MODEL_BIG)
//extern const w_type  _W2  L5_fc_wt_buf[];
//extern const w_type  _W2  L5_fc_bias_buf[];
//#endif

//======================================================
//
// Tensor's Integer bits per layer definitions
//
//======================================================
#if !(MODEL_BIT_DEPTH == MODEL_FX_16)
//using 8 bit
//(MODEL_BIT_DEPTH == MODEL_FX_8 or MODEL_FX_8W16D)

#define CONV1_W_INT   (0)
#define CONV1_B_INT   (0)
#define CONV1_OUT_INT (2)//4

#define CONV2_W_INT   (0) //-1
#define CONV2_B_INT   (0) //-1
#define CONV2_OUT_INT (3)//4

#define CONV3_W_INT   (0) //-1
#define CONV3_B_INT   (0) // -1
#define CONV3_OUT_INT (3)  //5

#define CONV4_W_INT   (0) //
#define CONV4_B_INT   (0)
#define CONV4_OUT_INT (3)//4

#define CONV5_W_INT   (0) //
#define CONV5_B_INT   (0)
#define CONV5_OUT_INT (3)//0

#define CONV6_W_INT   (0) //-2
#define CONV6_B_INT   (0) //-2
#define CONV6_OUT_INT (3) //4

#define FC7_W_INT   (0) //0
#define FC7_B_INT   (0) //-2
#define FC7_OUT_INT (3) //5

#endif

//======================================================
//
// Shape and Fractional bits per layer definitions
//
//======================================================

// CONV1
//================================================
// out, in , h, w
#if defined(MODEL_BIG)
#define CONV1_W_SHAPE {16,3,3,3}
#define CONV1_W_ELEMENTS (16*3*3*3)
#define CONV1_W_RANK (4)
#define CONV1_B_ELEMENTS (16)
#define CONV1_B_SHAPE {16}
#define CONV1_B_RANK (1)
#else
#define CONV1_W_SHAPE {32,3,3,3}
#define CONV1_W_ELEMENTS (32*3*3*3)
#define CONV1_W_RANK (4)
#define CONV1_B_ELEMENTS (32)
#define CONV1_B_SHAPE {32}
#define CONV1_B_RANK (1)
#endif

#define CONV1_W_FRAQ   (FRQ_BITS(CONV1_W_INT, w_type))
#define L1_WQ(val)   QMN(w_type, CONV1_W_FRAQ, val)

#define CONV1_B_FRAQ   (FRQ_BITS(CONV1_B_INT, w_type))
#define L1_BQ(val)   QMN(w_type, CONV1_B_FRAQ, val)

#define CONV1_OUT_FRAQ (FRQ_BITS(CONV1_OUT_INT, d_type))


// CONV2
//================================================
#if defined(MODEL_BIG)
#define CONV2_W_SHAPE {16,16,3,3}
#define CONV2_W_ELEMENTS (16*16*3*3)
#define CONV2_B_SHAPE {16}
#define CONV2_B_ELEMENTS (16)
#else
#define CONV2_W_SHAPE {32,32,3,3}
#define CONV2_W_ELEMENTS (32*32*3*3)
#define CONV2_B_SHAPE {32}
#define CONV2_B_ELEMENTS (32)
#endif

#define CONV2_W_RANK (4)
#define CONV2_B_RANK (1)

#define CONV2_W_FRAQ   (FRQ_BITS(CONV2_W_INT, w_type))
#define L2_WQ(val)   QMN(w_type, CONV2_W_FRAQ, val)
#define CONV2_B_FRAQ   (FRQ_BITS(CONV2_B_INT, w_type))
#define L2_BQ(val)   QMN(w_type, CONV2_B_FRAQ, val)
#define CONV2_OUT_FRAQ (FRQ_BITS(CONV2_OUT_INT, d_type))

// CONV3
//================================================
#if defined(MODEL_BIG)
#define CONV3_W_SHAPE {16,16,3,3}
#define CONV3_W_ELEMENTS (16*16*3*3)
#define CONV3_B_SHAPE {16}
#define CONV3_B_ELEMENTS (16)
#else
#define CONV3_W_SHAPE {32,32,3,3}
#define CONV3_W_ELEMENTS (32*32*3*3)
#define CONV3_B_SHAPE {32}
#define CONV3_B_ELEMENTS (32)
#endif
#define CONV3_W_RANK (4)
#define CONV3_B_RANK (1)

#define CONV3_W_FRAQ   (FRQ_BITS(CONV3_W_INT, w_type))
#define L3_WQ(val)   QMN(w_type, CONV3_W_FRAQ, val)
#define CONV3_B_FRAQ   (FRQ_BITS(CONV3_B_INT, w_type))
#define L3_BQ(val)   QMN(w_type, CONV3_B_FRAQ, val)
#define CONV3_OUT_FRAQ (FRQ_BITS(CONV3_OUT_INT, d_type))

// CONV4
//================================================
#if defined(MODEL_BIG)
#define CONV4_W_SHAPE {32,16,3,3}
#define CONV4_W_ELEMENTS (32*16*3*3)
#define CONV4_B_SHAPE {32}
#define CONV4_B_ELEMENTS (32)
#else
#define CONV4_W_SHAPE {64,32,3,3}
#define CONV4_W_ELEMENTS (64*32*3*3)
#define CONV4_B_SHAPE {64}
#define CONV4_B_ELEMENTS (64)
#endif

#define CONV4_W_RANK (4)
#define CONV4_B_RANK (1)

#define CONV4_W_FRAQ   (FRQ_BITS(CONV4_W_INT, w_type))
#define L4_WQ(val)   QMN(w_type, CONV4_W_FRAQ, val)
#define CONV4_B_FRAQ   (FRQ_BITS(CONV4_B_INT, w_type))
#define L4_BQ(val)   QMN(w_type, CONV4_B_FRAQ, val)
#define CONV4_OUT_FRAQ (FRQ_BITS(CONV4_OUT_INT, d_type))

// CONV5
//================================================
#if defined(MODEL_BIG)
#define CONV5_W_SHAPE {32,32,3,3}
#define CONV5_W_ELEMENTS (32*32*3*3)
#define CONV5_B_SHAPE {32}
#define CONV5_B_ELEMENTS (32)
#else
#define CONV5_W_SHAPE {64,64,3,3}
#define CONV5_W_ELEMENTS (64*64*3*3)
#define CONV5_B_SHAPE {64}
#define CONV5_B_ELEMENTS (64)
#endif

#define CONV5_W_RANK (4)
#define CONV5_B_RANK (1)

#define CONV5_W_FRAQ   (FRQ_BITS(CONV5_W_INT, w_type))
#define L5_WQ(val)   QMN(w_type, CONV5_W_FRAQ, val)
#define CONV5_B_FRAQ   (FRQ_BITS(CONV5_B_INT, w_type))
#define L5_BQ(val)   QMN(w_type, CONV5_B_FRAQ, val)
#define CONV5_OUT_FRAQ (FRQ_BITS(CONV5_OUT_INT, d_type))

// CONV6
//================================================
#if defined(MODEL_BIG)
#define CONV6_W_SHAPE {32,32,3,3}
#define CONV6_W_ELEMENTS (32*32*3*3)
#define CONV6_B_SHAPE {32}
#define CONV6_B_ELEMENTS (32)
#else
#define CONV6_W_SHAPE {64,64,3,3}
#define CONV6_W_ELEMENTS (64*64*3*3)
#define CONV6_B_SHAPE {64}
#define CONV6_B_ELEMENTS (64)
#endif

#define CONV6_W_RANK (4)
#define CONV6_B_RANK (1)

#define CONV6_W_FRAQ   (FRQ_BITS(CONV6_W_INT, w_type))
#define L6_WQ(val)   QMN(w_type, CONV6_W_FRAQ, val)
#define CONV6_B_FRAQ   (FRQ_BITS(CONV6_B_INT, w_type))
#define L6_BQ(val)   QMN(w_type, CONV6_B_FRAQ, val)
#define CONV6_OUT_FRAQ (FRQ_BITS(CONV6_OUT_INT, d_type))

// FC7
//================================================
#if defined(MODEL_BIG)
#define FC7_W_SHAPE {2,32,1,1}
#define FC7_W_ELEMENTS (2*32*1*1)
#define FC7_B_SHAPE {2}
#define FC7_B_ELEMENTS (2)
#else
#define FC7_W_SHAPE {19,64,1,1}
#define FC7_W_ELEMENTS (19*64*1*1)
#define FC7_B_SHAPE {19}
#define FC7_B_ELEMENTS (19)
#endif

#define FC7_W_RANK (4)
#define FC7_B_RANK (1)

#define FC7_W_FRAQ   (FRQ_BITS(FC7_W_INT, w_type))
#define L7_WQ(val)   QMN(w_type, FC7_W_FRAQ, val)
#define FC7_B_FRAQ   (FRQ_BITS(FC7_B_INT, w_type))
#define L7_BQ(val)   QMN(w_type, FC7_B_FRAQ, val)
#define FC7_OUT_FRAQ (FRQ_BITS(FC7_OUT_INT, d_type))

#endif  //_SEG_CONSTANTS_H_
