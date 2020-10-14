 /*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#ifndef _CIFAR10_REF_INOUT_H_
#define _CIFAR10_REF_INOUT_H_

#include "cifar10_model.h"

#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1)


#define INQ(val)   ((unsigned char)val)
#define PBQ(val)  (val)
//================================================
// Model input vector #12: Compile-time quantization
//================================================
#define IN_IMG_12_SHAPE {10,32,32}
#define IN_IMG_12_RANK (3)

//==========================================================
// Output classes Probability vector: reference for img #12
//==========================================================

#define OUT_PROB_12_SHAPE {2}
#define OUT_PROB_12_RANK (1)

#define OUT_PROB_12 {\
    PBQ( 0.000122589),PBQ( 0.000146562)\
    }


#endif // _CIFAR10_REF_INOUT_H_
