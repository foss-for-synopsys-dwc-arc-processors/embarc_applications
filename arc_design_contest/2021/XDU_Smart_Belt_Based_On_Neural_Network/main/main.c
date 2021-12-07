#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#include "delay.h"

// External sensors
#include "LD3320a.h"
#include "dht11.h"
#include "SYN6288.h"
#include "PluseSensor.h"
#include "HC_05.h"
// Nine axis sensor
#include "mpu9250.h"
#include "inv_mpu.h"
#include "gpio.h"
#include "arc_timer.h"
#include "stdio.h"

// neural network
#include "input_buffer.h"
#include "process.h"
#include "settings.h"

#if defined(BOARD_IOTDK)
#define MPU9250_IIC_ID DFSS_IIC_0_ID
#else
#error Board isnt supported, check BOARD variable
#endif


// Nine axis sensor :DEFINE 
static MPU9250_DEFINE(mpu9250_sensor, MPU9250_IIC_ID, MPU9250_IIC_ADDRESS);   //mpu9250.h

// Nine axis sensor :priority:
static void set_i2c_irq_priority(MPU9250_DEF_PTR obj, uint32_t prio)
{
    DEV_IIC_PTR iic_obj = iic_get_dev(obj->i2c_id);
    iic_obj->iic_control(IIC_CMD_SET_IRQ_PRIO, CONV2VOID(prio));
}

// Nine axis sensor :timer interrupt
static void timer_isr(void*param)
{
    static MPU9250_DATA mp_data;
    mpu9250_sensor_read(mpu9250_sensor, &mp_data);   //mpu9250.c
    input_buffer_push_data(&mp_data);   //input_buffer.c
    timer_int_clear(TIMER);    //arc_time.c
    return;
}

// Nine axis sensor :timer interrupt start
static void start_timer(void)
{
    int_handler_install(TIMER_IRQ, timer_isr);
    timer_start(TIMER, 1 /* IE = 1 */, (CORE_FREQ_MHZ * 1000000 / TIMER_RATE_HZ));
}


uint16_t get_humanstate()
{
    pll_fout_config(CORE_FREQ_MHZ);
    set_i2c_irq_priority(mpu9250_sensor, 2);
    input_buffer_init();
    /* initializing the Sensor Hub */
    mpu9250_sensor_init(mpu9250_sensor);
    mpu_set_gyro_fsr(GYRO_DPS);
    mpu_set_accel_fsr(ACCEL_G);

    start_timer();
    uint16_t state;
    uint16_t start=1;
    while (start)
    {
        float *frame = input_buffer_get_next_buffer();
        if(frame != NULL) {
            state = process_data(frame);

            start = 0;
        }
    }
    timer_stop(TIMER);

    cpu_lock();            /* lock cpu to do initializations */
    board_init();     /* board level init */
    cpu_unlock();          /* unlock cpu to let interrupt work */

    LD3320A_init();

    audio_synth_init();

    dht11_init();

    heartRate_init();

    return state;

}

int main(void){
     
    cpu_lock();       /* lock cpu to do initializations */
    board_init();     /* board level init */
    cpu_unlock();     /* unlock cpu to let interrupt work */

    delay_s(1);    

    uint16_t state=0;
    /**********  LD3320A initialization **********/
    // Function   : speech recognition
    // Pin setting: DFSS_UART_1_ID (B3:uart1_rxd)
    uint8_t ctrl_cmd=0;
    /*********************************************/
    LD3320A_init();

    /*********** SYN6288 initialization *******/
    // Function   : speech synthesis
    // Pin setting: DFSS_UART_1_ID (B2:uart1_txd)
    /******************************************/
    audio_synth_init();

    /************* DHT11 initialization ************/
    // Function   : Temperature and humidity detection
    // Pin setting: DFSS_GPIO_8B0_ID (B7:gpio8b_0[4])
    /***********************************************/
    uint8_t dht11_rcv[4];	// temp/humi data recived from dht11
	uint8_t dht11_pre[4];   // Record the previous data
    dht11_init();
    dht11_read_data(dht11_pre);

    /*******  Pluse Sensor ***************/
    // Function   : Heart rate detection //
    // Pin setting: J1:ADC0/gpio8b_3[7]  //
    /*************************************/
    heartRate_init();
    uint16_t heart_val[1];

    /**************  HC-05 ***************/
    // Function   : Bluetooth module     //
    // Pin setting: DFSS_UART_2_ID(uart2)//
    /*************************************/
    HC_05_init();	// UART2 - HC-05
    char text[20];
  	printf("Initialization Complete!\n\r");
	

    while(1){
        ctrl_cmd = get_audio_cmd();
        switch (ctrl_cmd) {
            case CMD_START:{
                break;
            }
            case CMD_TEMPERATURE:{  /*命令“温度”*/
                printf("CMD_TEMPERATURE\n");
                // dht11_read_pre(dht11_rcv);
                if (dht11_read_data(dht11_rcv) > 0 ){
                    printf("DHT11 Output: humidity: %d.%d , temperature: %d.%d .\n",dht11_rcv[0],dht11_rcv[1],dht11_rcv[2],dht11_rcv[3]);
                    
                    sendToHC05("\n Tem:");
                    Int2String(dht11_rcv[2],text);
                    sendToHC05(text);

                    audio_synth("现在温度是");
                    audio_synth_num(dht11_rcv[2]);
                    audio_synth("度");
                    
                    audio_synth("湿度是百分之");
                    audio_synth_num(dht11_rcv[0]);
                }
                else {
                    printf("DHT11 Output: humidity: %d.%d , temperature: %d.%d .\n",dht11_rcv[0],dht11_rcv[1],dht11_rcv[2],dht11_rcv[3]);
                	printf("DHT11 Error!");
                    audio_synth("温度模块异常");
                }
                break;
            }
                
            case CMD_HEARTRATE:{    /*命令“心跳”*/
                printf("CMD_HEARTRATE  \n");
                heart_val[0] = read_BPM_pre();
                printf("heart_val=%d\n",heart_val[0]);

                sendToHC05("\n Heart:");
                Int2String(heart_val[0],text);
                sendToHC05(text);

                if (heart_val[0]<190 && heart_val[0]>40){
                    audio_synth("心跳为每分钟");
                    audio_synth_num(heart_val[0]); 
                    audio_synth("次");
                }
                else{
                    printf("PluseSensor Error!");
                    audio_synth("心率模块异常");
                }
                break;                
            }
            case CMD_MOTIONSTATE:{  /*命令“状态”*/
                //activity recongnition
                //  0,    "stop"
                //  1,    "running"
                //  2,    "Wobble, imbalance"
                //  3,    "falling"
                printf("CMD_HEARTRATE  \n");
                printf("state = %d\n",state);

                sendToHC05("\n State:");
                Int2String(state,text);
                sendToHC05(text);
                switch (state){
                    case 0:{audio_synth("休息"); break;}
                    case 1:{audio_synth("跑步"); break;}
                    case 2:{audio_synth("摇晃"); break;}
                    case 3:{audio_synth("结束"); break;}
                }
                break;
            }   
            case CMD_MOVEMENTMDOE:  /*命令“运动”*/
                printf("NO4  \n\r");
                break;
            case CMD_END:           /*命令“结束”*/
                printf("stop  \n\r");
                break;
        }


        /*** Temperature monitoring ***/
        if (dht11_read_data(dht11_rcv) > 0 ){
            printf("DHT11 Output: humidity: %d.%d , temperature: %d.%d .\n",dht11_rcv[0],dht11_rcv[1],dht11_rcv[2],dht11_rcv[3]);
            
            sendToHC05("\n Tem:");
            Int2String(dht11_rcv[2],text);
            sendToHC05(text);

            // The temperature difference is greater than 5 ℃
            if(dht11_rcv[2]>dht11_pre[2]+5){
                audio_synth("现在温度是");
                audio_synth_num(dht11_rcv[2]);
                audio_synth("升温过快 请注意防暑");
            }
            // The temperature difference is lower than 5 ℃
            if(dht11_rcv[2]<dht11_pre[2]-5){
                audio_synth("现在温度是");
                audio_synth_num(dht11_rcv[2]);
                audio_synth("降温过快 请注意保暖");
            }  
            dht11_pre[2]=dht11_rcv[2];
        }
        else {
            printf("DHT11 Error!");
            audio_synth("温度模块异常");
        }

        /***  Heart rate monitoring ***/
        heart_val[0] = getBPM();    // get heart rate value
        if(heart_val[0]>40 && heart_val[0]<190){
            printf("heart_val=%d\n",heart_val[0]);

            sendToHC05("\n Heart:");
            Int2String(heart_val[0],text);
            sendToHC05(text);
        }

        if (heart_val[0]<60 && heart_val[0]>40){
            audio_synth("心率过低 注意安全");
        }
        else if (heart_val[0]>120 && heart_val[0]<190){
            audio_synth("心率过高 注意安全");
        }   

        /*** activity recongnition ***/
        //  0,    "stop"
        //  1,    "running"
        //  2,    "Wobble, imbalance"
        //  3,    "falling"
        
        state = get_humanstate();
        printf("state = %d\n",state);
        sendToHC05(" State:");
        Int2String(state,text);
        sendToHC05(text);

        if (state == 2)
        {
            audio_synth("身体摇晃过大");
            audio_synth("注意平衡适当休息");
        }
        if (state == 3){
            while( !(ctrl_cmd == CMD_END)){
            audio_synth("本人摔倒需要帮助");
            delay_s(2);
            ctrl_cmd = get_audio_cmd();
            }
            printf("stand up\n");
        }
        
    }

    
  	printf("human state Complete!\n\r");

  return E_SYS;
}