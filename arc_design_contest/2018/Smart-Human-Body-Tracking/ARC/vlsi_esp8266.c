/* #include "embARC.h" */
#include "dev_uart.h"
#include "vlsi_esp8266.h"
#include "ringbuffer.h"
#include "stdlib.h"
#include "embARC_error.h"
/* #include <stdio.h> */
#include "board.h"
#include "embARC_debug.h"
#include <string.h>

char at[] = "AT\r\n";

char rx_buf[1000] = {0};
int rx_flag = 0;

static void rx_cb(void* ptr)
{
  DEV_UART * uart_obj = (DEV_UART*)ptr;
  int rd_avail = 0;
  int cnt = 0;

  uart_obj->uart_control(UART_CMD_GET_RXAVAIL, (void*)(&rd_avail));
  while (rd_avail > 0) {
    cnt = (rd_avail < UART_MAX_RX)? rd_avail:(UART_MAX_RX-1);
    uart_obj->uart_read((void*)(rx_buf + rx_flag), cnt);
    rx_flag += cnt;
    uart_obj->uart_control(UART_CMD_GET_RXAVAIL, (void*)(&rd_avail));
  }
}


int vlsi_esp8266_read(VLSI_ESP *obj, char *buf, int cnt)
{
  for (int i = 0; i < cnt; ++i) {
    buf[i] = rx_buf[i];
  }
  return 0;
}

int vlsi_esp8266_init(VLSI_ESP *obj, int baudrate)
{
  obj->uart = uart_get_dev(0);
  int32_t ercd = obj->uart->uart_open(baudrate);
  if (ercd != E_OK && ercd != E_OPNED) {
    EMBARC_PRINTF("INIT ERROR\r\n");
    return AT_ERROR;
  }
  /* obj->uart->uart_control(UART_CMD_SET_BAUD, baudrate); */
  obj->uart->uart_control(UART_CMD_SET_RXCB, (void*)rx_cb);
  obj->uart->uart_control(UART_CMD_SET_RXINT, 1);

  for (int i = 0; i < 1000; ++i) {
    obj->rx.rx_buf[i] = 0;
  }
  obj->rx.rx_index = 0;

  DEV_UART_INFO_SET_EXTRA_OBJECT(&(obj->uart->uart_info), &(obj->rx));
  return AT_OK;
}

int vlsi_esp8266_get_repley(VLSI_ESP *obj, char* buf, int time_out)
{
  for(;;)
  {
    board_delay_ms(10, 1);
    if ((strstr(rx_buf, "OK") != NULL) || (strstr(rx_buf, "ERROR") != NULL))
    {
      EMBARC_PRINTF("break\r\n");
      break;
    }
  }

  EMBARC_PRINTF("--------------------------\r\n");
  EMBARC_PRINTF("reply rxbuf:\r\n%s", rx_buf);
  EMBARC_PRINTF("--------------------------\r\n");

  int f = AT_ERROR;
  if (strstr(rx_buf, AT_OK_STR) != NULL)
    f = AT_OK;

  for (int i = 0; i < 1000; ++i)
    rx_buf[i] = 0;

  rx_flag = 0;
  return f;
}

int vlsi_esp8266_wait_repley(VLSI_ESP *obj)
{
  for(int i=0;;i++)
  {
    board_delay_ms(5, 1);
    /** EMBARC_PRINTF("rx_buf: %s \r\n", rx_buf); */
    if ((strstr(rx_buf, "OK") != NULL) || (strstr(rx_buf, "ERROR") != NULL))
    {
      EMBARC_PRINTF("break\r\n");
      
      break;
    }
    /** if (i > 120) */
    /** { */
    /**   return AT_NOREPLEY; */
    /** } */
  }

  EMBARC_PRINTF("--------------------------\r\n");
  EMBARC_PRINTF("reply rxbuf:\r\n%s", rx_buf);
  EMBARC_PRINTF("--------------------------\r\n");

  int f = AT_ERROR;
  if (strstr(rx_buf, AT_OK_STR) != NULL)
    f = AT_OK;

  for (int i = 0; i < 1000; ++i)
    rx_buf[i] = 0;

  rx_flag = 0;
  return f;
}

//int vlsi_esp8266_set_baud(VLSI_ESP *obj, int baud)
//{
//  char cmd[] = "AT+UART_CUR=230400,8,1,0,0\r\n";
//  obj->uart->uart_write(cmd, sizeof(cmd));
//  int f = vlsi_esp8266_wait_repley(obj);
//
//  f = obj->uart->uart_control(UART_CMD_SET_BAUD, 230400);
//  EMBARC_PRINTF("uart baud = %d \r\n", f);
//
//  return f;
//}

int vlsi_esp8266_test(VLSI_ESP *obj)
{
  int f = AT_NOREPLEY;
  /** obj->uart->uart_write(at, sizeof(at)); */
  /** int f = vlsi_esp8266_wait_repley(obj); */

  while (f == AT_NOREPLEY)
  {
    obj->uart->uart_write(at, sizeof(at));
    //f = vlsi_esp8266_wait_repley(obj);
    for (int i = 0; i < 20; i++) {
      board_delay_ms(5,1);
      if ((strstr(rx_buf, "OK") != NULL) || (strstr(rx_buf, "ERROR") != NULL))
      {
        EMBARC_PRINTF("break\r\n");
        
        EMBARC_PRINTF("--------------------------\r\n");
        EMBARC_PRINTF("reply rxbuf:\r\n%s", rx_buf);
        EMBARC_PRINTF("--------------------------\r\n");

        int f = AT_ERROR;
        if (strstr(rx_buf, AT_OK_STR) != NULL)
          f = AT_OK;

        for (int i = 0; i < 1000; ++i)
          rx_buf[i] = 0;

        rx_flag = 0;
        return f;
      }
    }
    f = AT_NOREPLEY;
  }

  return f;
}

int vlsi_esp8266_mode_set(VLSI_ESP *obj)
{
  char cmd[] = "AT+CWMODE_CUR=3\r\n";
  obj->uart->uart_write(cmd, sizeof(cmd));

  char buf[100] = {0};
  /* int f = vlsi_esp8266_get_repley(obj, buf, 100); */
  int f = vlsi_esp8266_wait_repley(obj);
  return f;
}

int vlsi_esp8266_connect(VLSI_ESP *obj, char* ssid, char* key)
{
  char cmd[] = "AT+CWJAP_CUR=\"VLSILAB_2G2\",\"vlsi95514\"\r\n";
  /* EMBARC_PRINTF("wifi cmd: %s\r\n", cmd); */

  obj->uart->uart_write(cmd, sizeof(cmd));

  /* int f = vlsi_esp8266_get_repley(obj, NULL, 100); */
  int f = vlsi_esp8266_wait_repley(obj);
  return f;
}

int vlsi_esp8266_server(VLSI_ESP *obj)
{
  char cmd[] = "AT+CIPMUX=1\r\n";
  obj->uart->uart_write(cmd, sizeof(cmd));
  /* int f = vlsi_esp8266_get_repley(obj, NULL, 100); */
  int f = vlsi_esp8266_wait_repley(obj);

  board_delay_ms(1000, 1);

  char cmd2[] = "AT+CIPSERVER=1,80\r\n";
  obj->uart->uart_write(cmd2, sizeof(cmd2));

  /* f = vlsi_esp8266_get_repley(obj, NULL, 100); */
  f = vlsi_esp8266_wait_repley(obj);

  return f;
}

int vlsi_esp8266_get_ip(VLSI_ESP *obj)
{
  char cmd[] = "AT+CIFSR\r\n";
  obj->uart->uart_write(cmd, sizeof(cmd));
  /* int f = vlsi_esp8266_get_repley(obj, NULL, 100); */
  int f = vlsi_esp8266_wait_repley(obj);
  
  return f;
}

int vlsi_esp8266_send(VLSI_ESP *obj, char *buf, char idp)
{
  char cmd[] = "AT+CIPSEND=0,60\r\n";

  int index = strstr(cmd, "0");
  if (index != NULL) {
    EMBARC_PRINTF("index=%d, cmd=%d", index, cmd);
    cmd[(int)(cmd-index)] = idp;
  }

  EMBARC_PRINTF("\r\n------------------------------\r\n");
  EMBARC_PRINTF("send cmd = %s", cmd);
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  /* EMBARC_PRINTF("send cmd = %d\r\n", sizeof(cmd)); */
  /* obj->uart->uart_write(cmd, sizeof(cmd)); */
  obj->uart->uart_write(cmd, sizeof(cmd));
  int f = vlsi_esp8266_get_repley(obj, NULL, 420);

  EMBARC_PRINTF("\r\n------------------------------\r\n");
  EMBARC_PRINTF("send buf = %s", buf);
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  if(f == 0)
  {
    obj->uart->uart_write(buf, 60);
  }

  f = vlsi_esp8266_get_repley(obj, NULL, 420);
  return 0;
}

int vlsi_esp8266_close(VLSI_ESP *obj, char idp)
{
  char cmd[] = "AT+CIPCLOSE=0\r\n";

  int index = strstr(cmd, "0");
  if (index != NULL) {
    EMBARC_PRINTF("index=%d, cmd=%d", index, cmd);
    cmd[(int)(cmd-index)] = idp;
  }

  EMBARC_PRINTF("\r\n------------------------------\r\n");
  EMBARC_PRINTF("send cmd = %s", cmd);
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  obj->uart->uart_write(cmd, sizeof(cmd));
  int f = vlsi_esp8266_get_repley(obj, NULL, 100);

  return f;
}


int vlsi_esp8266_send_pose(VLSI_ESP *obj, char *buf, char idp)
{
  /* char pose_len = 1920; */
  char pose_len = 240;
  char cmd[] = "AT+CIPSEND=0,240\r\n";

  int index = strstr(cmd, "0");
  if (idp != NULL) {
    cmd[index] = idp;
  }

  EMBARC_PRINTF("\r\n------------------------------\r\n");
  EMBARC_PRINTF("send cmd = %s", cmd);
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  obj->uart->uart_write(cmd, sizeof(cmd));

  int f = vlsi_esp8266_get_repley(obj, NULL, 100);

  for (int i = 0; i < 1; ++i) {
    obj->uart->uart_write(buf, 240);
  }

  f = vlsi_esp8266_get_repley(obj, NULL, 100);
  return f;
}


int vlsi_esp8266_send_pose_(VLSI_ESP *obj, char *buf, char idp)
{
  /* char pose_len = 1920; */
  char pose_len = 240;
  char cmd[] = "AT+CIPSEND=0,240\r\n";

  int index = strstr(cmd, "0");
  if (idp != NULL) {
    cmd[index] = idp;
  }

  EMBARC_PRINTF("\r\n------------------------------\r\n");
  EMBARC_PRINTF("send cmd = %s", cmd);
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  obj->uart->uart_write(cmd, sizeof(cmd));

  int f = vlsi_esp8266_get_repley(obj, NULL, 100);

  for (int i = 0; i < 1; ++i) {
    obj->uart->uart_write(buf, 240);
  }

  f = vlsi_esp8266_get_repley(obj, NULL, 100);
  return f;
}

int vlsi_esp8266_send_datas(VLSI_ESP *obj, char* cmd, int cmd_size, char* buf, int buf_size)
{
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  EMBARC_PRINTF("send cmd = %s, size = %d", cmd, cmd_size);
  EMBARC_PRINTF("vlsi_esp8266 addr: %x", obj);
  EMBARC_PRINTF("\r\n------------------------------\r\n");
  obj->uart->uart_write(cmd, cmd_size);
  int f = vlsi_esp8266_wait_repley(obj);
  EMBARC_PRINTF("reply code: %d", f);

  obj->uart->uart_write(buf, buf_size);
  f = vlsi_esp8266_wait_repley(obj);
  return f;
}
