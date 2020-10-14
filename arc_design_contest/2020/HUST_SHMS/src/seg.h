/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/
#ifndef SEG_H
#define SEG_H
#include <stdlib.h>
#include <math.h>
#include "embARC.h"
#include <stdio.h>
#include "mli_api.h"
#include "embARC_debug.h"
#include "embARC_error.h"


#include "cifar10_model.h"

#ifndef MAX
/** @def Common Maximum macro function (two values)*/
#define MAX(A,B) (((A) > (B))? (A): (B))
#endif

int process(uint8_t* images);


#endif
