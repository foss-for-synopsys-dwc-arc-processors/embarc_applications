// embARC
#include "embARC.h"
// Standard
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// Mli
#include "mli_api.h"
#include "cifar10_ref_inout.h"
#include "cifar10_model.h"
// Mpu9250
#include "mpu9250.h"
// DEVICE-iotdk
#include "board.h"
#include "dev_iic.h"
#include "dev_gpio.h"
#include "dev_uart.h"
#include "dfss_iic_obj.h"
#include "dfss_gpio_obj.h"
#include "dfss_uart_obj.h"
// OLED
#include "oled.h"
// 24L01-WiFi
#include "24l01.h"

//=========================================================================
//								MPU9250 and button
//==========================================================================
#define MPU9250_IIC_ID DFSS_IIC_0_ID
static MPU9250_DEFINE(mpu9250_sensor, MPU9250_IIC_ID, MPU9250_IIC_ADDRESS);
DEV_UART *dev_uart0 = NULL;


#define IOT_BUTTON_OFFSET	(2)
#define IOT_BUTTON_MASK	    ((0x1) << IOT_BUTTON_OFFSET)
#define IOT_BUTTON3_OFFSET	(3)
#define IOT_BUTTON3_MASK	    ((0x1) << IOT_BUTTON3_OFFSET)
#define IOT_BUTTON_PORT	    (DFSS_GPIO_4B1_ID)
#define IOT_BUTTON_DIR		((0x0) << IOT_BUTTON_OFFSET)
#define IOT_BUTTON3_DIR		((0x0) << IOT_BUTTON3_OFFSET)

static DEV_GPIO *button_port;

/* Button init */
void iot_button_init(void)
{
	button_port = gpio_get_dev(IOT_BUTTON_PORT);

	if (button_port->gpio_open(IOT_BUTTON_DIR) == E_OPNED) {
		button_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(IOT_BUTTON_MASK));
		button_port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(IOT_BUTTON_MASK));
	}
	if (button_port->gpio_open(IOT_BUTTON3_DIR) == E_OPNED) {
		button_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(IOT_BUTTON3_MASK));
		button_port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(IOT_BUTTON3_MASK));
	}

	error_exit:
	return;
}

/* Press down bit set to 1 else 0 */
uint32_t iot_button_read(uint32_t num)
{
	uint32_t value;
	button_port->gpio_read(&value, (0x01 << num));// &IOT_BUTTON_MASK
	return (value >> num);

	error_exit:
	return 0;
}


//========================================================================================
//								Other internal functions and routines
//========================================================================================

float max(float x, float y)
{
    return x>y ? x:y;
}

float min(float x, float y)
{
    return x<y ? x:y;
}

int8_t in_Q(float val)
{
	return (uint8_t)(val * (1u << 7) + ((val >= 0)? 0.5f: -0.5f));
}

#define QMN(type, fraq, val)   (type)(val * (1u << (fraq)) + ((val >= 0)? 0.5f: -0.5f))
#define FRQ_BITS(int_part, el_type) ((sizeof(el_type)*8)-int_part-1)

#define IN_FRAQ   (FRQ_BITS(IN_INT, w_type))
#define INQ(val)   QMN(w_type, IN_FRAQ, val)


//================================================================================
//								Transform MLI FX tensor to float array
//=================================================================================
static mli_status mli_hlp_fx_tensor_to_float (const mli_tensor * src, float *dst, uint32_t dst_size)
{
	uint32_t elem_num = mli_hlp_count_elem_num(src, 0);
	if (elem_num > dst_size)
		return MLI_STATUS_LENGTH_ERROR;
	if (elem_num == 0)
		return MLI_STATUS_BAD_TENSOR;

	const float scale_val = 1.0f / (float) (1u << (src->el_params.fx.frac_bits));
	// printf("type:%d\n",src->el_type);
	// printf("scale_val:%f\n",scale_val);
	if (src->el_type == MLI_EL_FX_8) 
	{
		int8_t *src_arr = src->data;
		for (int idx = 0; idx < elem_num; idx++)
		{
			// printf("before[%d]: %d\n", idx, src_arr[idx]);
			dst[idx] = (float) (scale_val * src_arr[idx]);
			// printf("after[%d]: %f\n", idx, dst[idx]);
		}
	}
	return MLI_STATUS_OK;
}

//========================================================================================
//							Image pre-processing for CIFAR-10 net
//========================================================================================
static void cifar10_preprocessing(const void * image_, mli_tensor * net_input_) 
{
	const unsigned char * in = image_;
	d_type * const dst = (d_type * const)net_input_->data;

	for (int idx = 0; idx < IN_POINTS; idx++)
		dst[idx] = in[idx];
}

char net_process(void * kSingleIn) 
{
	size_t output_elements = mli_hlp_count_elem_num(cifar10_cf_net_output, 0);
	float * pred_data = malloc(output_elements * sizeof(float));
	uint8_t max_data=0;
	float   max_pre=0;

	uint8_t data_array[output_elements];
	float   odds_array[output_elements];
	uint8_t temp_data;
	float   temp_odds;

	if (pred_data == NULL) 
	{
		printf("ERROR: Can't allocate memory for output\n");
		return -1;
	}

	cifar10_preprocessing(kSingleIn, cifar10_cf_net_input);

	cifar10_cf_net();

	if (MLI_STATUS_OK == mli_hlp_fx_tensor_to_float(cifar10_cf_net_output, pred_data, output_elements)) 
	{
		for(int i=0;i<output_elements;i++)
		{
			printf("pred_data[%d]: %f\n", i, pred_data[i]);
			if (max_pre < pred_data[i])
			{
				if (i<=9)
					max_data = i + '0';
				else
					max_data = i - 10 + 'A';
				max_pre = pred_data[i];
			}
			odds_array[i] = pred_data[i];
			if (i <= 9)
			{
				data_array[i] = i + '0';
			}
			else
			{
				data_array[i] = i - 10 + 'A';
			}				
		}
				printf("----------------------------------------\r\n");
		
		for (int i = 0; i < output_elements; i++)
		{
			for (int j = i; j < output_elements; j++)
			{
				if (odds_array[i] > odds_array[j])
				{
					temp_data = data_array[i];
					data_array[i] = data_array[j];
					data_array[j] = temp_data;

					temp_odds = odds_array[i];
					odds_array[i] = odds_array[j];
					odds_array[j] = temp_odds;
				}
			}
			if (odds_array[i] > 0.000001f)
				printf("WRITE [%c] : %f\r\n",data_array[i],odds_array[i]);
		}
				printf("----------------------------------------\r\n");
	} 
	else 
	{
		printf("ERROR: Can't transform out tensor to float\n");
	}

	free(pred_data);
	printf("|write  %c   |     %f  |\r\n",max_data,max_pre);
					printf("----------------------------------------\r\n");
	return max_data;
}

//Defined as global variables in the data, the variables in the function are in the stack area
MPU9250_DATA mpu9250_data = { 0 };  //read imu data

//the imu max and min data
float roll_max=0, roll_min=0, pitch_max=0, pitch_min=0, yaw_max=0, yaw_min=0;
float accx_max=0, accx_min=0, accy_max=0, accy_min=0, accz_max=0, accz_min=0;
float gyrx_max=0, gyrx_min=0, gyry_max=0, gyry_min=0, gyrz_max=0, gyrz_min=0;

int16_t part,num_part;		//Interval, number to be deleted or inserted
int16_t cut;				//Group index to delete or insert
int16_t num_append;			//Useful array subscripts
int8_t	IMU_In  [144][9] = { 0 };  //net input data
int16_t imu_data[800][9] = { 0 };  //imu data buffer
float 	imu_temp[144][9] = { 0.0 }; //temp imu data
int	 	num_imu;    //The amount of data read each time
int16_t count;    //Number of read times

//========================================================================================
//							Linear interpolation
//========================================================================================
void bi_linear(int16_t (*src)[9], float (*dst)[9], int input_size)
{
	float corr_x,corr_y;
	float point1[2],point2[2],point3[2],point4[2];
	float fr1,fr2;
	for(int i=0; i<144; i++)
	{
		for(int j=0; j<9; j++)
		{
			//First find the corresponding point (X, Y) coordinates in the original image
			corr_x = ((float)i+0.5)/144*(float)input_size-0.5;
			// printf("ccorx:%f\r\n",corr_x);
			corr_y = ((float)j+0.5)/9*9-0.5;
			// printf("corr_y:%f\r\n",corr_y);
			//if i*pic.shape[0]%th==0 and j*pic.shape[1]%tw==0:     
			//emptyImage[i, j, k] = pic[int(corr_x), int(corr_y), k]
			point1[0] = floor(corr_x);
			point1[1] = floor(corr_y);   // In the upper left corner
			point2[0] = point1[0];
			point2[1] = point1[1]+1;
			point3[0] = point1[0]+1;
			point3[1] = point1[1];
			point4[0] = point1[0]+1;
			point4[1] = point1[1]+1;

			fr1 = (point2[1]-corr_y)*(float)src[(int)point1[0]][(int)point1[1]] + (corr_y-point1[1])*(float)src[(int)point2[0]][(int)point2[1]];
			fr2 = (point2[1]-corr_y)*(float)src[(int)point3[0]][(int)point3[1]] + (corr_y-point1[1])*(float)src[(int)point4[0]][(int)point4[1]];
			dst[i][j] = (point3[0]-corr_x)*fr1 + (corr_x-point1[0])*fr2;
			// printf("src[%d][%d]:%f\r\n",i,j,src[i][j]);
			// printf("dst[%d][%d]:%f ",i,j,dst[i][j]);
		}
		// printf("\r\n");
	}
}

int main(void)
{
	/* Boadr initialization */ 
    cpu_lock();
	board_init(); 
	cpu_unlock();
    		EMBARC_PRINTF(" BOARD INITIAL  OK!\n\r");
	/* Device initialization */ 
    NRF24L01_Init();        				//NRF2401
	NRF24L01_TX_Mode();     				//TX-mode

    uint8_t	tmp_buf[33];
	uint8_t write_data;
	uint8_t write_data_str[1];
	
	

	printf("%d\r\n",INQ(0.99));
    dev_uart0 = uart_get_dev(DFSS_UART_0_ID);
    if(dev_uart0->uart_open(UART_BAUDRATE_115200) == E_OPNED){
        dev_uart0->uart_control(UART_CMD_SET_BAUD, (void *)(UART_BAUDRATE_115200));
    }

	/* Device initialization */
	iot_button_init();						//Button
	mpu9250_sensor_init(mpu9250_sensor);	//Mpu9250
	OLED_Configuration();   				//Oled

	count = 0;
	uint8_t time = 1;
	//inference
	while(1){

		// Botton isn't pressed
		while (!iot_button_read(2))
		{

		}
		// OLED_Clear();
		
		// Test number
		printf("test_num:%d\r\nresult is :\r\n",count++);

		// Mark the max and min value of each group
		num_imu = -1;
		roll_max=0.;
		roll_min=0.;
		pitch_max=0.;
		pitch_min=0.;
		yaw_max=0.;
		yaw_min=0.;
        accx_max=0.;
		accx_min=0.;
		accy_max=0.;
		accy_min=0.;
		accz_max=0.;
		accz_min=0.;
        gyrx_max=0.;
		gyrx_min=0.;
		gyry_max=0.;
		gyry_min=0.;
		gyrz_max=0.;
		gyrz_min=0.;

		// Pessing the botton
		while (iot_button_read(2)) 
		{
			// OLED_ShowString(1,1, "You are writting...", 8);  
			// Read mpu9250 data
			board_delay_ms(1, 0);
			mpu9250_sensor_read(mpu9250_sensor, &mpu9250_data);
			num_imu++;

			// printf("%d %d %d %d %d %d %d %d %d \r\n", (int16_t)(mpu9250_data.roll*100),(int16_t)(mpu9250_data.pitch*100),(int16_t)(mpu9250_data.yaw*100),mpu9250_data.accel_x,mpu9250_data.accel_y,mpu9250_data.accel_z,mpu9250_data.gyro_x,mpu9250_data.gyro_y,mpu9250_data.gyro_z);  

			// Store mpu9250 data
			if(num_imu>=800)
			{
				printf("The memory have filled!\r\n");
				break;
			}
			else
			{
				imu_data[num_imu][0] = (int16_t)(mpu9250_data.roll*100);
				imu_data[num_imu][1] = (int16_t)(mpu9250_data.pitch*100);
				imu_data[num_imu][2] = (int16_t)(mpu9250_data.yaw*100);
				imu_data[num_imu][3] = mpu9250_data.accel_x;
				imu_data[num_imu][4] = mpu9250_data.accel_y;
				imu_data[num_imu][5] = mpu9250_data.accel_z;
				imu_data[num_imu][6] = mpu9250_data.gyro_x;
				imu_data[num_imu][7] = mpu9250_data.gyro_y;
				imu_data[num_imu][8] = mpu9250_data.gyro_z;
			}

		}

		OLED_Clear();

		//Bilinear resize
		bi_linear(imu_data, imu_temp, num_imu);
		// Store max and min mpu9250 data
		for(int i=0; i<144; i++){
			roll_max = max(roll_max,imu_temp[i][0]);
			pitch_max = max(pitch_max,imu_temp[i][1]);
			yaw_max = max(yaw_max,imu_temp[i][2]);
			accx_max = max(accx_max,imu_temp[i][3]);
			accy_max = max(accy_max,imu_temp[i][4]);
			accz_max = max(accz_max,imu_temp[i][5]);
			gyrx_max = max(gyrx_max,imu_temp[i][6]);
			gyry_max = max(gyry_max,imu_temp[i][7]);
			gyrz_max = max(gyrz_max,imu_temp[i][8]);
			roll_min = min(roll_min,imu_temp[i][0]);
			pitch_min = min(pitch_min,imu_temp[i][1]);
			yaw_min = min(yaw_min,imu_temp[i][2]);
			accx_min = min(accx_min,imu_temp[i][3]);
			accy_min = min(accy_min,imu_temp[i][4]);
			accz_min = min(accz_min,imu_temp[i][5]);
			gyrx_min = min(gyrx_min,imu_temp[i][6]);
			gyry_min = min(gyry_min,imu_temp[i][7]);
			gyrz_min = min(gyrz_min,imu_temp[i][8]);
		}
		// Data process 144*9,normalization[0,0.99]
		if(((roll_max == 0) && (roll_min == 0)) || ((pitch_max == 0) && (pitch_min == 0)) || ((yaw_max == 0) && (yaw_min == 0)) ||
			((accx_max == 0) && (accx_min == 0)) || ((accy_max == 0) && (accy_min == 0)) || ((accz_max == 0) && (accz_min == 0)) ||
			((gyrx_max == 0) && (gyrx_min == 0)) || ((gyry_max == 0) && (gyry_min == 0)) || ((gyrz_max == 0) && (gyrz_min == 0)))
		{
			// Divisor cannot be 0
			printf("max and min is zero!");
		}
		else
		{
			for(int i=0; i<144; i++)
			{
				IMU_In[i][0] = INQ(((imu_temp[i][0]-(float)roll_min)/(float)(roll_max-roll_min))*0.99);
				IMU_In[i][1] = INQ(((imu_temp[i][1]-(float)pitch_min)/(float)(pitch_max-pitch_min))*0.99);
				IMU_In[i][2] = INQ(((imu_temp[i][2]-(float)yaw_min)/(float)(yaw_max-yaw_min))*0.99);
				IMU_In[i][3] = INQ(((imu_temp[i][3]-(float)accx_min)/(float)(accx_max-accx_min))*0.99);
				IMU_In[i][4] = INQ(((imu_temp[i][4]-(float)accy_min)/(float)(accy_max-accy_min))*0.99);
				IMU_In[i][5] = INQ(((imu_temp[i][5]-(float)accz_min)/(float)(accz_max-accz_min))*0.99);
				IMU_In[i][6] = INQ(((imu_temp[i][6]-(float)gyrx_min)/(float)(gyrx_max-gyrx_min))*0.99);
				IMU_In[i][7] = INQ(((imu_temp[i][7]-(float)gyry_min)/(float)(gyry_max-gyry_min))*0.99);
				IMU_In[i][8] = INQ(((imu_temp[i][8]-(float)gyrz_min)/(float)(gyrz_max-gyrz_min))*0.99);
			}
		}

		write_data = net_process(IMU_In);
		sprintf(write_data_str, "%c" , write_data);
		OLED_ShowString(1, 3, "You haved written:", 8);		
		OLED_ShowString(1, 4, write_data_str, 8);		
		EMBARC_PRINTF("You haved written_str:   %s\r\n",write_data_str);
		NRF24L01_TX(write_data_str);	
	}

	// train Data collection
	while(0)
	{
		// Button isn't pressed
		while (!iot_button_read(2));
		
		// Data number
		printf("%d\r\n",count++);

		// Pressing the button
		while (iot_button_read(2)) 
		{

			// Read mpu9250 data
			board_delay_ms(1, 0);
			mpu9250_sensor_read(mpu9250_sensor, &mpu9250_data);

			//Send data
			//mpu6050_send_data(dev_uart0,mpu9250_data.accel_x,mpu9250_data.accel_y,mpu9250_data.accel_z,mpu9250_data.gyro_x,mpu9250_data.gyro_y,mpu9250_data.gyro_z);
			//report_imu(dev_uart0,(int)(mpu9250_data.roll*100),(int)(mpu9250_data.pitch*100),(int)(mpu9250_data.yaw*100),0,0);
			//printf("write\r\n");

			// Print data
			printf("%d %d %d %d %d %d %d %d %d ", (int16_t)(mpu9250_data.roll*100),(int16_t)(mpu9250_data.pitch*100),(int16_t)(mpu9250_data.yaw*100),
					mpu9250_data.accel_x,mpu9250_data.accel_y,mpu9250_data.accel_z,mpu9250_data.gyro_x,mpu9250_data.gyro_y,mpu9250_data.gyro_z);  

		}

		
		printf("\r\n");
	}
	// return E_SYS;
}