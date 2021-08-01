/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

//
// Convolutional Neural network model for small pictures classification.
// Based on standart Caffe [1] example for CIFAR-10 [2] dataset
//
// Correspondence of classes:
//  0: 0 
//  1: 1
//  2: 2
//  3: 3
//  4: 4
//  5: 5
//  6: 6
//  7: 7
//  8: 8
//  9: 9
// 10: A
// 11: B
// 12: C
// 13: D
// 14: E
// 15: F
// 16: G
// 17: H
// 18: I
// 19: J
// 20: K
// 21: L
// 22: M
// 23: N
// 24: O
// 25: P
// 26: Q
// 27: R
// 28: S
// 29: T
// 30: U
// 31: V
// 32: W
// 33: X
// 34: Y
// 35: Z


#ifndef _CIFAR10_MODEL_H_
#define _CIFAR10_MODEL_H_

#include <stdint.h>

#include "mli_types.h"

//=============================================
//
// Model interface
//
//=============================================
// Input tensor. To be filled with input image by user befor calling inference function (cifar10_cf_net).
#define IN_POINTS (144 * 9 * 1)
extern mli_tensor * const cifar10_cf_net_input;

// Output tensor for model. Will be filled with probabilities vector by model
#define OUT_POINTS (36)
extern mli_tensor * const cifar10_cf_net_output;

// Model inference function
//
// Get input data from cifar10_cf_net_input tensor (FX format), fed it to the neural network,
// and writes results to cifar10_cf_net_output tensor (FX format). It is user responsibility
// to prepare input tensor correctly before calling this function and get result from output tensor
// after function finish
void cifar10_cf_net(void);


//=============================================
//
// Model bit depth configuration
//
//=============================================
#define MODEL_FX_8       (8)

#define MODEL_BIT_DEPTH (MODEL_FX_8)

typedef int8_t d_type;

#endif  //_CIFAR10_MODEL_H_
