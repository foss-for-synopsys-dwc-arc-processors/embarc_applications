/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "dev_iic.h"
#include "CH2O.h"
#include "board.h"

#include "embARC.h"
#include "embARC_debug.h"

static DEV_IIC *CH2O;
static uint32_t CH2O_Slvaddr;

int32_t CH2O_regs_read(uint8_t *buff, uint32_t length)
{
	int32_t ercd = E_OK;
	CH2O = iic_get_dev(DW_IIC_1_ID);

	EMSK_CH2O_SENSOR_CHECK_EXP_NORTN(CH2O!=NULL);

	CH2O->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(CH2O_Slvaddr));	
	ercd = CH2O->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = CH2O->iic_read(buff, length);

error_exit:
	return ercd;
}

int32_t CH2O_IIC_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;
	CH2O = iic_get_dev(DW_IIC_1_ID);
	EMSK_CH2O_SENSOR_CHECK_EXP_NORTN(CH2O!=NULL);
	ercd = CH2O->iic_open(DEV_MASTER_MODE, IIC_SPEED_FAST);
	if ((ercd == E_OK) || (ercd == E_OPNED))
	{
		ercd = CH2O->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		CH2O_Slvaddr = slv_addr;	
	}
error_exit:	
	return ercd;
}

uint16_t _CH2O_(void)
{
	int32_t ercd;
	uint8_t i = 0;
    uint16_t d = 0;
	uint8_t CH2O_BUFF1[1] = {0};
	uint8_t CH2O_BUFF2[7] = {0};
	ercd = CH2O_regs_read(CH2O_BUFF1,1);
	while(1)
	{
		ercd = CH2O_regs_read(CH2O_BUFF1,1);
		if(CH2O_BUFF1[0]==255) break;
	}
	while(1)
	{
		ercd = CH2O_regs_read(CH2O_BUFF1,1);
		if((CH2O_BUFF1[0]!=0))
		{
			CH2O_BUFF2[i++] = CH2O_BUFF1[0];		
		}
		else if(i==7)
			break;
	}
	if((CH2O_BUFF2[5]==0x02) && (CH2O_BUFF2[6]==0x05))
	{
		d = CH2O_BUFF2[3]*256 + CH2O_BUFF2[4];
	}
	else 
	{
		d = CH2O_BUFF2[3];
	}
	return d;	
}