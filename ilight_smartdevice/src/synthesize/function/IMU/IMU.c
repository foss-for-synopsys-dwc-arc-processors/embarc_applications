#include "imu.h"

/***************Attitude calculation***********************/

static MPU050_UPDATA n_data;
static ACC_UPBUF acc_upbuf;
static ACC_SPECIAL acc_special_data;


MPU050_UPDATA *mpu6050_updata_ptr=&n_data;

/*Action captures state variables
 *7   :Capture success flag,1 represents one capture is complete
 *6:1 :Capture type
 *0   :1 represents get a start point
*/
static uint8_t partern_rec_startflag;
static uint8_t shaking_rec_startflag;		

/*Get the start and stop point ,set the first position of partern_rec_startflag */
void imu_get_stastp_point(void)
{
	/*If the value of acceleration difference is bigger than the threshold of start,set the first bit of partern_rec_startflag one.Get the start of action.*/
	if ((mpu6050_updata_ptr->acc_judge_dif_buf[1]>=JUDGE_START_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[0]>=JUDGE_START_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[3]<=JUDGE_START_THRESHOLD))
	{
		partern_rec_startflag|=0x01;
	}
	/*If the start action has been got and the value of acceleration difference is smaller than the threshold of stop,set the eighth bit of partern_rec_startflag one.*/
	if ((mpu6050_updata_ptr->acc_judge_dif_buf[1]<=JUDGE_STOP_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[0]<=JUDGE_STOP_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[3]>=JUDGE_STOP_THRESHOLD))
	{
		if(partern_rec_startflag&0x01)
		{
			partern_rec_startflag&=0xfe;
			partern_rec_startflag|=0x80;
		}
	}
}
void imu_shaing_get_stastp_point(void)
{
	/*If the value of acceleration difference is bigger than the threshold of start,set the first bit of partern_rec_startflag one.Get the start of action.*/
	if ((mpu6050_updata_ptr->acc_judge_dif_buf[1]>=JUDGE_START_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[0]>=JUDGE_START_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[3]<=JUDGE_START_THRESHOLD))
	{
		shaking_rec_startflag|=0x01;
	}
	/*If the start action has been got and the value of acceleration difference is smaller than the threshold of stop,set the eighth bit of partern_rec_startflag one.*/
	if ((mpu6050_updata_ptr->acc_judge_dif_buf[1]<=JUDGE_STOP_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[0]<=JUDGE_STOP_THRESHOLD)&&(mpu6050_updata_ptr->acc_judge_dif_buf[3]>=JUDGE_STOP_THRESHOLD))
	{
		if(shaking_rec_startflag&0x01)
		{
			shaking_rec_startflag&=0xfe;
			shaking_rec_startflag|=0x80;
		}
	}
}
/*Get the start of shaking action,used in shaking mode.*/
int8_t imu_get_shaking_start(void)
{
	if((mpu6050_updata_ptr->acc_judge_y_buf[1]<-1000)&&(mpu6050_updata_ptr->acc_judge_y_buf[1]<=mpu6050_updata_ptr->acc_judge_y_buf[2]))
	{
		return ACTION_START;
	}else
	{
		return 0;
	}
}

/**Gets the sign of the first peak
   *parameter:*point .The address of the highest bit 0f the acceleration buffer that need to obtain the eigenvalue.
   *parameter:multi .Set the threshold for positive and negative feature judgments.z:0.3,y:0.4.
   *parameter：recognition.
   *parameter:peak_order. The order of the peak of y axies or z axies.
   *parameter:last_value_flag. Sign of get the first positive or negative .
*/ 
void get_acc_special(int32_t *point,float multi,uint32_t *recognition,uint8_t *peak_order,uint8_t peak_flag)
{
	uint8_t i,last_value_flag=0;
	int32_t max_value=0,min_value=0,*cal_point=point,posi_zero_value,nega_zero_value;
	*recognition=0;
	/*Get the the maximum value and minimum value.*/
	for(i=ACC_BUF_NUM-1;i>ACC_BUF_WINDOW_NUM;i--)
	{
		if(*cal_point>max_value)
		{
			max_value=*cal_point;
		}
		if(*cal_point<min_value)
		{
			min_value=*cal_point;
		}
		cal_point--;
	}
	/*Point the pointer to the first address of the acceleration cache.*/
	cal_point+=(ACC_BUF_NUM-1-ACC_BUF_WINDOW_NUM);
	/*Get the first positive peak's position.*/
	if(peak_flag==GET_PEAK_PO)
	{
		for(i=ACC_BUF_NUM-1;i>ACC_BUF_WINDOW_NUM;i--)
		{
			/*If the value is bigger than 0.8 times of the maximum,calibrate the first positive peak's position.'*/
			if(*cal_point>=(max_value*0.8))
			{
				*peak_order=i;
				break;
			}
			cal_point--;
		}
	}else
	/*Get the first negative peak's position.*/
	{
		for(i=ACC_BUF_NUM-1;i>ACC_BUF_WINDOW_NUM;i--)
		{
			if((*cal_point)<=(min_value*0.8))
			{
				/*If the value is smaller than 0.8 times of the minimal,calibrate the first negative peak's position.'*/
				*peak_order=i;
				break;
			}
			cal_point--;
		}
	}
	/*Set the threshold for determining acceleration 0.*/
	posi_zero_value=0.1*max_value;
	nega_zero_value=0.1*min_value;
	/*Set the positive and negative thresholds.If the max_value*multi is smaller than JUDGE_SYMBOL_THRESHOLD,the threshold will be set as JUDGE_SYMBOL_THRESHOLD.*/
	max_value=(max_value*multi>JUDGE_SYMBOL_THRESHOLD)?max_value*multi:JUDGE_SYMBOL_THRESHOLD;
	min_value=(min_value*multi<-JUDGE_SYMBOL_THRESHOLD)?min_value*multi:-JUDGE_SYMBOL_THRESHOLD;

	//EMBARC_PRINTF("The max_value:%d!   The min_value:%d\r\n",max_value,min_value);

	/**Judge the positive and negative of the intercept waveform
	  *If the judgment waveform is positive，the variable "(*recognition)" is shifted by two bits and takes 0x02.Take "last_value_flag" with 0x02.
	  *If the judgment waveform is negative，the variable "(*recognition)" is shifted by two bits and takes 0x01.Take "last_value_flag" with 0x01.
	  *Take "last_value_flag" with 0x00.
	*/
	for(i=ACC_BUF_NUM-1;i>=ACC_BUF_WINDOW_NUM;i--)
	{
	/**If the last feature is not negative(Judge by the second bit of last_value_flag,if the second bit of last_value_flag is not one.),\
	   and the value is smaller than min_value.
	*/
			if(((*point)<min_value)&&((last_value_flag&0x02)==0x00))
			{
				(*recognition)=((*recognition)<<2)|NEGATIVE;
				last_value_flag&=0x00;
				last_value_flag|=NEGATIVE;
	       		EMBARC_PRINTF("!!!The NEGATIVE!!!\r\n");
			}
	/**If the last feature is not zero(Judge by the first and second bits of last_value_flag,if these bits of last_value_flag are zero.) */
			if((last_value_flag&0x03)!=0x00)
			{
				if(((*point)>nega_zero_value)&&(*point)<posi_zero_value)
				{
					last_value_flag&=0x00;
					EMBARC_PRINTF("Zero!\r\n");
				}
			}
	/**If the last feature is not positive(Judge by the first bit of last_value_flag,if the first bit of last_value_flag is not one.) */
			if(((*point)>max_value)&&(((*recognition)&0x01)==0x00))
			{
				(*recognition)=((*recognition)<<2)|POSITIVE;
				last_value_flag&=0x00;
				last_value_flag|=POSITIVE;
	       		EMBARC_PRINTF("!!!The POSITIVE!!!\r\n");
			}
			point--;
	}
	EMBARC_PRINTF("The (*recognition):%d\r\n",(*recognition));
}
void mpu6050_updata()
{	
	static uint8_t filter_cnt=0;		//Filter cache flag
	//float scope_data[4];
	uint32_t cal_acc_temp[3],acc_sum;//cal_gyro_temp[3];
	uint32_t acc_dif_temp=0;//gyro_dif_temp=0;
	int8_t i;
	int acc_temp[3]={0};

	acc_upbuf.acc_x_buf[filter_cnt] = mpu6050_get_data(ACCEL_XOUT_H);		//Update the moving average
  	acc_upbuf.acc_y_buf[filter_cnt] = mpu6050_get_data(ACCEL_YOUT_H);
  	acc_upbuf.acc_z_buf[filter_cnt] = mpu6050_get_data(ACCEL_ZOUT_H); 	

  	for (i=0;i<FILTER_ACC_NUM;i++)
  	{
    	acc_temp[0] += acc_upbuf.acc_x_buf[i];
    	acc_temp[1] += acc_upbuf.acc_y_buf[i];
    	acc_temp[2] += acc_upbuf.acc_z_buf[i];
  	}

  	acc_temp[0] = acc_temp[0] / FILTER_ACC_NUM;
  	acc_temp[1] = acc_temp[1] / FILTER_ACC_NUM;
  	acc_temp[2] = acc_temp[2] / FILTER_ACC_NUM;

	mpu6050_updata_ptr->acc_x=16*((acc_temp[0])*1000)/(32768);		//Calculate the acceleration value
	mpu6050_updata_ptr->acc_y=16*((acc_temp[1])*1000)/(32768);
	mpu6050_updata_ptr->acc_z=16*((acc_temp[2])*1000)/(32768);
	//EMBARC_PRINTF("acc_x:%d.     acc_y:%d.     acc_z:%d.\r\n",mpu6050_updata_ptr->acc_x,mpu6050_updata_ptr->acc_y,mpu6050_updata_ptr->acc_z);

	for (i=ACC_BUF_WINDOW_NUM-1;i>0;i--)
	{
		mpu6050_updata_ptr->acc_judge_x_buf[i]=mpu6050_updata_ptr->acc_judge_x_buf[i-1];
	}
	mpu6050_updata_ptr->acc_judge_x_buf[0]=mpu6050_updata_ptr->acc_x;
	for (i=ACC_BUF_WINDOW_NUM-1;i>0;i--)
	{
		mpu6050_updata_ptr->acc_judge_y_buf[i]=mpu6050_updata_ptr->acc_judge_y_buf[i-1];
	}
	mpu6050_updata_ptr->acc_judge_y_buf[0]=mpu6050_updata_ptr->acc_y;
	for (i=ACC_BUF_WINDOW_NUM-1;i>0;i--)
	{
		mpu6050_updata_ptr->acc_judge_z_buf[i]=mpu6050_updata_ptr->acc_judge_z_buf[i-1];
	}
	mpu6050_updata_ptr->acc_judge_z_buf[0]=mpu6050_updata_ptr->acc_z;

	if (partern_rec_startflag&0x01){
		
		for (i=ACC_BUF_NUM-1;i>ACC_BUF_WINDOW_NUM;i--)
		{
			mpu6050_updata_ptr->acc_judge_y_buf[i]=mpu6050_updata_ptr->acc_judge_y_buf[i-1];
			mpu6050_updata_ptr->acc_judge_x_buf[i]=mpu6050_updata_ptr->acc_judge_x_buf[i-1];
			mpu6050_updata_ptr->acc_judge_z_buf[i]=mpu6050_updata_ptr->acc_judge_z_buf[i-1];
		}
		mpu6050_updata_ptr->acc_judge_y_buf[ACC_BUF_WINDOW_NUM]=mpu6050_updata_ptr->acc_judge_y_buf[ACC_BUF_WINDOW_NUM-1]*4;
		mpu6050_updata_ptr->acc_judge_x_buf[ACC_BUF_WINDOW_NUM]=mpu6050_updata_ptr->acc_judge_x_buf[ACC_BUF_WINDOW_NUM-1];
		mpu6050_updata_ptr->acc_judge_z_buf[ACC_BUF_WINDOW_NUM]=mpu6050_updata_ptr->acc_judge_z_buf[ACC_BUF_WINDOW_NUM-1]+1000;	
	}

	cal_acc_temp[0]=(mpu6050_updata_ptr->acc_judge_x_buf[1]-mpu6050_updata_ptr->acc_judge_x_buf[0]);
	cal_acc_temp[1]=(mpu6050_updata_ptr->acc_judge_y_buf[1]-mpu6050_updata_ptr->acc_judge_y_buf[0]);
	cal_acc_temp[2]=(mpu6050_updata_ptr->acc_judge_z_buf[1]-mpu6050_updata_ptr->acc_judge_z_buf[0]);
	
	acc_upbuf.acc_dif_buf[filter_cnt]=abs(cal_acc_temp[0])+abs(cal_acc_temp[1])+abs(cal_acc_temp[2]);

	for(i=0;i<FILTER_ACC_NUM;i++)
	{
		acc_dif_temp += acc_upbuf.acc_dif_buf[i];
	}

	acc_dif_temp = acc_dif_temp/FILTER_ACC_NUM;

	for(i=ACC_BUF_NUM-1;i>0;i--)
	{
		mpu6050_updata_ptr->acc_judge_dif_buf[i]=mpu6050_updata_ptr->acc_judge_dif_buf[i-1];
	}
	mpu6050_updata_ptr->acc_judge_dif_buf[0]=acc_dif_temp;

	// scope_data[0]=mpu6050_updata_ptr->acc_y;
	// scope_data[1]=mpu6050_updata_ptr->acc_z;
	// scope_data[2]=mpu6050_updata_ptr->acc_judge_y_buf[ACC_BUF_WINDOW_NUM];
	// scope_data[3]=mpu6050_updata_ptr->acc_judge_z_buf[ACC_BUF_WINDOW_NUM];

	filter_cnt++;
	
  	if (filter_cnt==FILTER_ACC_NUM) 
  	{
  		filter_cnt=0;
  	}
	//OutPut_Data(scope_data);
	//EMBARC_PRINTF("Updata OK\r\n");
}

uint8_t imu_get_action_shaking()
{
	uint8_t i,y_order,z_order;
	uint32_t recognition_z=0,recognition_y=0;
	imu_shaing_get_stastp_point();
	imu_get_stastp_point();
	if (shaking_rec_startflag&0x80)
	{
		EMBARC_PRINTF("GET IN  THE DATA DEALING.\r\n");
		get_acc_special(&(mpu6050_updata_ptr->acc_judge_y_buf[ACC_BUF_NUM-1]),JUDGE_WAVE_NE_THRESHOLD,&recognition_y,&y_order,GET_PEAK_PO);
		get_acc_special(&(mpu6050_updata_ptr->acc_judge_z_buf[ACC_BUF_NUM-1]),JUDGE_WAVE_PO_THRESHOLD,&recognition_z,&z_order,GET_PEAK_NE);
		//EMBARC_PRINTF("The CLOCK recognition_y:%d\r\n!    The CLOCK recognition_z:%d\r\n! ",recognition_y,recognition_z);
		if((recognition_z>=WAVE_PN)||(recognition_y>=WAVE_PN))
		{
			shaking_rec_startflag=0;
			return ACTION_SHAKING;
		}
	}
	return 0;
}

uint8_t imu_pattern_recongnition(void)
{
	uint8_t i,y_order,z_order;
	/*start_action_get_flag,Used to indicate that the initial action has been successfully identified */
	static uint8_t start_action_get_flag;
	uint32_t recognition_z=0,recognition_y=0;
	uint8_t action_result=0;
	imu_get_stastp_point();
	if (partern_rec_startflag&0x80)
	{
		get_acc_special(&(mpu6050_updata_ptr->acc_judge_y_buf[ACC_BUF_NUM-1]),JUDGE_WAVE_NE_THRESHOLD,&recognition_y,&y_order,GET_PEAK_PO);
		get_acc_special(&(mpu6050_updata_ptr->acc_judge_z_buf[ACC_BUF_NUM-1]),JUDGE_WAVE_PO_THRESHOLD,&recognition_z,&z_order,GET_PEAK_NE);
		//EMBARC_PRINTF("The recognition_y:%d\r\n!    The recognition_z:%d\r\n! ",recognition_y,recognition_z);
		//EMBARC_PRINTF("The Y order:%d!   The Z order:%d！\r\n",y_order,z_order);
		/*Only when the system does not enter the mode of switching state,we will judge whether we have got the T. */
		if(start_action_get_flag==0)
		{
			/***Judge T!  Z:6,2,38.  Y:9,38*/
			if((recognition_z==WAVE_PN)||(recognition_z==WAVE_N)||(recognition_z==WAVE_NPN))
			{
				if((recognition_y==WAVE_NP)||(recognition_y==WAVE_NPN))
				{
					/* Judge first action is vertical or vertical to reduce miscarriage of justice */
					if(y_order>z_order)
					{
						start_action_get_flag=0x01;
						EMBARC_PRINTF("Recognition OK!!  :T!\r\n");
						action_result=ACTION_START;
					}
				}
			}
		}
		/*Only when the system enter the mode of switching state,we will judge left or right. */
		 if(start_action_get_flag!=0)
		{
			if((recognition_y==WAVE_NP)||(recognition_y==WAVE_NPN))
			{
				if((recognition_z==0)||(recognition_z==WAVE_P)||(recognition_z==WAVE_N))
				{
					action_result=ACTION_LEFT;
					EMBARC_PRINTF("Recognition OK!!  :LEFT\r\n");
				}
			}
			if((recognition_y==WAVE_PN)||(recognition_y==WAVE_PNP))
			{
				if((recognition_z==0)||(recognition_z==WAVE_P)||(recognition_z==WAVE_N))
				{
					action_result=ACTION_RIGHT;
					EMBARC_PRINTF("Recognition OK!!  :RIGHT\r\n");
				}
			}
		}
		 if(start_action_get_flag!=0)
		{
			if((recognition_z==WAVE_PNP)||(recognition_z==WAVE_NP))
			{
				if((recognition_y==WAVE_NP))
				{
					action_result=ACTION_OVER;
					//EMBARC_PRINTF("Recognition OK!!  :out\r\n");
					start_action_get_flag=0;
				}
			}
		}
		partern_rec_startflag&=0x7f;
		for(i=0;i<ACC_BUF_NUM;i++)
		{
			mpu6050_updata_ptr->acc_judge_x_buf[i]=0;
			mpu6050_updata_ptr->acc_judge_y_buf[i]=0;
			mpu6050_updata_ptr->acc_judge_z_buf[i]=0;

		}
		//EMBARC_PRINTF("\r\n");
	}

	return action_result;
}