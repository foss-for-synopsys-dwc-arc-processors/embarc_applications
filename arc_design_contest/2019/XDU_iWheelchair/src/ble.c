/**
*Copyright: Copyright (c) 2019
*Created on 2019-8-12
*Author: YuaniLee
*/
#include "ble.h"
#include "buzzer.h"

#include "drive_control.h"


/*******Use PMOD1*******/

void ble_control(uint8_t ble_rcv_buf[]) {
  switch (ble_rcv_buf[0]) {
    EMBARC_PRINTF("%s\n", ble_rcv_buf);
  case 'p':
    car_set(0,PWM_L,PWM_R);  // stop
    EMBARC_PRINTF("ble command is %s，停下\r\n", ble_rcv_buf);
  break;
  case 'w':
    car_set(1,PWM_L,PWM_R);  // forward
    EMBARC_PRINTF("ble command is %s，向前\r\n", ble_rcv_buf);
  break;
  case 'a':
    car_set(2,PWM_L,PWM_R);  // left
    EMBARC_PRINTF("ble command is %s，向左\r\n", ble_rcv_buf);
  break;
  case 'd':
    car_set(3,PWM_L,PWM_R);  // right
    EMBARC_PRINTF("ble command is %s， 向右\r\n", ble_rcv_buf);
  break;
  case 's':
    car_set(6,PWM_L,PWM_R);  // backward
    EMBARC_PRINTF("ble command is %s， 向前\r\n", ble_rcv_buf);
  break;
  case 'u':
    car_set(4,PWM_L,PWM_R);  // speed up
    EMBARC_PRINTF("ble command is %s,speed=%d， 加速\r\n", ble_rcv_buf, speed);
  break;
  case 'i':
    car_set(5,PWM_L,PWM_R);  // speed down
    EMBARC_PRINTF("ble command is %s， 减速\r\n", ble_rcv_buf);
  break;
  case 'b':
    buzzer_open();
    EMBARC_PRINTF("buzzer_open\n");
  break;
  case 'n':
    buzzer_close();
    EMBARC_PRINTF("buzzer_close\n");
  break;
  default:
    EMBARC_PRINTF("ble command error\r\n");
  break;
  }
}
