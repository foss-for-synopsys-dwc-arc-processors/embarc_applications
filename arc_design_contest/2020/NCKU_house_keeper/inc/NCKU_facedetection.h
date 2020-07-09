#ifndef	_NCKU_FD_H_
#define	_NCKU_FD_H_

#include "stdint.h"

#define IN_POINTS (12*12*3)
#define OUT_POINTS (2)

// Face Trigger Input dimmensions size (same for X and Y)
#define FT_MODEL_IN_DIM_SZ (12)

// Face Trigger Input size
#define FT_MODEL_IN_POINTS (3 * FT_MODEL_IN_DIM_SZ * FT_MODEL_IN_DIM_SZ)

//int mli_face_detection(const char * debug_ir_root);

#endif