/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#ifndef _LOAD_IDENTIFICATION_CONSTANTS_H_
#define _LOAD_IDENTIFICATION_CONSTANTS_H_

#include <stdint.h>

#include "mli_api.h"
#include "mli_config.h"
#include "load_identification_model.h"
#include "embarc_mli_sections.h"


// Defining weight data type
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

#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1)

//======================================================
//
// Common data transform (Qmn) defines
//
//======================================================

extern const w_type  _W  L1_conv_wt_buf[];
extern const w_type  _W  L1_conv_bias_buf[];

extern const w_type  _W  L2_conv_wt_buf[];
extern const w_type  _W  L2_conv_bias_buf[];


extern const w_type  _W2  L3_fc_wt_buf[];
extern const w_type  _W2  L3_fc_bias_buf[];


//======================================================
//
// Tensor's Integer bits per layer definitions
//
//======================================================
#if (MODEL_BIT_DEPTH == MODEL_FX_16)

#define CONV1_W_INT   (-1)
#define CONV1_B_INT   (0)
#define CONV1_OUT_INT (4)

#define CONV2_W_INT   (-1)
#define CONV2_B_INT   (-1)
#define CONV2_OUT_INT (5)


#define FC3_W_INT   (-1)
#define FC3_B_INT   (-2)
#define FC3_OUT_INT (5)

#else //(MODEL_BIT_DEPTH == MODEL_FX_8 or MODEL_FX_8W16D)

#define CONV1_W_INT   (-1)
#define CONV1_B_INT   (0)
#define CONV1_OUT_INT (3)

#define CONV2_W_INT   (-1)
#define CONV2_B_INT   (-1)
#define CONV2_OUT_INT (5)


#define FC3_W_INT   (-1)
#define FC3_B_INT   (-2)
#define FC3_OUT_INT (5)

#endif

//======================================================
//
// Shape and Fractional bits per layer definitions
//
//======================================================

// CONV1
//================================================
#define CONV1_W_SHAPE {4,1,5,5}
#define CONV1_W_ELEMENTS (4*1*5*5)
#define CONV1_W_RANK (4)

#define CONV1_W_FRAQ   (FRQ_BITS(CONV1_W_INT, w_type))
#define L1_WQ(val)   QMN(w_type, CONV1_W_FRAQ, val)

#define CONV1_B_ELEMENTS (4)
#define CONV1_B_SHAPE {4}
#define CONV1_B_RANK (1)

#define CONV1_B_FRAQ   (FRQ_BITS(CONV1_B_INT, w_type))
#define L1_BQ(val)   QMN(w_type, CONV1_B_FRAQ, val)

#define CONV1_OUT_FRAQ (FRQ_BITS(CONV1_OUT_INT, d_type))

// CONV2
//================================================
#define CONV2_W_SHAPE {2,4,5,5}
#define CONV2_W_ELEMENTS (2*4*5*5)
#define CONV2_B_SHAPE {2}
#define CONV2_B_ELEMENTS (2)

#define CONV2_W_RANK (4)
#define CONV2_B_RANK (1)

#define CONV2_W_FRAQ   (FRQ_BITS(CONV2_W_INT, w_type))
#define L2_WQ(val)   QMN(w_type, CONV2_W_FRAQ, val)
#define CONV2_B_FRAQ   (FRQ_BITS(CONV2_B_INT, w_type))
#define L2_BQ(val)   QMN(w_type, CONV2_B_FRAQ, val)
#define CONV2_OUT_FRAQ (FRQ_BITS(CONV2_OUT_INT, d_type))


// FC3
//================================================
#define FC3_W_SHAPE {6,(7*7*4)}
#define FC3_W_ELEMENTS (6*(7*7*4))
#define FC3_B_SHAPE {6}
#define FC3_B_ELEMENTS (6)
#define FC3_W_RANK (2)
#define FC3_B_RANK (1)

#define FC3_W_FRAQ   (FRQ_BITS(FC3_W_INT, w_type))
#define L3_WQ(val)   QMN(w_type, FC3_W_FRAQ, val)
#define FC3_B_FRAQ   (FRQ_BITS(FC3_B_INT, w_type))
#define L3_BQ(val)   QMN(w_type, FC3_B_FRAQ, val)
#define FC3_OUT_FRAQ (FRQ_BITS(FC3_OUT_INT, d_type))



#endif // _LOAD_IDENTIFICATION_CONSTANTS_H_
