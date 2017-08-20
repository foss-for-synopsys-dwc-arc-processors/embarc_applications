
#include "mpu6050.h"

static DEV_IIC *mpu_iic_port;
static uint32_t point=(MUPADDRESS>>1);
/*Initialize the IIC device and set the slave address to MPU6050.*/
static void iic_mpu6050_int(void)
{
	mpu_iic_port = iic_get_dev(MPU6050_IIC_NUM);
  /*Set the IIC speed as host standard speed*/
  mpu_iic_port -> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
  mpu_iic_port -> iic_control(IIC_CMD_ENA_DEV, (void *)point);
  mpu_iic_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point);

}
/*MPU6050 write register function.
 *parameter:regaddr.The address of the register that you want to write.
 *parameter:val.The valuation that you want to write.
*/
static void mpu6050_write_reg(uint8_t regaddr,uint8_t val)
{
  uint8_t send_data[2];
  uint8_t point;
  /*Place the register address value and the data you want to send into the array.*/
  send_data[0] = regaddr;
  send_data[1] = val;
  /*Set the IIC send end operation to stop*/
  mpu_iic_port -> iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_STOP));
  mpu_iic_port -> iic_write((void*)send_data,2);
}
/*MPU6050 read register function.
 *parameter:regaddr.The address of the register that you want to write.
 *return value:The valuation of the register.
*/
static uint8_t mpu6050_read_reg(uint8_t regaddr)
{
  uint8_t REG_data;
  uint8_t send_data = regaddr;

  mpu_iic_port -> iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_RESTART));
  mpu_iic_port -> iic_write((void*)(&send_data),1);
  mpu_iic_port -> iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_STOP));
  mpu_iic_port -> iic_read((void*)(&REG_data), 1);

  return REG_data;
}
/*MPU6050 init.
 *parameter:gyro_rng.Set the range of gyro,0(250 degree/s),1(500 degree/2),2(1000 degree/s),3(2000 degree/s).
 *parameter:accel_rng.Set the range of accelerometer,0(2g),1(4g),2(8g),3(16g).
*/
void mpu6050_init(uint8_t gyro_rng,uint8_t accel_rng)
{  
  iic_mpu6050_int();
  /*Reset MPU6050.*/
  mpu6050_write_reg(MPU_PWR_MGMT1_REG,0X80); 
  vTaskDelay(100);   
  /*Wake up MPU6050*/           
  mpu6050_write_reg( MPU_PWR_MGMT1_REG, 0x07 );	
  /*Set sampling frequency divider*/
  mpu6050_write_reg( SMPLRT_DIV, 0x06);  
  /*Set the low-pass filter*/   
  mpu6050_write_reg( LPF_CONFIG, 0x06 );
  /*Turn off all interrupts*/    
  mpu6050_write_reg(MPU_INT_EN_REG,0x00);     
  /*Turn off IIC master mode*/
  mpu6050_write_reg(MPU_USER_CTRL_REG,0X00); 
  /*Close fifo*/ 
  mpu6050_write_reg(MPU_FIFO_EN_REG,0X00);
  /*Set interrupt pin active low*/
  mpu6050_write_reg(MPU_INTBP_CFG_REG,0X80); 
  /*Set CLKSEL, PLL, X axis for reference*/   
  mpu6050_write_reg(MPU_PWR_MGMT1_REG,0X01); 
  /*Enable accelerometer and gyroscope*/ 
  mpu6050_write_reg(MPU_PWR_MGMT2_REG,0X00);
  /*Configure the gyroscope full scale*/
  mpu6050_write_reg( MPU_GYRO_CFG_REG, (gyro_rng<<3));
  /*Configure the accelerometer full scale*/
  mpu6050_write_reg( MPU_ACCEL_CFG_REG, (accel_rng<<3)); 
}
/*Set the IIC slave address to MPU6050.*/
void iic_point_mpu(void)
{
  mpu_iic_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point);
  if (mpu_iic_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point) == E_OK) {
      //EMBARC_PRINTF("Target reset mpu6050 OK!\n");
  }
}
/*Get the MPU6050 data.
 *parameter:the address of the register that you want to get.
*/
int mpu6050_get_data(uint8_t regaddr)
{
  int data;
  uint16_t data_temp;
  uint8_t H = 0,L = 0;
  /*Read high 8 bits data.*/
  H = mpu6050_read_reg( regaddr );
  /*Read low 8 bits data.*/
  L = mpu6050_read_reg( regaddr + 1 );
  data_temp=( H << 8 )|L; 
  /*Change the data into a signed integer.*/
  if(data_temp<32768)
  {
    data=data_temp;
  }
  else
  {
    data=-(65535-data_temp);
  }
  return data;
}