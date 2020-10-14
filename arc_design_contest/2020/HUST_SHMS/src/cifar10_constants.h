/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#ifndef _CIFAR10_CONSTANTS_H_
#define _CIFAR10_CONSTANTS_H_

#include "mli_config.h"
#include "cifar10_model.h"
#include "embarc_mli_sections.h"

// Defining weight data type   W_EL_TYPE = MLI_EL_FX_8; w_type = int8_t
//===================================
#if (MODEL_BIT_DEPTH != MODEL_FX_16)
#define W_EL_TYPE (MLI_EL_FX_8)
typedef int8_t w_type;
#else
#define W_EL_TYPE (MLI_EL_FX_16)
typedef int16_t w_type;
#endif

//======================================================
//
// Common data transform (Qmn) defines
//
//======================================================
//QMN:float point to fix point
#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1)
//L1_WQ(val)   QMN(w_type, CONV1_W_FRAQ, val)
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

extern const w_type  _W2  L5_fc_wt_buf[];
extern const w_type  _W2  L5_fc_bias_buf[];

extern const w_type  _W2  L6_fc_wt_buf[];
extern const w_type  _W2  L6_fc_bias_buf[];


//======================================================
//
// Tensor's Integer bits per layer definitions
//
//======================================================

#if (MODEL_BIT_DEPTH == MODEL_FX_16)

#define CONV1_W_INT   (-2)
#define CONV1_B_INT   (-6)
#define CONV1_OUT_INT (1)

#define CONV2_W_INT   (-3)
#define CONV2_B_INT   (-7)
#define CONV2_OUT_INT (2)

#define CONV3_W_INT   (-3)
#define CONV3_B_INT   (-6)
#define CONV3_OUT_INT (2)

#define FC4_W_INT   (-1)
#define FC4_B_INT   (-2)
#define FC4_OUT_INT (5)

#else //(MODEL_BIT_DEPTH == MODEL_FX_8 or MODEL_FX_8W16D)

#define CONV1_W_INT   (-2)
#define CONV1_B_INT   (-4)
#define CONV1_OUT_INT (1)

#define CONV2_W_INT   (-2)
#define CONV2_B_INT   (-10)
#define CONV2_OUT_INT (2)

#define CONV3_W_INT   (-2)
#define CONV3_B_INT   (-11)
#define CONV3_OUT_INT (4)

#define CONV4_W_INT   (-2)
#define CONV4_B_INT   (-13)
#define CONV4_OUT_INT (6)

#define FC5_W_INT   (-2)
#define FC5_B_INT   (-6)
#define FC5_OUT_INT (5)

#define FC6_W_INT   (-1)
#define FC6_B_INT   (-6)
#define FC6_OUT_INT (3)

#endif




//======================================================
//
// Shape and Fractional bits per layer definitions
//
//======================================================

// CONV1   sum of weight and bias is 224+1736+6928+3472+25632+66=38058 bytes
//================================================
#define CONV1_W_SHAPE {8,3,3,3}     //1st dimension means output channels
#define CONV1_W_ELEMENTS (8*3*3*3)  //216+8=224
#define CONV1_W_RANK (4)

#define CONV1_W_FRAQ   (FRQ_BITS(CONV1_W_INT, w_type))
#define L1_WQ(val)   QMN(w_type, CONV1_W_FRAQ, val)

#define CONV1_B_ELEMENTS (8)
#define CONV1_B_SHAPE {8}
#define CONV1_B_RANK (1)

#define CONV1_B_FRAQ   (FRQ_BITS(CONV1_B_INT, w_type))
#define L1_BQ(val)   QMN(w_type, CONV1_B_FRAQ, val)

#define CONV1_OUT_FRAQ (FRQ_BITS(CONV1_OUT_INT, d_type))

// CONV2
//================================================
//8 out channels,8 input channels;
#define CONV2_W_SHAPE {8,3,3,3}
#define CONV2_W_ELEMENTS (8*3*3*3*8) //1728+8=1736
#define CONV2_B_SHAPE {8}
#define CONV2_B_ELEMENTS (8)

#define CONV2_W_RANK (4)
#define CONV2_B_RANK (1)

#define CONV2_W_FRAQ   (FRQ_BITS(CONV2_W_INT, w_type))
#define L2_WQ(val)   QMN(w_type, CONV2_W_FRAQ, val)
#define CONV2_B_FRAQ   (FRQ_BITS(CONV2_B_INT, w_type))
#define L2_BQ(val)   QMN(w_type, CONV2_B_FRAQ, val)
#define CONV2_OUT_FRAQ (FRQ_BITS(CONV2_OUT_INT, d_type))

// CONV3
//================================================

#define CONV3_W_SHAPE {16,3,3,3}
#define CONV3_W_ELEMENTS (16*3*3*3*8) //3456+16=3472
#define CONV3_B_SHAPE {16}
#define CONV3_B_ELEMENTS (16)

#define CONV3_W_RANK (4)
#define CONV3_B_RANK (1)

#define CONV3_W_FRAQ   (FRQ_BITS(CONV3_W_INT, w_type))
#define L3_WQ(val)   QMN(w_type, CONV3_W_FRAQ, val)
#define CONV3_B_FRAQ   (FRQ_BITS(CONV3_B_INT, w_type))
#define L3_BQ(val)   QMN(w_type, CONV3_B_FRAQ, val)
#define CONV3_OUT_FRAQ (FRQ_BITS(CONV3_OUT_INT, d_type))

// CONV4
//================================================

#define CONV4_W_SHAPE {16,3,3,3}
#define CONV4_W_ELEMENTS (16*3*3*3*16) //6912+16=6928
#define CONV4_B_SHAPE {16}
#define CONV4_B_ELEMENTS (16)

#define CONV4_W_RANK (4)
#define CONV4_B_RANK (1)

#define CONV4_W_FRAQ   (FRQ_BITS(CONV4_W_INT, w_type))
#define L4_WQ(val)   QMN(w_type, CONV4_W_FRAQ, val)
#define CONV4_B_FRAQ   (FRQ_BITS(CONV4_B_INT, w_type))
#define L4_BQ(val)   QMN(w_type, CONV4_B_FRAQ, val)
#define CONV4_OUT_FRAQ (FRQ_BITS(CONV4_OUT_INT, d_type))

// FC5
//================================================
#define FC5_W_SHAPE {32,800}
#define FC5_W_ELEMENTS (32*800)  //25600+32=25632
#define FC5_B_SHAPE {32}
#define FC5_B_ELEMENTS (32)

#define FC5_W_RANK (2)
#define FC5_B_RANK (1)

#define FC5_W_FRAQ   (FRQ_BITS(FC5_W_INT, w_type))
#define L5_WQ(val)   QMN(w_type, FC5_W_FRAQ, val)
#define FC5_B_FRAQ   (FRQ_BITS(FC5_B_INT, w_type))
#define L5_BQ(val)   QMN(w_type, FC5_B_FRAQ, val)
#define FC5_OUT_FRAQ (FRQ_BITS(FC5_OUT_INT, d_type))

// FC6
//================================================
#define FC6_W_SHAPE {2,32}
#define FC6_W_ELEMENTS (2*32)   //64+2=66
#define FC6_W_RANK (2)

#define FC6_W_FRAQ   (FRQ_BITS(FC6_W_INT, w_type))
#define L6_WQ(val)   QMN(w_type, FC6_W_FRAQ, val)

#define FC6_B_ELEMENTS (2)
#define FC6_B_SHAPE {2}
#define FC6_B_RANK (1)

#define FC6_B_FRAQ   (FRQ_BITS(FC6_B_INT, w_type))
#define L6_BQ(val)   QMN(w_type, FC6_B_FRAQ, val)

#define FC6_OUT_FRAQ (FRQ_BITS(FC6_OUT_INT, d_type))


#endif  //_CIFAR10_CONSTANTS_H_
