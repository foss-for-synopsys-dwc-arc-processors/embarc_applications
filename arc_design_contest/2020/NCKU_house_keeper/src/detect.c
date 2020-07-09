/*
* Copyright 2019, Synopsys, Inc.
* All rights reserved.
*
* This source code is licensed under the BSD-3-Clause license found in
* the LICENSE file in the root directory of this source tree.
*
*/

#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "detect.h"
#include "NCKU_facedetection.h"

#include <math.h>
#include "embARC.h"
#include <stdio.h>
#include "mli_api.h"




void face_detect(uint8_t* frame_buffer)
{

    //int result;
    //uint8_t input_buffer[input_size];
    //uint8_t cursor_x, cursor_y, idx_x, idx_y;
    //uint32_t global_pixel;

    //sliding window
    /*
    for(cursor_y = 0; cursor_y < (frame_h - window_h); cursor_y = cursor_y + stride_h)
    {
    	for(cursor_x = 0; cursor_x < (frame_w - window_w); cursor_x = cursor_x + stride_w)
    	{
    		global_pixel = (frame_w * cursor_y) + cursor_x;
    		printf("Current Position : (%d, %d)\n", cursor_x, cursor_y);
    		for(idx_y = 0; idx_y < 36; idx_y = idx_y + 1)
    		{
    			for(idx_x = 0; idx_x < 36; idx_x = idx_x + 1)
    			{
    				//R
    				input_buffer[(window_num * 0) + (idx_y * window_w) + idx_x] = frame_buffer[((global_pixel + (idx_y * frame_w) + idx_x) * 3) + 0];
    				//G
    				input_buffer[(window_num * 1) + (idx_y * window_w) + idx_x] = frame_buffer[((global_pixel + (idx_y * frame_w) + idx_x) * 3) + 1];
    				//B
    				input_buffer[(window_num * 2) + (idx_y * window_w) + idx_x] = frame_buffer[((global_pixel + (idx_y * frame_w) + idx_x) * 3) + 2];
    			}
    		}

    		result = mli_face_detection(input_buffer);
    		if(result == 1){
    			printf("Found a face.\n");
    		}


    	}
    }
    */

}