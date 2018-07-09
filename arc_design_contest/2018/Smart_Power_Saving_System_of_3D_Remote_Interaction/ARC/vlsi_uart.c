#include "embARC.h"
#include "embARC_debug.h"

#include "vlsi_uart.h"
float pose[105] = {0};
/* float R[4*15] = {0}; */
float R[4*15 + 3] = {0};
float* sent_R;
float T[3*15] = {0};

void clear_rx_buf(DEV_UART* uart_obj){
  uint8_t read_chr[1024];
  int num;
  uart_obj->uart_control(UART_CMD_GET_RXAVAIL, (void *)&num);
  while(num > 0){
    uart_obj->uart_read((void*)read_chr, num);
    uart_obj->uart_control(UART_CMD_GET_RXAVAIL, (void *)&num);
  }
}

void uart_cb(DEV_UART* uart_obj, void* ptr){
  static int rear = 0;
  static char uart_buf[252];
  int num;
  int cnt = 0;
  bool full_flag = false;

  uart_obj->uart_control(UART_CMD_GET_RXAVAIL, (void *)&num);
  if (rear+num > MAX_UART_BUF_SIZE) {
    cnt = MAX_UART_BUF_SIZE - rear+1;
    full_flag = true;
  }
  else {
    /** return; */
    cnt = num;
    full_flag = false;
  }

  uart_obj->uart_read((void*)(uart_buf + rear), cnt);
  rear += cnt;

  if (full_flag) {
    rear = 0;
    for (int i = 0; i < 15; ++i) {
      R[i*4+0] = *(((float*)uart_buf)+i*4+0);
      R[i*4+1] = *(((float*)uart_buf)+i*4+1);
      R[i*4+2] = *(((float*)uart_buf)+i*4+2);
      R[i*4+3] = *(((float*)uart_buf)+i*4+3);
      /* pose[i] = ((float*)uart_buf)[i]; */
      /** T[i*3+0] = ((float*)uart_buf)[i*7+0]; */
      /** T[i*3+1] = ((float*)uart_buf)[i*7+1]; */
      /** T[i*3+2] = ((float*)uart_buf)[i*7+2]; */
      /** R[i*4+0] = ((float*)uart_buf)[i*7+3]; */
      /** R[i*4+1] = ((float*)uart_buf)[i*7+4]; */
      /** R[i*4+2] = ((float*)uart_buf)[i*7+5]; */
      /** R[i*4+3] = ((float*)uart_buf)[i*7+6]; */
    }
    R[60] = ((float*)uart_buf)[15*4+0];
    R[61] = ((float*)uart_buf)[15*4+1];
    R[62] = ((float*)uart_buf)[15*4+2];
    
//    int r0 = R[0] *1000;
//    int r1 = R[1] *1000;
//    int r2 = R[2] *1000;
//    int r3 = R[3] *1000;
//    int r4 = R[60] *1000;
//    int r5 = R[61] *1000;
//    int r6 = R[62] *1000;
//    EMBARC_PRINTF("\nR: \n\t(%d, %d, %d, %d)\n\t(%d,%d,%d)", 
//        r0, 
//        r1, 
//        r2, 
//        r3, 
//        r4, 
//        r5, 
//        r6
//        );
    /* printf("%d, %d, %d ------", (int)(pose[0]*1000), (int)(pose[1]*1000), (int)(pose[2]*1000)); */
    /* printf("%d, %d, %d \r\n", (int)(pose[98]*1000), (int)(pose[99]*1000), (int)(pose[100]*1000)); */
    // clear_rx_buf(uart_obj);
  }
}

