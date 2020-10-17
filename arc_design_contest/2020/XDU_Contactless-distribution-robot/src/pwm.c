#include "pwm.h"
#include "board.h"
#include "tdriver.h"

static TDR pca9685_tdr;
TDR *pca9685 = NULL;


void delay_10us(volatile int z) //10us
{
	volatile uint32_t x,y;
	for(x=14;x>0;x--)
		for(y=z;y>0;y--);
}

void PCA9685_init(void)
{
    pca9685 = &pca9685_tdr;
	tdr_init(pca9685, PCA9685_IIC_ID, PCA9685_adrr);
    pwm_init();
}

bool PCA9685_is_good(void)
{
    if (tdr_state_err(pca9685)) {
		EMBARC_PRINTF("> Something error happened in IIC communication.\r\n");
		tdr_clear_error(pca9685);
        delay_10us(5);
		return false;	
	}
    return true;
}
bool PCA9685_is_ready(void)
{    
    if (tdr_state_ready(pca9685)) {
        return true;
    }
    EMBARC_PRINTF("> PCA9685 not ready!\r\n");
    return false;
}

uint8_t PCA9685_read(uint8_t reg)
{
    uint8_t regval;
    while (!PCA9685_is_good());
    while (!PCA9685_is_ready());
    do {
	    tdr_read_reg(pca9685, reg);
      
    } while (!PCA9685_is_good());
    while (!PCA9685_is_ready());
    regval = pca9685->regval;
    return regval;
}

void PCA9685_write(uint8_t reg, uint8_t regval)
{
    while (!PCA9685_is_good());
    while (!PCA9685_is_ready());
    do {
    
	    tdr_write_reg(pca9685, reg, regval);
   
    } while (!PCA9685_is_good());
    while (!PCA9685_is_ready());
}

void PCA9685_reset(void)
{
    PCA9685_write(PCA9685_MODE1, 0x00);
}

void setPWMFreq(uint8_t freq)
{
 uint8_t buf[1];
	uint8_t prescale,oldmode,newmode;
   double prescaleval;
	 
   prescaleval = 25000000.0/(4096*freq*0.915);
   prescale = (uint8_t)(prescaleval+0.5)-1;
 
   oldmode = PCA9685_read(PCA9685_MODE1);
   newmode = (oldmode&0x7F) | 0x10; // sleep

   PCA9685_write(PCA9685_MODE1, newmode); // go to sleep
   PCA9685_write(PCA9685_PRESCALE, prescale); // set the prescaler
   PCA9685_write(PCA9685_MODE1, oldmode);
   board_delay_ms(5,1);
   PCA9685_write(PCA9685_MODE1, oldmode | 0xA1); 
}

void setPWM(uint8_t num, uint16_t on, uint16_t off) 
{
    PCA9685_write(LED0_ON_L+4*num,on & 0xFF);
    PCA9685_write(LED0_ON_H+4*num,(on>>8) & 0xFF);
    PCA9685_write(LED0_OFF_L+4*num,off & 0xFF);
    PCA9685_write(LED0_OFF_H+4*num,(off>>8) & 0xFF);
}

uint16_t calculate_PWM(uint8_t angle){
    return (uint16_t)(204.8*(0.5+angle*1.0/90));
}

void pwm_init(void)
{
     PCA9685_reset();
    setPWMFreq(work_Freq);
}

void PWM1_0(void){
    uint16_t pwm = calculate_PWM(0);	
    setPWM(0x0,0,pwm);
	EMBARC_PRINTF("pwm1_0\r\n");
    board_delay_ms(1,1);
}

void PWM1_90(void){
    uint16_t pwm = calculate_PWM(90);
    setPWM(0x0,0,pwm);
	EMBARC_PRINTF("pwm1_90\r\n");
    board_delay_ms(1,1);
}
void PWM2_0(void){
    uint16_t pwm = calculate_PWM(0);	
    setPWM(0x1,0,pwm);
	EMBARC_PRINTF("pwm2_0\r\n");
	board_delay_ms(1,1);
}

void PWM2_120(void){
    uint16_t pwm = calculate_PWM(120);	
    setPWM(0x1,0,pwm);
	EMBARC_PRINTF("pwm2_120\r\n");
	board_delay_ms(1,1);
}

void PWM3_90(void){
    uint16_t pwm = calculate_PWM(90);
    setPWM(0x2,0,pwm);
	EMBARC_PRINTF("pwm3_0\r\n");
    board_delay_ms(1,1);
}
void PWM3_45(void){
    uint16_t pwm = calculate_PWM(45);	
    setPWM(0x2,0,pwm);
	EMBARC_PRINTF("pwm3_90\r\n");
	board_delay_ms(1,1);
}

void PWM3_135(void){
    uint16_t pwm = calculate_PWM(135);	
    setPWM(0x2,0,pwm);
	EMBARC_PRINTF("pwm3_180\r\n");
	board_delay_ms(1,1);
}





