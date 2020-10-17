#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "board.h"

#include "sr501.h"
#include "keypad.h"
#include "motordrive.h"
#include "pwm.h"
#include "sim900.h"
#include "ultradrive.h"
#include "syn6288.h"
#include "chinfo.h"


#define uchar unsigned char
#define uint unsigned int
uint8_t *a;
uint8_t q;//body detect sign
uint8_t b;



uint8_t *phone; //telephone number
uint8_t *verify;//verify password
uint8_t *password;//input password

 

int  main(void) 
{    
 		
	cpu_lock();	/* lock cpu to do initializations */

	board_init();	/* board level init */

	cpu_unlock();	/* unlock cpu to let inetrrupt work */

	EMBARC_PRINTF("init...\r\n");
    HC_SR501Configuration();
	Motor_Init();
	keypad_init();
	sim900_init();
	PCA9685_init();
	UltrasonicWave_Init();
	SYN6288_init();
    board_delay_ms(100,1);
	
while(1)
{

    EMBARC_PRINTF("TONIGHT!!!\r\n");

//body detect---------------------------------------------
  
     while(!(q==1))
	 	{
	        q=HC_SR501_Status();
	        board_delay_ms(500,1);
	    };
	board_delay_ms(500,1);
	EMBARC_PRINTF("Welcome...\r\n");
	SYN6288_info(INFO_1);
	board_delay_ms(7000,1);
	
//key out-------------------------------------------------s

    PWM1_0(); 
    
//input telephone number---------------------------------- 

	while(!(keypad_read()==12));
	
	SYN6288_info(INFO_2);
	board_delay_ms(5000,1);
	EMBARC_PRINTF("Please input telephone number...\r\n"); 
    phone = VerifyString(11);
	//EMBARC_PRINTF("tele:%s\r\n",phone); 
	while(!(keypad_read ()==13));//input Enter
	EMBARC_PRINTF("Successful!\r\n");
	SYN6288_info(INFO_3);
	board_delay_ms(2500,1);
	
//input target--------------------------------------------

    EMBARC_PRINTF("Please input target...\r\n"); 
	SYN6288_info(INFO_4);
 	board_delay_ms(3000,1);
	a=(uint8_t *)malloc(4); 
    a = VerifyString(1);
	b=a[3]-48;
    //EMBARC_PRINTF("%s\r\n",a); 
	EMBARC_PRINTF("%d\r\n",b); 
    while(!(keypad_read ()==13));
	EMBARC_PRINTF("Successful!\r\n");
	SYN6288_info(INFO_3);
	board_delay_ms(2500,1);
	
 //open box-----------------------------------------------
 
    EMBARC_PRINTF("Finish set! Please input thing...\r\n"); 
 	SYN6288_info(INFO_5);
	board_delay_ms(5500,1);
    PWM2_0();
    while(!(keypad_read ()==13));
    PWM2_120();
	
//move ---------------------------------------------------

	board_delay_ms(1000,1);
	EMBARC_PRINTF("Start move...\r\n");
    SYN6288_info(INFO_6);
	board_delay_ms(3000,1);
	PWM1_90();
	board_delay_ms(1000,1);
	car_control(b);//choose route

//send message and call-----------------------------------

    verify=(uint8_t *)malloc(16);
	verify = send(phone);	
    EMBARC_PRINTF("send successfuly\r\n");
	SYN6288_info(INFO_8);
	board_delay_ms(5000,1);
	call(phone);
	

    
//body detect---------------------------------------------

    board_delay_ms(5000,1);
     while(!(q==1))
	 	{
	        q=HC_SR501_Status();
	        board_delay_ms(500,1);
	    };
//input password------------------------------------------

	PWM1_0();
	   while(1){
	        EMBARC_PRINTF("Please input password...\r\n"); 
			SYN6288_info(INFO_9);
	        board_delay_ms(5000,1); 
	   		password=(uint8_t *)malloc(16); 
	        password = VerifyString(4);
            //EMBARC_PRINTF("%s\r\n",password);
            while(!(keypad_read ()==13));
			board_delay_ms(1000,1);
            if(password[3]==verify[3]&&password[7]==verify[7]&&password[11]==verify[11]&&password[15]==verify[15])
                  	{
				    	SYN6288_info(INFO_10);
						board_delay_ms(3000,1);
						PWM2_0();
	                	break;
			   		}
            else
 				    {
 				    	board_delay_ms(1000,1);
			        	EMBARC_PRINTF("\r\n");
						SYN6288_info(INFO_11);
						board_delay_ms(4000,1);
            		}
				}
	        	 
//finish one task-----------------------------------------

	while(!(keypad_read ()==13));
	     PWM2_120();
	board_delay_ms(2000,1);
	EMBARC_PRINTF("Finish!!!\r\n");
	SYN6288_info(INFO_12);
	board_delay_ms(5000,0);
	     PWM1_90();
	board_delay_ms(5000,1);
}
return E_SYS;  /* system error */  

}

	   
	

