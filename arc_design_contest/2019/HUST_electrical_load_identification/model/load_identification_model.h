/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/



#ifndef _LOAD_IDENTIFICATION_MODEL_H_
#define _LOAD_IDENTIFICATION_MODEL_H_

#include <stdint.h>

#include "mli_types.h"

//=============================================
//
// Model interface
//
//=============================================

extern mli_tensor * const load_identification_net_input;
#define IN_POINTS (28 * 28 * 1)
extern mli_tensor * const load_identification_net_output;
#define OUT_POINTS (10)

extern void load_identification_net(void);

//=============================================
//
// Model bit depth configuration
//
//=============================================
#define MODEL_FX_8       (8)
#define MODEL_FX_16      (16)
#define MODEL_FX_8W16D   (816)

#if !defined(MODEL_BIT_DEPTH)
#define MODEL_BIT_DEPTH (MODEL_FX_16)
#endif

#if !defined(MODEL_BIT_DEPTH) || \
    (MODEL_BIT_DEPTH != MODEL_FX_8 && MODEL_BIT_DEPTH != MODEL_FX_16 && MODEL_BIT_DEPTH != MODEL_FX_8W16D)
#error "MODEL_BIT_DEPTH must be defined correctly!"
#endif

#if (MODEL_BIT_DEPTH == MODEL_FX_8)
typedef int8_t d_type;
#else
typedef int16_t d_type;
#endif


#endif // _LOAD_IDENTIFICATION_MODEL_H_
