#ifndef _RECOGNIZE_ALG_H_
#define _RECOGNIZE_ALG_H_

/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/
#include <stdlib.h>
#include <math.h>
#include "embARC.h"
#include <stdio.h>
#include "mli_api.h"

#include "model/model.h"

extern mli_tensor * const lenet_input;
extern mli_tensor * const lenet_output;

void model_init(void);
int recognize_algorithm(void* input,uint8_t* result);





#endif