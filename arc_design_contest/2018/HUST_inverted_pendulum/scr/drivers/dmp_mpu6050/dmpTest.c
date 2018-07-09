/**
 * \file
 * \ingroup	EMBARC_APP_BAREMETAL_BLINKY
 * \brief	main source file for blinky example
 */

/**
 * \addtogroup	EMBARC_APP_BAREMETAL_BLINKY
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include "imu.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dbgUart.h"
#include <math.h>

#define DEFAULT_MPU_HZ 200

/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[9];
};

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static struct platform_data_s gyro_pdata = {
    .orientation = { 1, 0, 0,
                     0, -1, 0,
                     0, 0, -1}
};

static unsigned short inv_row_2_scale(const signed char *row)
{
    unsigned short b;

    if (row[0] > 0)
        b = 0;
    else if (row[0] < 0)
        b = 4;
    else if (row[1] > 0)
        b = 1;
    else if (row[1] < 0)
        b = 5;
    else if (row[2] > 0)
        b = 2;
    else if (row[2] < 0)
        b = 6;
    else
        b = 7;		// error
    return b;
}

/** Converts an orientation matrix made up of 0,+1,and -1 to a scalar representation.
* @param[in] mtx Orientation matrix to convert to a scalar.
* @return Description of orientation matrix. The lowest 2 bits (0 and 1) represent the column the one is on for the
* first row, with the bit number 2 being the sign. The next 2 bits (3 and 4) represent
* the column the one is on for the second row with bit number 5 being the sign.
* The next 2 bits (6 and 7) represent the column the one is on for the third row with
* bit number 8 being the sign. In binary the identity matrix would therefor be:
* 010_001_000 or 0x88 in hex.
*/
unsigned short inv_orientation_matrix_to_scalar(const signed char *mtx)
{

    unsigned short scalar;

    /*
       XYZ  010_001_000 Identity Matrix
       XZY  001_010_000
       YXZ  010_000_001
       YZX  000_010_001
       ZXY  001_000_010
       ZYX  000_001_010
     */

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;


    return scalar;
}

int dmp_init(void)
{
    long gyroZero[3] = {-44, 35, -38};

	mpu6050_enable();

	if(!mpu_init())	
	{	
		if(!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL))    
	        putStr("mpu_set_sensor complete ......\r\n");
	    vTaskDelay(50);
		if(!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL)) 
			putStr("mpu_configure_fifo complete ......\r\n");
	    vTaskDelay(50);
		if(!mpu_set_sample_rate(DEFAULT_MPU_HZ))              
			putStr("mpu_set_sample_rate complete ......\r\n");
	    vTaskDelay(50);
        if(!mpu_set_gyro_bias_reg(gyroZero))              
            putStr("mpu_set_gyro_bias_reg complete ......\r\n");
        vTaskDelay(50);
		if(!dmp_load_motion_driver_firmware())                
			putStr("dmp_load_motion_driver_firmware complete ......\r\n");
	    vTaskDelay(50);
		if(!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_pdata.orientation)))
			putStr("dmp_set_orientation complete ......\r\n"); 
	    vTaskDelay(50);
		if(!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_SEND_RAW_ACCEL |
								DMP_FEATURE_SEND_RAW_GYRO))
							   //DMP_FEATURE_SEND_CAL_GYRO | DMP_FEATURE_GYRO_CAL))
			putStr("dmp_enable_feature complete ......\r\n");
	    vTaskDelay(50);
		if(!dmp_set_fifo_rate(DEFAULT_MPU_HZ))    
			putStr("dmp_set_fifo_rate complete ......\r\n");
	    vTaskDelay(50);
		if(!mpu_set_dmp_state(1))                 
			putStr("mpu_set_dmp_state complete ......\r\n");
	    vTaskDelay(50);
	}
}

uint8_t read_dmp(float* Pitch,float* Roll,float* Yaw, float *gout)
{   
	short gyro[3], accel[3], sensors;
	float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;
	unsigned long sensor_timestamp;
	unsigned char more;
	long quat[4];
	float q30 = (float)(1 << 30);
    if(dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors, &more)) return 1;

    gout[0] = gyro[0] / 937.0f;
    gout[1] = gyro[1] / 937.0f;
    gout[2] = gyro[2] / 937.0f;

    if (sensors & INV_WXYZ_QUAT)
    {    
         q0=quat[0] / q30;
         q1=quat[1] / q30;
         q2=quat[2] / q30;
         q3=quat[3] / q30;
         *Pitch = (float)asin(-2 * q1 * q3 + 2 * q0* q2)*57.3f;     
         //*Roll = (float)atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1);
         *Yaw = (float)atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3)*57.3f;
         return 0;
    }   
    else 
        return 2;
}

const int topTskPrio = 4;
const int topTskStkSize = 128;

/**
 * \brief	Test hardware board without any peripheral
 */
int topTask(void)
{
	uint64_t start_us, end_us;
	float pitch, roll, yaw, gout[3];
	int i = 0, rtn = 0;

	dbgInit();

	dmp_init();

    //vTaskDelay(8000);

    while(1){
    	start_us = board_get_cur_us();
        rtn = read_dmp(&pitch, &roll, &yaw, gout);
        end_us = board_get_cur_us() - start_us;
        if(i > 1){
			putStr("rtn:%d\r\n", rtn);
        	putStr("X:%.3f, Y:%.3f, Z:%.3f\r\n", 
                	gout[0], gout[1], gout[2]);
        	putStr("P:%.3f, R:%.3f, Y:%.3f, T:%d\r\n", 
                 	pitch, roll, yaw, end_us);
        	i = 0;
        }	
        else i++;
	    vTaskDelay(5);
    }

	return E_SYS;
}

int topInit(void)
{
	BaseType_t rtn;

    // Create tasks
    rtn=xTaskCreate(topTask, "topTask", topTskStkSize, NULL, topTskPrio, NULL);
    configASSERT(rtn==pdPASS);
}

int dmpTest(void)
{
	topInit();
    vTaskStartScheduler();
	return 0;
}

/** @} */