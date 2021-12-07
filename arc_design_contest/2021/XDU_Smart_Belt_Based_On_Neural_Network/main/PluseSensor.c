#include "PluseSensor.h"

static SMIC_ADC_DEF adc;

int BPM_pre=0;

int read_BPM_pre(){
    return BPM_pre;
}

void smic_adc_isr(void *ptr){
    SMIC_ADC_DEF* heartRate_adc = &adc;
    smic_adc_int_isr(heartRate_adc,ptr);
}


void heartRate_init(){
    SMIC_ADC_DEF* heartRate_adc = &adc;

    const uint32_t adc_int_num = 97;

    INT_HANDLER ADC_HANDER = int_handler_get(adc_int_num);

    heartRate_adc->adc_mode = ADC_MODE_CONTINUOUS;
    heartRate_adc->adc_channel_max = ADC_CHANNEL_MAX;
    heartRate_adc->adc_open_cnt = 0;
    heartRate_adc->adc_intno = ADC_INT_NUM;
    heartRate_adc->adc_handle = (INT_HANDLER)smic_adc_isr;
    heartRate_adc->adc_reg = (SMIC_ADC_REG_PTR)ADC_CRTL_BASE;

    smic_adc_open(heartRate_adc);

    smic_adc_control(heartRate_adc,ADC_MODE_CONTINUOUS,CONV2VOID(0x01 << 0));
}




int32_t heartRate_read(){
    SMIC_ADC_DEF* heartRate_adc = &adc;
    uint16_t bim[1];
    uint8_t channel = 0;
    
    // Take the average value for many times
    uint32_t a = 0;
    for (int i = 0; i < 5; i++)
    {
        smic_adc_read_polling(heartRate_adc,channel,bim);
        a = a+bim[0];
    }
    a = a/5;
    return a;
}

int32_t getBPM(){
    int BPM=0;                   // used to hold the pulse rate
    int Signal;                // holds the incoming raw data
    int IBI = 600;             // holds the time between beats, must be seeded! 
    unsigned char Pulse = false;     // true when pulse wave is high, false when it's low
    unsigned char QS = false;
    int rate[10];                    // array to hold last ten IBI values
    unsigned long sampleCounter = 0;          // used to determine pulse timing
    unsigned long lastBeatTime = 0;           // used to find IBI
    int P ;                      // used to find peak in pulse wave, seeded
    int T ;                     // used to find trough in pulse wave, seeded
    int thresh = 0;                // used to find instant moment of heart beat, seeded
    int amp = 100;                   // used to hold amplitude of pulse waveform, seeded
    unsigned char firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
    unsigned char secondBeat = false;      // used to seed rate array so we startup with reasonable BPM

    uint32_t timeNum=0;
    uint16_t runningTotal=0;
	uint8_t i;
	uint16_t Num;
    int a;    

    printf("Begin Heart check\n");
    for (int i = 0; i < 10; i++)
    {

        delay_ms(2);
        a= heartRate_read(); 
        // printf("a=%d",a);
        thresh = a + thresh;
        a = thresh;
    }
    thresh = thresh/10;
    T = thresh;
    P = thresh;
    // printf("thresh= %d\n",thresh);

    while (timeNum<10000)
    {
        timeNum=timeNum+1;
        delay_ms(2);
        Signal = heartRate_read();                   // read the Pulse Senso
        // printf("Signal=%d\n",Signal);

        sampleCounter += 2;                          // keep track of the time in mS with this variable
        Num = sampleCounter - lastBeatTime;          // monitor the time since the last beat to avoid noise	

        //发现脉冲波的波峰和波谷
        //  find the peak and trough of the pulse wave
        if(Signal < thresh && Num > (IBI/5)*3){     // avoid dichrotic noise by waiting 3/5 of last IBI
            if (Signal < T){                        // T is the trough
            T = Signal;                             // keep track of lowest point in pulse wave 
            }
        }

        if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
            P = Signal;                             // P is the peak
        }                                           // keep track of highest point in pulse wave

        //开始寻找心跳
        //当脉冲来临的时候，signal的值会上升
        //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
        //  signal surges up in value every time there is a pulse
        if (Num > 250){                                 // avoid high frequency noise
            // printf("Signal=%d,thresh=%d,Pulse=%d,Num=%d\n",Signal,thresh,Pulse,Num);
            if ( (Signal > thresh) && (Pulse == false) && (Num > (IBI/5)*3) ){    
                // printf("Signal=%d,thresh=%d,Pulse=%d,Num=%d\n",Signal,thresh,Pulse,Num);    
                Pulse = true;                               // set the Pulse flag when we think there is a pulse
                
                IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
                lastBeatTime = sampleCounter;               // keep track of time for next pulse

                if(secondBeat){                             // if this is the second beat, if secondBeat == TRUE
                    secondBeat = false;                     // clear secondBeat flag
                    for(i=0; i<=9; i++){                    // seed the running total to get a realisitic BPM at startup
                        rate[i] = IBI;                         
                    }
                }

                if(firstBeat){                              // if it's the first time we found a beat, if firstBeat == TRUE
                    firstBeat = false;                      // clear firstBeat flag
                    secondBeat = true;                      // set the second beat flag
                    continue;                                 // IBI value is unreliable so discard it
                }   

                // keep a running total of the last 10 IBI values
                // runningTotal = 0;                  		// clear the runningTotal variable    

                for(i=0; i<=8; i++){                        // shift data in the rate array
                    rate[i] = rate[i+1];                    // and drop the oldest IBI value 
                    runningTotal += rate[i];                // add up the 9 oldest IBI values
                }

                rate[9] = IBI;                          // add the latest IBI to the rate array
                runningTotal += rate[9];                // add the latest IBI to runningTotal
                runningTotal /= 10;                     // average the last 10 IBI values 
                BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
                // printf("BPM=%d, runningTotal=%d\n",BPM,runningTotal);
                QS = true;                              // set Quantified Self flag 
                // QS FLAG IS NOT CLEARED INSIDE THIS ISR
            }                       
        }

        //脉冲开始下降
        if (Signal < thresh && Pulse == true){      // when the values are going down, the beat is over
        	// printf("Signal=%d,thresh=%d,Pulse=%d\n",Signal,thresh,Pulse);  
            Pulse = false;                          // reset the Pulse flag so we can do it again
            amp = P - T;                            // get amplitude of the pulse wave
            thresh = amp/2 + T;                     // set thresh at 50% of the amplitude
            P = thresh;                             // reset these for next time
            T = thresh;
        }

        // 没有检测到脉冲，设置默认值
        if (Num > 2500){                            // if 2.5 seconds go by without a beat
            lastBeatTime = sampleCounter;           // bring the lastBeatTime up to date        
            firstBeat = true;                       // set these to avoid noise
            secondBeat = false;                     // when we get the heartbeat back
            for (int i = 0; i < 10; i++)
            {

                delay_ms(2);
                a= heartRate_read(); 
                // printf("a=%d",a);
                thresh = a + thresh;
                a = thresh;
            }
            thresh = thresh/10;
            T = thresh;
            P = thresh;
            // printf("thresh= %d\n",thresh);
        }

    }
    if(BPM>40 && BPM<190){
        BPM_pre = BPM;
    }

    printf("Heart check over\n");
    return BPM;
    
errer:
    return 0;
}












