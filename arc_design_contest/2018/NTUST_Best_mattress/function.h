
#ifndef _FUNCTION_H
#define _FUNCTION_H

// ****************************set _SAMPLE_TIME************************
//#define _10MS

#ifndef _10MS
#define _20MS
#endif

//#define COUNT (BOARD_CPU_CLOCK/1000) //   1 ms
//#define COUNT (BOARD_CPU_CLOCK/4)    //  250 ms
#ifdef _10MS
#define COUNT (BOARD_CPU_CLOCK/100)    //  10 ms
#define _SAMPLE_TIME 10 //ms
#define RENEW_TIME 600
#define LENGTH 4096
#endif
#ifdef _20MS
#define COUNT (BOARD_CPU_CLOCK/50)     //   20 ms
#define _SAMPLE_TIME 20 //ms
#define RENEW_TIME 200
#define LENGTH 2048
#endif
// ***************************end set _SAMPLE_TIME**************************


// ******************************set devices*********************************
//#define _MAC

#ifndef _MAC
#define _ARC
#endif

#ifdef _MAC
#define EMBARC_PRINTF printf
#endif
// ****************************end set devices*******************************

// ******************************set test data******************************
//#define _TEST_DATA
// ****************************end set test data****************************

#include <stdio.h>
#include <stdlib.h>

#ifdef _ARC

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_uart.h"
#include "hm1x.h"
#include "dev_iic.h"
#include "ad7991.h"

#endif

#include <math.h>
#include "FFT.h"

#ifdef _TEST_DATA
extern int data_chang [29317] ;
#endif

typedef struct FLAG{
    char fft  :1 ;
    char read :1 ;
    char clear:1 ;
}Flag;
extern volatile Flag flag ;

/*
 * highpass FIR (Equiripple)
 * fs = 100 fpass = 1 fstop = 0.5
 */

/*
 * highpass FIR (Equiripple)
 * fs = 100 fpass = .8 fstop = 0.3
 */
#define BL 101
extern const int16_t B[BL] ;

extern int counter ; // count 10*600 = 6s

extern volatile int32_t breath_data[ LENGTH ] ;
extern volatile int32_t input_breath_data[ RENEW_TIME ] ;

extern volatile int hp_data[ BL ] ;
extern volatile int32_t input_heart_data[ RENEW_TIME ] ;
extern volatile int32_t heart_data[ LENGTH ] ;

extern double vReal[ LENGTH ] ;
extern double vImag[ LENGTH ] ;

extern char _string[100] ;
extern volatile uint64_t RTC_time ;

//void ISR();
#ifdef _MAC
static void timer1_isr( void );
#endif

#ifdef _ARC
static void timer1_isr(void *ptr);
#endif

int Breath( int data );
void FFT( void ) ;
double Heart( void );
uint16_t get_adc( uint8_t channel );

#define _SAMPLE 8
void heart_rate_average( volatile uint16_t *temp  );


#endif
