#ifndef	_NCKU_FD_COEFFICIENTS_H_
#define _NCKU_FD_COEFFICIENTS_H_

#include "mli_config.h"

#include "NCKU_facedetection.h"

//Define Fix-Point Calculate Function
#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1)

typedef int16_t w_type;
typedef int16_t d_type;

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

//-----------------------------------------------
//Define Integer Bits per Layer
//-----------------------------------------------
#define IMAGE_INT		(8)

#define RESIZE_W_INT	(-3)
#define RESIZE_B_INT	(0)
#define RESIZE_OUT_INT	(8)

#define CONV0_W_INT		(-7)
#define CONV0_B_INT		(0)
#define CONV0_OUT_INT	(0)

#define CONV1_W_INT   	(-1)
#define CONV1_B_INT   	(-2)
#define CONV1_P_INT	  	(0)
#define CONV1_OUT_INT 	(0)
#define CONV1_POUT_INT 	(-1)

#define CONV2_W_INT   	(0)
#define CONV2_B_INT   	(-3)
#define CONV2_P_INT	  	(-1)
#define CONV2_OUT_INT 	(1)
#define CONV2_POUT_INT 	(0)

#define CONV3_W_INT   	(-1)
#define CONV3_B_INT   	(-2)
#define CONV3_P_INT	  	(-2)
#define CONV3_OUT_INT 	(2)
#define CONV3_POUT_INT 	(2)

#define CONV4_W_INT   	(0)
#define CONV4_B_INT   	(-3)
#define CONV4_P_INT	  	(-1)
#define CONV4_OUT_INT 	(3)
#define CONV4_POUT_INT 	(2)

#define CONV5_W_INT   	(-1)
#define CONV5_B_INT   	(-3)
#define CONV5_P_INT	  	(-1)
#define CONV5_OUT_INT 	(4)
#define CONV5_POUT_INT 	(4)

#define CONV6_W_INT   	(-1)
#define CONV6_B_INT   	(-3)
#define CONV6_P_INT	  	(0)
#define CONV6_OUT_INT 	(4)
#define CONV6_POUT_INT 	(4)

#define CONV7_W_INT   	(0)
#define CONV7_B_INT   	(-4)
#define CONV7_OUT_INT 	(6)

//(Co, Ci, h, w)
//-----------------------------------------------
//Shape and Fractional bits per layer definitions
//-----------------------------------------------
//Input_image
//-----------------------------------------------
#define IMAGE_FRAQ		(FRQ_BITS(IMAGE_INT, d_type))
#define IQ(val)		QMN(d_type, IMAGE_FRAQ, val)
//Resize
//-----------------------------------------------
#define RESIZE_W_SHAPE {1,1,3,3}
#define RESIZE_W_ELEMENTS (1*1*3*3)
#define RESIZE_W_RANK (4)

#define RESIZE_W_FRAQ   (FRQ_BITS(RESIZE_W_INT, w_type))
#define LR_WQ(val)   QMN(w_type, RESIZE_W_FRAQ, val)

#define RESIZE_B_ELEMENTS (1)
#define RESIZE_B_SHAPE {1}
#define RESIZE_B_RANK (1)

#define RESIZE_B_FRAQ   (FRQ_BITS(RESIZE_B_INT, w_type))
#define LR_BQ(val)   QMN(w_type, RESIZE_B_FRAQ, val)

#define RESIZE_OUT_FRAQ (FRQ_BITS(RESIZE_OUT_INT, d_type))
//CONV0
//-----------------------------------------------
#define CONV0_W_SHAPE {1,1,1,1}
#define CONV0_W_ELEMENTS (1*1*1*1)
#define CONV0_W_RANK (4)

#define CONV0_W_FRAQ   (FRQ_BITS(CONV0_W_INT, w_type))
#define L0_WQ(val)   QMN(w_type, CONV0_W_FRAQ, val)

#define CONV0_B_ELEMENTS (1)
#define CONV0_B_SHAPE {1}
#define CONV0_B_RANK (1)

#define CONV0_B_FRAQ   (FRQ_BITS(CONV0_B_INT, w_type))
#define L0_BQ(val)   QMN(w_type, CONV0_B_FRAQ, val)

#define CONV0_OUT_FRAQ (FRQ_BITS(CONV0_OUT_INT, d_type))
//CONV1
//-----------------------------------------------
#define CONV1_W_SHAPE {24,1,3,3}
#define CONV1_W_ELEMENTS (24*1*3*3)
#define CONV1_W_RANK (4)

#define CONV1_W_FRAQ   (FRQ_BITS(CONV1_W_INT, w_type))
#define L1_WQ(val)   QMN(w_type, CONV1_W_FRAQ, val)

#define CONV1_B_ELEMENTS (24)
#define CONV1_B_SHAPE {24}
#define CONV1_B_RANK (1)

#define CONV1_B_FRAQ   (FRQ_BITS(CONV1_B_INT, w_type))
#define L1_BQ(val)   QMN(w_type, CONV1_B_FRAQ, val)

#define CONV1_OUT_FRAQ (FRQ_BITS(CONV1_OUT_INT, d_type))

#define CONV1_P_ELEMENTS (24)
#define CONV1_P_SHAPE {24}
#define CONV1_P_RANK (1)

#define CONV1_P_FRAQ   (FRQ_BITS(CONV1_P_INT, w_type))
#define L1_PQ(val)   QMN(w_type, CONV1_P_FRAQ, val)

#define CONV1_POUT_FRAQ (FRQ_BITS(CONV1_POUT_INT, d_type))
//CONV2
//-----------------------------------------------
#define CONV2_W_SHAPE {24,24,3,3}
#define CONV2_W_ELEMENTS (24*24*3*3)
#define CONV2_W_RANK (4)

#define CONV2_W_FRAQ   (FRQ_BITS(CONV2_W_INT, w_type))
#define L2_WQ(val)   QMN(w_type, CONV2_W_FRAQ, val)

#define CONV2_B_ELEMENTS (24)
#define CONV2_B_SHAPE {24}
#define CONV2_B_RANK (1)

#define CONV2_B_FRAQ   (FRQ_BITS(CONV2_B_INT, w_type))
#define L2_BQ(val)   QMN(w_type, CONV2_B_FRAQ, val)

#define CONV2_OUT_FRAQ (FRQ_BITS(CONV2_OUT_INT, d_type))

#define CONV2_P_ELEMENTS (24)
#define CONV2_P_SHAPE {24}
#define CONV2_P_RANK (1)

#define CONV2_P_FRAQ   (FRQ_BITS(CONV2_P_INT, w_type))
#define L2_PQ(val)   QMN(w_type, CONV2_P_FRAQ, val)

#define CONV2_POUT_FRAQ (FRQ_BITS(CONV2_POUT_INT, d_type))

//CONV3
//-----------------------------------------------
#define CONV3_W_SHAPE {32,24,3,3}
#define CONV3_W_ELEMENTS (32*24*3*3)
#define CONV3_W_RANK (4)

#define CONV3_W_FRAQ   (FRQ_BITS(CONV3_W_INT, w_type))
#define L3_WQ(val)   QMN(w_type, CONV3_W_FRAQ, val)

#define CONV3_B_ELEMENTS (32)
#define CONV3_B_SHAPE {32}
#define CONV3_B_RANK (1)

#define CONV3_B_FRAQ   (FRQ_BITS(CONV3_B_INT, w_type))
#define L3_BQ(val)   QMN(w_type, CONV3_B_FRAQ, val)

#define CONV3_OUT_FRAQ (FRQ_BITS(CONV3_OUT_INT, d_type))

#define CONV3_P_ELEMENTS (32)
#define CONV3_P_SHAPE {32}
#define CONV3_P_RANK (1)

#define CONV3_P_FRAQ   (FRQ_BITS(CONV3_P_INT, w_type))
#define L3_PQ(val)   QMN(w_type, CONV3_P_FRAQ, val)

#define CONV3_POUT_FRAQ (FRQ_BITS(CONV3_POUT_INT, d_type))

//CONV4
//-----------------------------------------------
#define CONV4_W_SHAPE {48,32,3,3}
#define CONV4_W_ELEMENTS (48*32*3*3)
#define CONV4_W_RANK (4)

#define CONV4_W_FRAQ   (FRQ_BITS(CONV4_W_INT, w_type))
#define L4_WQ(val)   QMN(w_type, CONV4_W_FRAQ, val)

#define CONV4_B_ELEMENTS (48)
#define CONV4_B_SHAPE {48}
#define CONV4_B_RANK (1)

#define CONV4_B_FRAQ   (FRQ_BITS(CONV4_B_INT, w_type))
#define L4_BQ(val)   QMN(w_type, CONV4_B_FRAQ, val)

#define CONV4_OUT_FRAQ (FRQ_BITS(CONV4_OUT_INT, d_type))

#define CONV4_P_ELEMENTS (48)
#define CONV4_P_SHAPE {48}
#define CONV4_P_RANK (1)

#define CONV4_P_FRAQ   (FRQ_BITS(CONV4_P_INT, w_type))
#define L4_PQ(val)   QMN(w_type, CONV4_P_FRAQ, val)

#define CONV4_POUT_FRAQ (FRQ_BITS(CONV4_POUT_INT, d_type))

//CONV5
//-----------------------------------------------
#define CONV5_W_SHAPE {32,48,3,3}
#define CONV5_W_ELEMENTS (32*48*3*3)
#define CONV5_W_RANK (4)

#define CONV5_W_FRAQ   (FRQ_BITS(CONV5_W_INT, w_type))
#define L5_WQ(val)   QMN(w_type, CONV5_W_FRAQ, val)

#define CONV5_B_ELEMENTS (32)
#define CONV5_B_SHAPE {32}
#define CONV5_B_RANK (1)

#define CONV5_B_FRAQ   (FRQ_BITS(CONV5_B_INT, w_type))
#define L5_BQ(val)   QMN(w_type, CONV5_B_FRAQ, val)

#define CONV5_OUT_FRAQ (FRQ_BITS(CONV5_OUT_INT, d_type))

#define CONV5_P_ELEMENTS (32)
#define CONV5_P_SHAPE {32}
#define CONV5_P_RANK (1)

#define CONV5_P_FRAQ   (FRQ_BITS(CONV5_P_INT, w_type))
#define L5_PQ(val)   QMN(w_type, CONV5_P_FRAQ, val)

#define CONV5_POUT_FRAQ (FRQ_BITS(CONV5_POUT_INT, d_type))

//CONV6
//-----------------------------------------------
#define CONV6_W_SHAPE {16,32,3,3}
#define CONV6_W_ELEMENTS (16*32*3*3)
#define CONV6_W_RANK (4)

#define CONV6_W_FRAQ   (FRQ_BITS(CONV6_W_INT, w_type))
#define L6_WQ(val)   QMN(w_type, CONV6_W_FRAQ, val)

#define CONV6_B_ELEMENTS (16)
#define CONV6_B_SHAPE {16}
#define CONV6_B_RANK (1)

#define CONV6_B_FRAQ   (FRQ_BITS(CONV6_B_INT, w_type))
#define L6_BQ(val)   QMN(w_type, CONV6_B_FRAQ, val)

#define CONV6_OUT_FRAQ (FRQ_BITS(CONV6_OUT_INT, d_type))

#define CONV6_P_ELEMENTS (16)
#define CONV6_P_SHAPE {16}
#define CONV6_P_RANK (1)

#define CONV6_P_FRAQ   (FRQ_BITS(CONV6_P_INT, w_type))
#define L6_PQ(val)   QMN(w_type, CONV6_P_FRAQ, val)

#define CONV6_POUT_FRAQ (FRQ_BITS(CONV6_POUT_INT, d_type))

//CONV7
//-----------------------------------------------
#define CONV7_W_SHAPE {2,16,1,1}
#define CONV7_W_ELEMENTS (2*16*1*1)
#define CONV7_W_RANK (4)

#define CONV7_W_FRAQ   (FRQ_BITS(CONV7_W_INT, w_type))
#define L7_WQ(val)   QMN(w_type, CONV7_W_FRAQ, val)

#define CONV7_B_ELEMENTS (2)
#define CONV7_B_SHAPE {2}
#define CONV7_B_RANK (1)

#define CONV7_B_FRAQ   (FRQ_BITS(CONV7_B_INT, w_type))
#define L7_BQ(val)   QMN(w_type, CONV7_B_FRAQ, val)

#define CONV7_OUT_FRAQ (FRQ_BITS(CONV7_OUT_INT, d_type))

#endif