#include "embARC.h"

#include "board.h"
#include "dev_uart.h"
#include "dev_pinmux.h"

#include "vlsi_uart.h"
#include "vlsi_esp8266.h"

#include "slerp.h"

#include <stdio.h>
#include "embARC_debug.h"

#define WIFI_SSID   "\"VLSILAB_2G2\""
#define WIFI_PWD    "\"ur pwd\""

#define Q_INTP 4
#define Q_SLERP (Q_INTP + 1)
float R_Q[63 * Q_SLERP] = { 0.5 };
float R_new[63 ] = { 0 };
#define COUNT (BOARD_CPU_CLOCK/1000 * 33)

char esp_data_cmd[] = "AT+CIPSEND=0,252\r\n";
static char http_get[] = "GET /";
static char http_IDP[] = "+IPD,";

struct vlsi_esp* vlsi_esp_ptr;
struct vlsi_esp *vlsi_esp8266;

void uart_init(DEV_UART_PTR* puart_obj)
{
    uart_obj = *puart_obj;

    int opend = uart_obj->uart_open(USE_BAUD);
    if (opend == AT_OK) {
      EMBARC_PRINTF("uart2 init done!\r\n");
    }

    uart_obj->uart_control(UART_CMD_SET_RXCB, uart_cb );
    uart_obj->uart_control(UART_CMD_SET_RXINT, 1);
    uart_obj->uart_control(UART_CMD_ENA_DEV, NULL);
    
    clear_rx_buf(uart_obj);

}

void esp_init(/** struct vlsi_esp* vlsi_esp8266*/)
{
    int f;
    vlsi_esp8266_init(vlsi_esp8266, 115200);
    f = vlsi_esp8266_test(vlsi_esp8266);
    EMBARC_PRINTF("test %d \r\n", f);
    board_delay_ms(1000, 1);

    
    f = vlsi_esp8266_mode_set(vlsi_esp8266);
    EMBARC_PRINTF("mode set %d \r\n", f);
    board_delay_ms(1000, 1);

    f = vlsi_esp8266_connect(vlsi_esp8266, WIFI_SSID, WIFI_PWD);
    if (f == AT_OK) {
      EMBARC_PRINTF("connect OK \r\n");
    }
    board_delay_ms(1000, 1);

    f = vlsi_esp8266_server(vlsi_esp8266);
    EMBARC_PRINTF("server %d \r\n", f);
    board_delay_ms(1000, 1);

    f = vlsi_esp8266_get_ip(vlsi_esp8266);
    board_delay_ms(1000, 1);
}

static void timer0_isr(void *ptr)
{
  EMBARC_PRINTF("time out\r\n");
  EMBARC_PRINTF("vlsi_esp8266 addr: %x", vlsi_esp8266);
  timer_int_clear(TIMER_1);
  static int i = 0;

  vlsi_esp8266_send_datas(vlsi_esp8266, esp_data_cmd, sizeof(esp_data_cmd), (char*)(R_Q+63*i), 252);
  i = (i + 1) % Q_SLERP;
}

void test_inter(DEV_UART_PTR* puart_obj)
{
  uart_obj = *puart_obj;
}

int main(void)
{
    char *conn_buf;
    char scan_result[1024];
    // set pin
    set_pmod_mux(0x151015);
    set_uart_map(0x9c);

    vlsi_esp8266 = (VLSI_ESP*)malloc(sizeof(VLSI_ESP));
    // uart init

    DEV_UART_PTR uart_obj = uart_get_dev(2);

    int opend = uart_obj->uart_open(USE_BAUD);
    if (opend == AT_OK) {
      EMBARC_PRINTF("uart2 init done!\r\n");
    }

    uart_obj->uart_control(UART_CMD_SET_RXCB, uart_cb );
    uart_obj->uart_control(UART_CMD_SET_RXINT, 1);
    uart_obj->uart_control(UART_CMD_ENA_DEV, NULL);

    clear_rx_buf(uart_obj);

    // esp8266
    /** struct vlsi_esp *vlsi_esp8266; */
    /** EMBARC_PRINTF("ptr: %d, vlsi: %d", vlsi_esp_ptr, vlsi_esp8266); */
    esp_init(/** vlsi_esp8266 */);

    char idp;

    EMBARC_PRINTF("WiFi init done... \r\n");
    EMBARC_PRINTF("esp8266 addr: %x \r\n", vlsi_esp8266);


    // initialize recieve buffer
    scan_result[0] = '\0';

    /** test_inter(uart_obj); */
//    float R_new[63];
//
//    /** char scan_result[1024] = { 0 }; */
//
//    while (1)
//    {
//        vlsi_esp8266_read(vlsi_esp_ptr, scan_result, 1000);
//
//
//        if (scan_result[0] != 0) {
//          uart_obj->uart_write("LINK", 4);
//        }
//        else
//        {
//          board_delay_ms(10, 1);
//          continue;
//        }
//
//        EMBARC_PRINTF("initial timer...\r\n");
//        int_disable(INTNO_TIMER1);
//        timer_stop(TIMER_1); 
//        int_handler_install(INTNO_TIMER1, timer0_isr);
//        int_enable(INTNO_TIMER1);
//        timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
//        EMBARC_PRINTF("initial timer done\r\n");
//
//        board_delay_ms(500, 1);
//
//        while (true)
//        {
//          for (size_t i = 0; i < 63; i++)
//          {
//            R_new[i] = R[i];
//          }
//
//          for (size_t i = 1; i <= Q_SLERP; i++)
//          {
//            quaternion_slerp(R_Q, R_new, R_Q + 63 * i, i / Q_SLERP);
//            R_Q[i + 60] = ( R_Q[60] + R_new[60] ) * i / Q_SLERP;
//            R_Q[i + 61] = ( R_Q[61] + R_new[61] ) * i / Q_SLERP;
//            R_Q[i + 62] = ( R_Q[62] + R_new[62] ) * i / Q_SLERP;
//          }
//
//          for (size_t i = 0; i < 63; i++)
//          {
//            R_Q[i] = R_new[i];
//          }
//        }
//    }
//    return 0;

//      char* send_buf = (char*)R;
//      while(1)
//      {
//        vlsi_esp8266_read(vlsi_esp8266, scan_result, 1000);
//
//        if (scan_result[0] != 0) {
//          uart_obj->uart_write("LINK", 4);
//        }
//        board_delay_ms(1000, 1);
//
//        // if have a call, start the trainsition
//        while (scan_result[0] != 0) {
//          EMBARC_PRINTF("recieve = %s ", scan_result);
//
//          // find who request
//          idp = *(strstr(scan_result, http_IDP)+5);
//          EMBARC_PRINTF("idp = %c \r\n", idp);
//
//          for (int i = 0; i < 1; ++i) {
//            vlsi_esp8266_send_datas(vlsi_esp8266, esp_data_cmd, sizeof(esp_data_cmd), send_buf, 252);
//            /** EMBARC_PRINTF("send = \n\t(%d, %d, %d, %d)\n\t(%d,%d,%d)",  */
//            /**     (*((float*)(send_buf   ))) *100, */
//            /**     (*((float*)(send_buf+1 ))) *100, */
//            /**     (*((float*)(send_buf+2 ))) *100, */
//            /**     (*((float*)(send_buf+3 ))) *100, */
//            /**     (*((float*)(send_buf+60))) *100, */
//            /**     (*((float*)(send_buf+61))) *100, */
//            /**     (*((float*)(send_buf+62))) *100 */
//            /**     ); */
//            board_delay_ms(5, 1);
//          }
//
//          /* vlsi_esp8266_close(vlsi_esp8266, idp); */
//
//          EMBARC_PRINTF("send complete %c \r\n");
//        }
//        board_delay_ms(100, 1);
//      }
//
//    return E_OK;
      char* send_buf = (char*)R;
      while(1)
      {
        vlsi_esp8266_read(vlsi_esp8266, scan_result, 1000);

        if (scan_result[0] != 0) {
          uart_obj->uart_write("LINK", 4);
        }
        board_delay_ms(1000, 1);

        // if have a call, start the trainsition
        while (scan_result[0] != 0) {
          /** EMBARC_PRINTF("recieve = %s ", scan_result); */
          /**  */
          /** // find who request */
          /** idp = *(strstr(scan_result, http_IDP)+5); */
          /** EMBARC_PRINTF("idp = %c \r\n", idp); */
          //vlsi_esp8266_send_datas(vlsi_esp8266, esp_data_cmd, sizeof(esp_data_cmd), send_buf, 252);
          //board_delay_ms(5, 1);
          //EMBARC_PRINTF("send complete %c \r\n");

          for (size_t i = 0; i < 63; i++)
          {
            R_new[i] = R[i];
          }

          for (size_t i = 1; i < Q_SLERP; i++)
          {
            for (int j = 0; j < 15; j++) {
              quaternion_slerp(R_Q +4*j, R_new +4*j, R_Q + 63 * i + 4*j, i / (float)Q_SLERP);
            }
            R_Q[i + 60] = ( R_Q[60] + R_new[60] ) * i /(float) Q_SLERP;
            R_Q[i + 61] = ( R_Q[61] + R_new[61] ) * i /(float) Q_SLERP;
            R_Q[i + 62] = ( R_Q[62] + R_new[62] ) * i /(float) Q_SLERP;
            vlsi_esp8266_send_datas(vlsi_esp8266, "AT+CIPSEND=0,252\r\n", 19, (char*)(R_Q+63*i), 252);
            /** vlsi_esp8266_send_datas(vlsi_esp8266, esp_data_cmd, sizeof(esp_data_cmd), (char*)(R_Q+63*i), 252); */
            //board_delay_ms(5, 1);
            EMBARC_PRINTF("send complete %c \r\n");
          }

          for (size_t i = 0; i < 63; i++)
          {
            R_Q[i] = R_new[i];
          }
        }
        board_delay_ms(100, 1);
      }

    return E_OK;
}
