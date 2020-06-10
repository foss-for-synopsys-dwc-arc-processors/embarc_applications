

#include "function.h"

volatile Flag flag = { 0 , 0 , 0 };

int counter = 0 ; // count 10*500 = 5s

volatile int32_t breath_data[ LENGTH ] = { 0 } ;
volatile int32_t input_breath_data[ RENEW_TIME ] = { 0 } ;

volatile int hp_data[ BL ] = { 0 } ;
volatile int32_t input_heart_data[ RENEW_TIME ] = { 0 } ;
volatile int32_t heart_data[ LENGTH ] = { 0 } ;


// FFT
double vReal[ LENGTH ] = { 0 } ;
double vImag[ LENGTH ] = { 0 } ;
// end FFT

volatile uint64_t RTC_time = 0 ;

volatile uint16_t heart_rate = 0 , breath_rate = 0 ;

char _string[100] = { 0 };



#ifdef _ARC
// initial Hardware
AD7991_DEFINE(ad7991, DEV_ID_0, ADC_I2C_SLAVE_ADDRESS);
HM1X_DEFINE(hm1x, HM_1X_UART_ID);
// end initial Hardware
#endif


#ifdef _TEST_DATA
static int x = 0 ;
#endif

int main()
{
	uint16_t led_toggle_val = 0b10101010;
	uint32_t baudrate = 115200 ;//UART_BAUDRATE_38400;

#ifdef _ARC
	// initial Hardware
	ad7991_adc_init(ad7991);
	hm1x_init(hm1x, baudrate);
	// end initial Hardware
#endif

	// initial High pass filter data

	// end initial High pass filter data


#ifdef _ARC
	// timer ISR begin
	int_disable(INTNO_TIMER1);
	timer_stop(TIMER_1);
	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_pri_set(INTNO_TIMER1, INT_PRI_MIN);
	EMBARC_PRINTF("\r\n/******** TEST MODE START ********/\r\n\r\n");
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
	// end timer ISR begin
#endif



#ifdef _TEST_DATA
	while (x < 29317)
#else
	while (1)
#endif
	{
#ifdef _MAC
		timer1_isr();
#endif

		if ( flag.clear )
		{
			memset( heart_data , 0 , sizeof(heart_data) );
			heart_rate_average( &heart_rate ) ;
		}


		if ( flag.fft )
		{


#ifdef _ARC
			timer_stop(TIMER_1);
#endif

// shift data
//            EMBARC_PRINTF("heart_data : ");
			for (int i = 0; i < (LENGTH - RENEW_TIME) ; ++i)
			{
				// breath_data[i] = breath_data[i + 1];
				heart_data[i] = heart_data[i + RENEW_TIME];
				//	EMBARC_PRINTF(" %d ,",heart_data[i]);
			}
			for (int i = (LENGTH - RENEW_TIME); i < LENGTH ; ++i)
			{
				// breath_data[i] = input_breath_data[ i - LENGTH - RENEW_TIME ] ;
				heart_data[i]  =  input_heart_data[ i - (LENGTH - RENEW_TIME) ] ;
				//	EMBARC_PRINTF(" %d ,",heart_data[i]);
			}

#ifdef _ARC
			timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
#endif
//            EMBARC_PRINTF("\r\n");


			// calculate heart rate
			//FFT
			for (int i = 0; i < LENGTH; ++i)
			{
				vReal[i] = heart_data[i] ;
				vImag[i] = 0 ;
			}
			FFT();//filter_data => FFT_data
			//end FFT


			// find heart rate pulse from vReal
			heart_rate = (uint16_t)Heart( );//FFT_data
			// heart_rate_average( &heart_rate ) ;
			// end find heart rate pulse from vReal


			flag.fft = 0 ;
			led_write(led_toggle_val, BOARD_LED_MASK);
			led_toggle_val = ~led_toggle_val;

		}
	}

	return 0;
}


// each 10 || 20 ms
#ifdef _MAC
static void timer1_isr()//void *ptr)
#endif
#ifdef _ARC
static void timer1_isr(void *ptr)
#endif
{
	uint16_t v1 = 0 ;

#ifdef _ARC
	timer_int_clear(TIMER_1);
#endif

	RTC_time ++ ;
	if ( get_adc( 1 ) < 1000 )
		flag.clear = 1 ;
	else
		flag.clear = 0 ;


	if ( flag.clear )
	{
		memset(input_breath_data , 0 , sizeof(input_breath_data) );
		memset(input_heart_data , 0 , sizeof(input_heart_data) );
		memset(hp_data , 0 , sizeof(hp_data) );
	}

	else
	{
		input_breath_data[ counter ] = get_adc( 0 ) ;
		// High pass
		input_heart_data[ counter ] = 0 ;
		for (int j = 0; j < BL ; j++) {
			input_heart_data[ counter ] += (int32_t)hp_data[ j ] * (int32_t)B[j] ;
			if ( j == BL - 1 )
				hp_data[ j ] = get_adc( 0 ) ;
			else
				hp_data[j] = hp_data[j + 1] ;
		}
#ifdef _10MS
		input_heart_data[ counter ] /= pow(2, 15) ;
#endif
#ifdef _20MS
		input_heart_data[ counter ] /= pow(2, 8) ;
#endif
// END High pass
	}

	// calculate breath rate
	breath_rate = Breath( input_breath_data[ counter ] );//data
	// end calculate breath rate



		sprintf( _string , "%3u,%2u,%4d,%7d,%u\r\n", heart_rate , breath_rate , (int32_t)input_breath_data[counter] , (int32_t)input_heart_data[counter],flag.clear);
		//    EMBARC_PRINTF( "%s",_string );
		//    EMBARC_PRINTF("%2d\r\n",breath_rate);
		hm1x_write(hm1x, _string, strlen(_string));


	counter ++ ;

	if ( flag.clear )
	{
		counter = 0 ;
		RTC_time ++ ;
	}

	if ( counter >= RENEW_TIME ) {
		flag.fft = 1 ;
		counter = 0 ;
	}

	// EMBARC_PRINTF(" ISR counter = %3d , flag.fft = %d \r\n", counter, flag.fft);

}

uint16_t get_adc( uint8_t channel ) {
	float buffer[4] = { 0 };
	uint16_t data[4] = { 0 };

#ifndef _TEST_DATA
	ad7991_adc_read(ad7991, buffer);
	data[ channel ] = (uint16_t)buffer[ channel ] ;
#else
	data[ channel ] = data_chang[x] ;
#ifdef _10MS
	x++ ;
#else
	x += 2 ;
#endif
#endif

	return data[ channel ] ;
}


