#include "cmd.h"
#include "dbgUart.h"
#include "TskMotor.h"

#include "embARC.h"
#include "embARC_debug.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

VarAndName varname[varnum] = {
	/** enlarge 2 */
	{&lqr.k1,					"ka"},
	{&lqr.k2,					"kb"},
	{&lqr.k3,					"kc"},
	{&lqr.k4,					"kd"},
	/** enlarge 128 */
	{&AcclLPFParam,				"lpfa"},
	{&SpdLPFParam,				"lpfv"},
	{&AngLPFParam,				"lpfang"},
	{&GyroLPFParam,				"lpfg"},
	{&PRINT_PERIOD,				"putTs"},
	/** enlarge 256 */
	{&angPid.p,					"angp"},
	{&angPid.i,					"angi"},
	{&angPid.d,					"angd"},
	{&angPid.n,					"angn"},
	/** enlarge 32768 */
	{&AngleKal.Q,				"kalq"},
	{&AngleKal.R,				"kalr"},
	/** enlarge (1<<21) */
	{&ANGLEDIFF,				"angmin"},
	{&ANGLIMIT,					"angmax"},
	{&desire.Velocity,			"lv"},
	{&desire.Timeout,			"ts"},
	{&desire.Acc,				"acc"},
	{&desire.Omega,				"av"},
	{&desire.ThetaZ,			"angz"},
	{&desire.ThetaY,			"angy"}
};

char input[120];
char *cmd;

void paramCorr(void)
{
	/** *d = "space", for string decomposition */
	const char *d = " ";
	bool exit_flag = 0, gsensor_flag = 0;
	float temp, angSave = -1106667;
	int temp_int;
	MsgType msg;

	putStr("start:  start motor\r\n");
	vTaskDelay(100);
	putStr("stop:   stop motor\r\n");
	vTaskDelay(100);
	putStr("angle:  Enable angle\r\n");
	vTaskDelay(100);
	putStr("stop:   stop motor\r\n");
	vTaskDelay(100);
	putStr("put:    print data\r\n");
	vTaskDelay(100);	
	putStr("time,spd,kal,imu,pwm,lqr\r\n");
	vTaskDelay(100);
	putStr("q:    stop data print\r\n");
	vTaskDelay(100);
	putStr("Format:	str num\r\n");
	vTaskDelay(100);
	putStr("angpid kal lqr desire\r\n");
	vTaskDelay(100);
	putStr("lpfParam, angle range \r\n");
	vTaskDelay(100);
	putStr("print period \r\n");
	vTaskDelay(100);

	while(1){

        putStr("input:\r");
        vTaskDelay(50);
        
        /** wait for cmd input */
		UartGetLine(input);
		
		/** string decomposition, obtain command */
		cmd = strtok(input, d);

	    for(int i = 0; i < varnum; i++){
	    	//start system
	    	if(!strcmp(cmd, "k")) {
	    		gsensor_flag = 0;
	    		*varname[22].value = angSave;
				putStr("start acqZeros\r\n");
	        	vTaskDelay(50);
	        	msg = EnableAcqZeros;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	putStr("please don't touch within 2s\r\n");
	        	vTaskDelay(2000);

	        	putStr("Enable Motor\r\n");
	        	vTaskDelay(50);
	        	msg = DisableAcqZeros;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	msg = EnableMotors;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	msg = EnableAng;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	msg = EnableSeqSet;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}
        	/*
        	 * APP control 
        	 * w  --  lv + 0.1m/s
        	 * b  --  lv - 0.1m/s
        	 * l  --  yaw angle + 0.2rad
        	 * r  --  yaw angle - 0.2rad
        	 * t  --  lv = 0m/s
        	 * g  --  start posture control
        	 * c  --  change center of gravity   
        	 */
        	else if(!strcmp(cmd, "w")) {
        		*varname[17].value += 209715;
        		putStr("v=%5.3f", *varname[17].value / 2097152.f);
        		msg = EnableSeqSet;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	else if(!strcmp(cmd, "b")) {
        		*varname[17].value -= 209715;
        		putStr("v=%5.3f", *varname[17].value / 2097152.f);
        		msg = EnableSeqSet;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	else if(!strcmp(cmd, "l")) {
        		*varname[21].value += 419430;
        		putStr("ang=%5.3f", *varname[21].value / 2097152.f);
        		msg = EnableSeqSet;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	else if(!strcmp(cmd, "r")) {
        		*varname[21].value -= 419430;
        		putStr("yaw=%5.3f", *varname[21].value / 2097152.f);
        		msg = EnableSeqSet;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	else if(!strcmp(cmd, "t")) {
        		*varname[17].value = 0;
        		putStr("v=%5.3f", *varname[17].value / 2097152.f);
        		msg = EnableSeqSet;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	else if(!strcmp(cmd, "s")) {
				putStr("disable motor\r\n");
	        	vTaskDelay(50);
				msg = DisableMotors;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	else if(!strcmp(cmd, "g")){
        		putStr("Enable gravity modify\r\n");
        		gsensor_flag = 1;
	        	break;
        	}

        	else if(!strcmp(cmd, "c") && (gsensor_flag==1)){
        		cmd = strtok(NULL, d);
	        	temp = atof(cmd);
        		*varname[22].value += (int)(temp * 1048576);
        		putStr("pitch=%5.3f", *varname[22].value / 2097152.f);
	        	break;
        	}

        	else if(!strcmp(cmd, "zero")){
				putStr("Enable acqZeros\r\n");
	        	vTaskDelay(50);
				msg = EnableAcqZeros;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}  

        	else if(!strcmp(cmd, "angle")){
				putStr("Enable angle\r\n");
	        	vTaskDelay(50);
				msg = EnableAng;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}  

        	else if(!strcmp(cmd, "put")){
				putStr("print data\r\n");
	        	vTaskDelay(50);
				msg = EnablePutStr;
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	        	break;
        	}

        	 else if(!strcmp(cmd, "time")){
				putStr("calc time\r\n");
	        	vTaskDelay(50);
				PRINT_TIME = true;
	        	break;
        	}   

        	else if(!strcmp(cmd, "lqr")){
				DEBUG_LQR = true;
	        	vTaskDelay(20);
				msg = EnablePutStr;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
				putStr("print lqr param\r\n");
	        	break;
        	}

        	 else if(!strcmp(cmd, "imu")){
				PRINT_IMU = true;
	        	vTaskDelay(20);
				msg = EnablePutStr;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
				putStr("print imu data\r\n");
	        	break;
        	}           	

        	else if(!strcmp(cmd, "pwm")){
				PRINT_PWM = true;
	        	vTaskDelay(20);
				msg = EnablePutStr;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
				putStr("print pwm qei data\r\n");
	        	break;
        	} 

        	else if(!strcmp(cmd, "q")){
				PRINT_IMU = false;
				PRINT_PWM = false;
				DEBUG_LQR = false;
	        	vTaskDelay(10);
				msg = DisablePutStr;	
	        	xQueueSend(motMbCmd, &msg, portMAX_DELAY);
				putStr("stop data print\r\n");
	        	break;
        	}

	    	else if(!strcmp(cmd, varname[i].name)) {  		
	    		vTaskDelay(50);
	    		/** string decomposition, obtain params variation */
	        	cmd = strtok(NULL, d);
	        	temp = atof(cmd);

	        	if(isnan(temp))
	            	temp = *varname[i].value;
	            switch(i){
	            	case 0:
	            	case 1:
	            	case 2:
	            	case 3:
	            		temp_int = (int)(temp * 2);	        	
	            		putStr("\t %s = %5.3f (was %5.3f) \r\n", varname[i].name,
	                			*varname[i].value / 2.f + temp, *varname[i].value / 2.f);
	            		break;
	            	case 4:
	            	case 5:
	            	case 6:
	            	case 7:
	            	case 8:
	            		temp_int = (int)(temp * 128);	        	
	            		putStr("\t %s = %5.3f (was %5.3f) \r\n", varname[i].name,
	                			*varname[i].value / 128.f + temp, *varname[i].value / 128.f);
	            		break;
	            	case 9:
	            	case 10:
	            	case 11:
	            	case 12:
	            		temp_int = (int)(temp * 256);	        	
	            		putStr("\t %s = %5.3f (was %5.3f)\r\n", varname[i].name,
	                			*varname[i].value / 256.f + temp, *varname[i].value / 256.f);
	            		break;
	            	case 13:
	            	case 14:
	            		temp_int = (int)(temp * 32768);	        	
	            		putStr("\t %s = %5.3f (was %5.3f)\r\n", varname[i].name,
	                			*varname[i].value / 32768.f + temp, *varname[i].value / 32768.f);
	            		break;
	            	default:
	            		temp_int = (int)(temp * 2097152);	        	
	            		putStr("\t %s = %5.4f (was %5.4f)\r\n", varname[i].name,
	                			*varname[i].value / 2097152.f + temp, *varname[i].value / 2097152.f);
	            		break;
	            }

	        	vTaskDelay(100);
	        	*varname[i].value += temp_int;
	        	
	        	if(i < 17) 
	        		putStr("data change after input stop\r\n");
	    		else {
	    			putStr("data change right now\r\n");
	    			msg = EnableSeqSet;	
	        		xQueueSend(motMbCmd, &msg, portMAX_DELAY);
	    		}
	    		break;
	    	}

       		else if(!strcmp(cmd, "exit")) {
            	exit_flag = 1;
            	break;
        	}

	    	vTaskDelay(50);
        }

        if(exit_flag){
            break;
        }
	}
}         