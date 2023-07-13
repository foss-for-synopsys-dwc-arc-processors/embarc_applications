/* ------------------------------------------
 * Copyright (c) 2018, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <state.h>
// EMSK
#include <emsk_sdcard.h>
#include <emsk_sdcard_spi.h>

// IoTDk
// #include <sdcard.h>
// #include <sdcard_sdio.h>


#include <embARC.h>
#include <embARC_debug.h>

#include <board.h>
#include <esp8266.h>




#define WIFI_SSID   "\"embarc\""
#define WIFI_PWD    "\"12344321\""

static char http_get[] = "+IPD,0,";
static char http_IDP[] = "+IPD,";
static char http_server_buf[2048];
// static char http_sddata_buf[2048];
static char data_request[] = "Upload SD Card Data";


char data_save[100];
char flag_buff[3];                              // State of ABC regions.
char vaild_buff[1];

int line;
char *time_get_1;
char *time_get_2;

struct DATA {
    char year[4];
    char month[2];
    char day[2];
    char hour[2];
    char minute[2];
    char second[2];
    int people_num;
    int people_num_pre;          // historical data
    int people_num_pre_pre;      // Earlier historical data
    int people_num_buf;
    int people_num_pre_buf;
} device_A, device_B, device_C;



char *num_get;                              // Number of people
int total_people_num;                       // The total number of people
int total_people_num_pre;                   // historical data
int total_people_num_vaild;                 // Valid data for non-outliers

char Month_trans[] = "month";
char Date_trans;
char *Warning = "Warning! Too many people in the area!   ";
char *data;
char *time_all;
char *time_year;
char *time_month;
char *time_day;
char *time_hour;
char *time_minute;
char *time_second;
char *send_to_server_first = " There are ";
char *send_to_server_last = " people in the area.   At ";
char *colon = ":";
char *full_stop = ".";




/************************************************************/
// Create queues to calculate cumulative values
struct Queue {
    int *pBase;
    int front;
    int rear;
    int len;
};

void initQueue(struct Queue * q, int len);
bool inQueue(struct Queue * q, int total_people_num);
bool outQueue(struct Queue * q);
bool isEmpty(struct Queue * q);
bool isFull(struct Queue * q);
void printList(struct Queue * q);
void clearQueue(struct Queue * q);
void average(struct Queue * q, int* sum, float* ave);
void variance(struct Queue * q, float* var, float ave);

// Abnormal Data Processing
int abnormal_data_processing(float var, float ave);
// Data Storage in SD Card
int data_storage(char *buff);
// Data Storage in SD Card
int data_save_send(char *buff, int line);
// Output analysis results and population status
void print_process(char *buff);

int  data_retransmit(char *buff);
// Converts an integer type to a string type
char *itoa(int num, char *str, int radix);
// Take a piece of data from a string
char* substring(char* ch, int pos, int length);
// String concatenation
char *word_strcat(char *first_word, char *mid_word, char *last_word);

int data_save_send(char *buff, int line);
int data_read(char *buff, int line);


// Using Mean and Variance to Find Outlier Data
int   sum = 0;                      // sum total
float ave = 0.0;                    // average value
float var = 0.0;                    // variance


char *conn_buf;


/*******************************************************************/

int main(void) {
    // char *conn_buf;
    // ESP8266 Init
    EMBARC_PRINTF("========================== Init ==="
                  "=========================\n");

    ESP8266_DEFINE(esp8266);
    esp8266_init(esp8266, UART_BAUDRATE_115200);

    at_test(esp8266->p_at);
    board_delay_ms(100, 1);

    // Set Mode
    EMBARC_PRINTF("============================ Set Mode ==="
                  "=========================\n");

    esp8266_wifi_mode_get(esp8266, false);
    board_delay_ms(100, 1);
    esp8266_wifi_mode_set(esp8266, 3, false);
    board_delay_ms(100, 1);

    // Connect WiFi
    EMBARC_PRINTF("============================ Connect WiFi ==="
                  "=========================\n");

    do {
        esp8266_wifi_scan(esp8266, http_server_buf, WIFI_SSID);
        EMBARC_PRINTF("Searching for WIFI %s ......\n", WIFI_SSID);
        board_delay_ms(100, 1);
    } while (strstr(http_server_buf, WIFI_SSID) == NULL);

    EMBARC_PRINTF("WIFI %s found! Try to connect\n", WIFI_SSID);

    while (esp8266_wifi_connect(esp8266, WIFI_SSID, WIFI_PWD, false) != AT_OK) {
        EMBARC_PRINTF("WIFI %s connect failed\n", WIFI_SSID);
        board_delay_ms(100, 1);
    }

    EMBARC_PRINTF("WIFI %s connect succeed\n", WIFI_SSID);

    // Creat Server
    EMBARC_PRINTF("============================ Connect Server ===="
                  "========================\n");

    esp8266_tcp_server_open(esp8266, 80);

    // Show IP
    EMBARC_PRINTF("============================ Show IP ==="
                  "=========================\n");

    esp8266_address_get(esp8266);
    board_delay_ms(100, 1);

    char http_server_buf_1[2048];

    int data_process(char *buff);


// Initialize queues as data buffers
    int*  p1 = &sum;
    float* p2 = &ave;
    float* p3 = &var;
    struct Queue q;
    initQueue(&q, 12);


    while (1) {
        // Initialize array
        memset(http_server_buf, 0, sizeof(http_server_buf));
        memset(http_server_buf_1, 0, sizeof(http_server_buf_1));
        // Receive WIFI data
        at_read(esp8266->p_at , http_server_buf , 1000);
        // board_delay_ms(1000, 1);
        // strcpy(http_server_buf_1, http_server_buf);
        snprintf(http_server_buf_1, sizeof(http_server_buf_1), "%s",
                 http_server_buf);
        data_process(http_server_buf);

//        cloud_side_collaborative_rocessing(http_server_buf,total_people_num);

        if (strstr(http_server_buf, http_get) != NULL) {
            inQueue(&q, total_people_num);
            outQueue(&q);
            // printList(&q);
            average(&q, p1, p2);
            variance(&q, p3, ave);
        }
// Send the processed data to the cloud
        // cloud_side_collaborative_rocessing(http_server_buf,total_people_num);
        if (strstr(http_server_buf, http_get) != NULL) {
            // char *char_total_people_num[4];
            char char_total_people_num[4];
            abnormal_data_processing(var, ave);
            // EMBARC_PRINTF("%d,velid", total_people_num_vaild);

            if (total_people_num_vaild > 2000) {
                EMBARC_PRINTF("\nThere are %d people in the area."
                              "\n ", total_people_num_vaild);
                EMBARC_PRINTF("Warning! Too many people in the area! \n \n");

                int length = 58;
                itoa(total_people_num_vaild, char_total_people_num, 10);
                char *send_to_server =
                    (char *)malloc(strlen(send_to_server_first)
                                   + strlen(char_total_people_num));

                snprintf(send_to_server, sizeof(send_to_server), "%s",
                         send_to_server_first);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, char_total_people_num);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, send_to_server_last);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, time_hour);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, colon);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, time_minute);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, colon);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, time_second);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, full_stop);

                send_to_server[length] = '\0';

                EMBARC_PRINTF("\n======================= Data sent to the cloud"
                              "============================\n\n");

                conn_buf = strstr(esp8266, http_IDP) + 5;
                *(conn_buf + 1) = 0;
                esp8266_connect_write(esp8266, send_to_server, conn_buf, 50);
                board_delay_ms(100, 1);
                esp8266_connect_write(esp8266, Warning, conn_buf, 40);
                board_delay_ms(100, 1);

                EMBARC_PRINTF("\n=========================== Send finished ==="
                              "=============================\n\n");
// The number of people below the threshold will not be reported to the cloud
            } else if (total_people_num_vaild > 500
                       && total_people_num_vaild < 2000) {
                EMBARC_PRINTF("There are %d people in the "
                              "area. ", total_people_num_vaild);

                int length = 58;
                itoa(total_people_num_vaild, char_total_people_num, 10);
                char *send_to_server =
                    (char *) malloc(strlen(send_to_server_first)
                                    + strlen(char_total_people_num));
                snprintf(send_to_server, sizeof(send_to_server), "%s",
                         send_to_server_first);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, char_total_people_num);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, send_to_server_last);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, time_hour);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, colon);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, time_minute);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, colon);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, time_second);
                snprintf(send_to_server, sizeof(send_to_server), "%s%s",
                         send_to_server, full_stop);


                // EMBARC_PRINTF("%s",send_to_server);

                send_to_server[length] = '\0';
                EMBARC_PRINTF("\n======================= Data sent to the cloud"
                              "===========================\n\n");

                conn_buf = strstr(http_server_buf, http_IDP) + 5;
                *(conn_buf + 1) = 0;
                esp8266_connect_write(esp8266, send_to_server, conn_buf, 50);
                board_delay_ms(50, 1);

                EMBARC_PRINTF("\n=========================== Send finished"
                              " ================================\n\n");
            }
        }
        board_delay_ms(500, 1);
    }

    return E_OK;
}

// Data aggregation, processing and analysis
int data_process(char *buff) {
    if (strstr(buff, http_get) != NULL) {
        time_year = substring(buff, 17, 4);
        time_month = substring(buff, 21, 2);
        time_day = substring(buff, 23, 2);
        time_hour = substring(buff, 25, 2);
        time_minute = substring(buff, 27, 2);
        time_second = substring(buff, 29, 2);
        time_all = substring(buff, 17, 31);

        if (strstr(time_month, "01") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Jan");}
        if (strstr(time_month, "02") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Feb");}
        if (strstr(time_month, "03") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Mar");}
        if (strstr(time_month, "04") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Apr");}
        if (strstr(time_month, "05") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "May");}
        if (strstr(time_month, "06") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Jun");}
        if (strstr(time_month, "07") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Jul");}
        if (strstr(time_month, "08") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Agu");}
        if (strstr(time_month, "09") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Sept");}
        if (strstr(time_month, "10") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Oct");}
        if (strstr(time_month, "11") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Nov");}
        if (strstr(time_month, "12") != NULL)
        {snprintf(Month_trans, sizeof(Month_trans), "%s", "Dec");}

        switch (buff[12]) {
        case 'A': {
            EMBARC_PRINTF("\n============================== Receive Data ==="
                          "==============================\n\n");
            EMBARC_PRINTF("%s\n", buff);
            EMBARC_PRINTF("\n============================ Receive Finished ==="
                          "============================\n");
            EMBARC_PRINTF("\n========================== Start Data Analysis ==="
                          "===========================\n");
            // int people_num_pre;
            num_get = substring(buff, 13, 4);
            // String type to integer type
            device_A.people_num = atoi(num_get);
            memcpy(data_save, &buff[12], 15);
            EMBARC_PRINTF("\n============================= Number of people ==="
                          "=============================\n\n");
            EMBARC_PRINTF("Number of people:  The number of people"
                          " in area A is:%04d.\r\n", device_A.people_num);
            EMBARC_PRINTF("Number of people:  The number of people"
                          " in area B is:%04d.\r\n", device_B.people_num);
            EMBARC_PRINTF("Number of people:  The number of people"
                          " in area C is:%04d.\r\n", device_C.people_num);


            if (device_A.people_num > device_A.people_num_pre_buf)
            {   EMBARC_PRINTF("\n============================ Crowd status"
                              "  ================================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people"
                              " in area A  has increased. \r\n");
            }
            if (device_A.people_num < device_A.people_num_pre_buf)
            {   EMBARC_PRINTF("\n============================= Crowd status"
                              "  =================================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people"
                              " in area A has decreased.\r\n");
            }
            (device_A.people_num > device_A.people_num_pre_buf) ? (
                flag_buff[0] = 'I') : (flag_buff[0] = 'D');

            device_A.people_num_pre_pre = device_A.people_num_pre;
            device_A.people_num_pre = device_A.people_num_buf;
            device_A.people_num_pre_buf = device_A.people_num;
            device_A.people_num_buf = device_A.people_num;



            if (device_A.people_num > device_A.people_num_pre &&
                    device_A.people_num_pre > device_A.people_num_pre_pre)
            {   EMBARC_PRINTF("\n========================= Crowd status "
                              "estimation =============================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people"
                              " in area A may continue increased.\r\n");
            }
            if (device_A.people_num < device_A.people_num_pre &&
                    device_A.people_num_pre < device_A.people_num_pre_pre)
            {   EMBARC_PRINTF("\n========================= Crowd status "
                              "estimation =============================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people"
                              " in area A may continue decreased.\r\n");
            }
            //    square[i++] = device_A.last_num;
            // sprintf(&data_save[14], "A:%04d\r\n", device_A.people_num);
            EMBARC_PRINTF(data_save);
            total_people_num_pre = total_people_num;
            total_people_num = device_A.people_num +
                               device_B.people_num + device_C.people_num;

            data_storage(buff);
            data_save_send(buff, line);
            print_process(flag_buff);
            board_delay_ms(500, 1);

            EMBARC_PRINTF("\nNumber of people:  The total number of people"
                          " in the current region is: %04d , "
                          " ,at %s,%s,%s, %s:%s:%s,\n",
                          total_people_num, time_day, Month_trans, time_year,
                          time_hour, time_minute, time_second);
            EMBARC_PRINTF("\n======================== Data Analysis Finished"
                          " =============================\n");
        } break;

        case 'B': {
            EMBARC_PRINTF("\n============================= Receive Data"
                          " ================================\n\n");
            EMBARC_PRINTF("%s\n", buff);
            EMBARC_PRINTF("\n=========================== Receive Finished"
                          " ==============================\n");
            EMBARC_PRINTF("\n========================= Start Data Analysis"
                          " =============================\n");
            //   int people_num_pre;
            num_get = substring(buff, 13, 4);
            // String type to integer type
            device_B.people_num = atoi(num_get);
            memcpy(data_save, &buff[12], 15);
            EMBARC_PRINTF("\n========================== Number of people"
                          " =============================\n\n");
            EMBARC_PRINTF("Number of people:  The number of people in area"
                          " A is:%04d.\r\n", device_A.people_num);
            EMBARC_PRINTF("Number of people:  The number of people in area"
                          " B is:%04d.\r\n", device_B.people_num);
            EMBARC_PRINTF("Number of people:  The number of people in area"
                          " C is:%04d.\r\n", device_C.people_num);



            if (device_B.people_num > device_B.people_num_pre_buf)
            {   EMBARC_PRINTF("\n========================== Crowd status"
                              "  =============================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area B"
                              "  has increased. \r\n");
            }
            if (device_B.people_num < device_B.people_num_pre_buf)
            {   EMBARC_PRINTF("\n=========================== Crowd status"
                              "  ===============================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area B"
                              " has decreased.\r\n");
            }
            (device_B.people_num > device_B.people_num_pre_buf) ? (
                flag_buff[1] = 'I') : (flag_buff[1] = 'D');

            device_B.people_num_pre_pre = device_B.people_num_pre;
            device_B.people_num_pre = device_B.people_num_buf;
            device_B.people_num_pre_buf = device_B.people_num;
            device_B.people_num_buf = device_B.people_num;



            if (device_B.people_num > device_B.people_num_pre &&
                    device_B.people_num_pre > device_B.people_num_pre_pre)
            {   EMBARC_PRINTF("\n====================== Crowd status estimation"
                              " ==========================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area B"
                              " may continue increased.\r\n");
            }
            if (device_B.people_num < device_B.people_num_pre &&
                    device_B.people_num_pre < device_B.people_num_pre_pre)
            {   EMBARC_PRINTF("\n====================== Crowd status estimation"
                              " ==========================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area B"
                              " may continue decreased.\r\n");
            }

            //    square[i++] = device_A.last_num;
            // sprintf(&data_save[14], "B:%04d\r\n", device_B.people_num);
            EMBARC_PRINTF(data_save);
            total_people_num_pre = total_people_num;
            total_people_num = device_A.people_num +
                               device_B.people_num + device_C.people_num;
            data_storage(buff);
            data_save_send(buff, line);
            print_process(flag_buff);
            board_delay_ms(500, 1);
            EMBARC_PRINTF("\nNumber of people:  The total number of people"
                          " in the current region is: %04d ,"
                          "  ,at %s,%s,%s, %s:%s:%s,\n",
                          total_people_num, time_day, Month_trans,
                          time_year, time_hour, time_minute, time_second);
            EMBARC_PRINTF("\n====================== Data Analysis Finished"
                          " =============================\n");
        } break;

        case 'C': {
            EMBARC_PRINTF("\n============================== Receive Data"
                          " =================================\n\n");
            EMBARC_PRINTF("%s\n", buff);
            EMBARC_PRINTF("\n============================ Receive Finished"
                          " ===============================\n");
            EMBARC_PRINTF("\n========================== Start Data Analysis"
                          " ==============================\n");
            //   int people_num_pre;
            num_get = substring(buff, 13, 4);
            // String type to integer type
            device_C.people_num = atoi(num_get);
            memcpy(data_save, &buff[12], 15);
            EMBARC_PRINTF("\n============================= Number of people ==="
                          "=============================\n\n");
            EMBARC_PRINTF("Number of people:  The number of people in area"
                          " A is:%04d.\r\n", device_A.people_num);
            EMBARC_PRINTF("Number of people:  The number of people in area"
                          " B is:%04d.\r\n", device_B.people_num);
            EMBARC_PRINTF("Number of people:  The number of people in area"
                          " C is:%04d.\r\n", device_C.people_num);

            EMBARC_PRINTF("\n============================== Crowd status"
                          "  ==================================\n\n");
            if (device_C.people_num > device_C.people_num_pre_buf)
            {   EMBARC_PRINTF("\n============================ Crowd status"
                              "  ================================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area C"
                              "  has increased. \r\n");
            }
            if (device_C.people_num < device_C.people_num_pre_buf)
            {   EMBARC_PRINTF("\n============================ Crowd status"
                              "  ================================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area C"
                              " has decreased.\r\n");
            }
            (device_C.people_num > device_C.people_num_pre_buf) ? (
                flag_buff[2] = 'I') : (flag_buff[2] = 'D');

            device_C.people_num_pre_pre = device_C.people_num_pre;
            device_C.people_num_pre = device_C.people_num_buf;
            device_C.people_num_pre_buf = device_C.people_num;
            device_C.people_num_buf = device_C.people_num;



            if (device_C.people_num > device_C.people_num_pre &&
                    device_C.people_num_pre > device_C.people_num_pre_pre)
            {   EMBARC_PRINTF("\n========================= Crowd status"
                              " estimation ===========================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area C"
                              " may continue increased.\r\n");
            }
            if (device_C.people_num < device_C.people_num_pre &&
                    device_C.people_num_pre < device_C.people_num_pre_pre)
            {   EMBARC_PRINTF("\n========================= Crowd status"
                              " estimation ===========================\n\n");
                EMBARC_PRINTF("Crowd status: The number of people in area C"
                              " may continue decreased.\r\n");
            }
            //    square[i++] = device_A.last_num;
            // sprintf(&data_save[14], "C:%04d\r\n", device_C.people_num);
            EMBARC_PRINTF(data_save);
            total_people_num_pre = total_people_num;
            total_people_num = device_A.people_num + device_B.people_num
                               + device_C.people_num;
            data_storage(buff);
            data_save_send(buff, line);
            print_process(flag_buff);
            board_delay_ms(500, 1);
            EMBARC_PRINTF("\nNumber of people:  The total number of people"
                          " in the current region is: %04d ,  ,at %s,%s,%s,"
                          " %s:%s:%s,\n", total_people_num, time_day,
                          Month_trans, time_year,  time_hour, time_minute,
                          time_second);
            EMBARC_PRINTF("\n======================== Data Analysis Finished"
                          " =============================\n");
        } break;

        default: break;
        }
    }
    return total_people_num;
}

// Abnormal Data Processing
int abnormal_data_processing(float var, float ave) {
    if (total_people_num < ave + 5 * var || total_people_num > ave - 5 * var) {
        total_people_num_vaild = total_people_num;
    } else {
        total_people_num_vaild = total_people_num_vaild;
    }

    //  EMBARC_PRINTF("%dtotal_people_num", total_people_num);
    //  EMBARC_PRINTF("%dtotal_people_num_pre", total_people_num_pre);
    // EMBARC_PRINTF("%dtotal_people_num_vaild", total_people_num_vaild);
    (ave - 5 * var < total_people_num < ave + 5 * var) ? (
        vaild_buff[0] = 'T') : (vaild_buff[0] = 'F');
    return total_people_num_vaild;
}

// Data Storage in SD Card
int data_storage(char *buff) {
    FILE *fp = fopen("/sd/data_storage.txt", "a");
    fprintf(fp, "%s\r\n", buff);
    fprintf(fp, "A:%04d,B:%04d,C:%04d,total:%04d\r\n", device_A.people_num,
            device_B.people_num, device_C.people_num, total_people_num);
    fclose(fp);
    EMBARC_PRINTF("\n============================= Stores the data"
                  " =================================\n\n");
    EMBARC_PRINTF(" Data successfully saved!\r\n");
    return E_OK;
}



int data_save_send(char *buff, int line) {
    int time_all_1;
    int time_all_2;
    line++;
    time_get_1 = substring(buff, 17, 9);
    time_get_1 = substring(buff, 27, 5);
    time_all_1 = atoi(time_get_1);
    time_all_2 = atoi(time_get_2);              // String type to integer type

    FILE *fp = fopen("/sd/data_save.txt", "a");
    fprintf(fp, "%4d%9d%5d\r\n", total_people_num, time_all_1, time_all_2);
    fprintf(fp, "%s", time_get_1);
    fprintf(fp, "%s", time_get_2);
    fclose(fp);
    return E_OK;
}
/*int data_read(char *buff, int line) {
    FILE *fp = fopen("/sd/data_save.txt", "r+");
    int size = 20 * line;
    int read_buff[size];
    // printf("\n***Test Begin***\n");
    for (int i = 0; i < size; i++)
    {
        read_buff[i] = fgetc( fp );
        // printf("%c",read_buff[i] );
    }

    fclose(fp);
    return E_OK;

}*/


void print_process(char *buff) {
    // The number of peoplein A,B and C increase.
    if (memcmp(buff, "III", 3) == 0) {
        EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: The crowd is gathering. \n");
        state_III();
    }
    // The number of peoplein A,B and C decrease(DDD).
    if (memcmp(buff, "DDD", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: The crowd is evacuating.  \n");
        state_DDD();
    }
    if (memcmp(buff, "IID", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: Crowds move from area C"
                      " to area A and B.\n");
        state_IID();
    }
    if (memcmp(buff, "IDI", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: Crowds move from area"
                      " B to area A and C \n");
        state_IDI();
    }
    if (memcmp(buff, "DII", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: Crowds move from area"
                      " A to area B and C. \n");
        state_DII();
    }
    if (memcmp(buff, "DDI", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: Crowds move from area"
                      " A and B to area C \n");
        state_DDI();
    }
    if (memcmp(buff, "DID", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: Crowds move from area"
                      " A and C to area B \n");
        state_DID();
    }
    if (memcmp(buff, "IDD", 3) == 0)
    {   EMBARC_PRINTF("\n============================= Flow of crowd"
                      " =================================\n\n");
        EMBARC_PRINTF("Flow of crowd: Crowds move from area"
                      " B and C to area A \n");
        state_IDD();
    }
}



/*int  data_retransmit(char *buff) {
    if (strstr(buff, data_request) != NULL)
    {

    }

    return E_OK;
}
*/

// Converts an integer type to a string type
char *itoa(int num, char *str, int radix) {
    char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;
    int i = 0, j, k;
    if (radix == 10 && num < 0) {
        unum = (unsigned) - num;
        str[i++] = '-';
    } else {
        unum = (unsigned)num;
    }
    do {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';
    if (str[0] == '-') {
        k = 1;
    } else {
        k = 0;
    }
    char temp;
    for (j = k; j <= (i - k - 1) / 2.0; j++) {
        temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
    return str;
}


// Take a piece of data from a string
char* substring(char* ch, int pos, int length) {
    char* pch = ch;
    char* subch = (char*) calloc(sizeof(char), length + 1);
    int i;
    pch = pch + pos;
    for (i = 0; i < length; i++) {
        subch[i] = *(pch++);
    }
    subch[length] = '\0';
    return subch;
}

// String concatenation
/*char *word_strcat(char *first_word, char *mid_word, char *last_word) {
    int length = 38;
    char *send_to_server_mid =
        <char *> malloc(strlen(first_word)
                        + strlen(mid_word));
    strcpy(send_to_server_mid, first_word);
    strcat(send_to_server_mid, mid_word);

    char *send_to_server_all =
        <char *> malloc(strlen(send_to_server_mid)
                        + strlen(last_word));
    strcpy(send_to_server_all, send_to_server_mid);
    strcat(send_to_server_all, last_word);

    send_to_server_all[length] = '\0';
    return send_to_server_all;
    EMBARC_PRINTF("%s\n", send_to_server_all);
}*/
// Create queues to calculate cumulative values
void initQueue(struct Queue * q, int len) {
    q->pBase = (int *)malloc(sizeof(int) * len);
    q->len = len;
    q->front = 0;
    q->rear = 0;
}

bool inQueue(struct Queue * q, int total_people_num) {
    if (isFull(q)) {
        return false;
    }
    int* p = q->pBase + q->rear;
    *p = total_people_num;
    q->rear = (q->rear + 1) % q->len;
    return true;
}

bool outQueue(struct Queue * q) {
    if (isEmpty(q)) {
        return false;
    }

    if (isFull(q) == 0) {
        return false;
    }

    if (isFull(q)) {
        printf("output:%d\n", *(q->pBase + q->front));
        q->front = (q->front + 1) % q->len;
        return true;
    } else {
        return false;
    }
}

bool isEmpty(struct Queue * q) {
    if (q->front == q->rear) {
        printf("fifo empty\n");
        return true;
    }
    return false;
}

bool isFull(struct Queue * q) {
    if ((q->rear + 1) % q->len == q->front) {
        printf("fifo full\n");
        return true;
    }
    return false;
}

void printList(struct Queue * q) {
    int temp = q->front;
    // int sum = 0;
    while ((temp != q->rear) && !isEmpty(q)) {
        printf("%d\n", *(q->pBase + temp));
        temp = (temp + 1) % q->len;
    }
}

void clearQueue(struct Queue * q) {
    while (!isEmpty(q)) {
        q->front = (q->front + 1) % q->len;
    }
}
void average(struct Queue * q, int* sum, float * ave) {
    int temp = q->front;
    *sum = 0;

    while ((temp != q->rear) && !isEmpty(q)) {
        *sum = *sum + *(q->pBase + temp);
        temp = (temp + 1) % q->len;
    }
    *ave = (float)(*sum) / (q->len - 2);
    return;
}
void variance(struct Queue * q, float * var, float ave) {
    int temp = q->front;
    while ((temp != q->rear) && !isEmpty(q)) {
        *var = (float)(*(q->pBase + temp) - ave)
               * (*(q->pBase + temp) - ave) / (q->len - 2);
        temp = (temp + 1) % q->len;
    }
    return;
}

/*int cloud_side_collaborative_rocessing(char *buff,int total_people_num_vaild) {

    //ESP8266_DEFINE(esp8266);

    //char *conn_buf;
    //

    if (strstr(buff, http_get) != NULL)
    {
        int total_people_num_pre;
        char *char_total_people_num[4];
        abnormal_data_processing(int total_people_num_pre, float var,float ave)
        total_people_num_pre=total_people_num;
        if (total_people_num_vaild > 2000)
        {
            EMBARC_PRINTF("\nThere are %d people in the area.\n ", total_people_num_vaild);
            EMBARC_PRINTF("Warning! Too many people in the area! \n \n");

            int length = 58;
            itoa(total_people_num_vaild, char_total_people_num, 10);
            char *send_to_server = (char *) malloc(strlen(send_to_server_first) + strlen(char_total_people_num));
            strcpy(send_to_server, send_to_server_first);
            strcat(send_to_server, char_total_people_num);
            strcat(send_to_server, send_to_server_last);
            strcat(send_to_server, time_hour);
            strcat(send_to_server, colon);
            strcat(send_to_server, time_minute);
            strcat(send_to_server, colon);
            strcat(send_to_server, time_second);
            strcat(send_to_server, full_stop);

            send_to_server[length] = '\0'; //加上字符串结束符。

            EMBARC_PRINTF("\n============================ Data sent to the cloud =============================\n\n");

            conn_buf = strstr(esp8266, http_IDP) + 5;
            *(conn_buf + 1) = 0;
            esp8266_connect_write(esp8266, send_to_server, conn_buf, 50);
            board_delay_ms(50, 1);
            esp8266_connect_write(esp8266, Warning, conn_buf, 40);
            board_delay_ms(50, 1);

            EMBARC_PRINTF("\n================================ Send finished =================================\n\n");

        } else if (total_people_num_vaild > 500 && total_people_num_vaild < 2000 )
        {

            EMBARC_PRINTF("There are %d people in the area. ", total_people_num);

            int length = 58;
            itoa(total_people_num_vaild, char_total_people_num, 10);
            char *send_to_server = (char *) malloc(strlen(send_to_server_first) + strlen(char_total_people_num));
            strcpy(send_to_server, send_to_server_first);
            strcat(send_to_server, char_total_people_num);
            strcat(send_to_server, send_to_server_last);
            strcat(send_to_server, time_hour);
            strcat(send_to_server, colon);
            strcat(send_to_server, time_minute);
            strcat(send_to_server, colon);
            strcat(send_to_server, time_second);
            strcat(send_to_server, full_stop);

            //EMBARC_PRINTF("%s",send_to_server);

            send_to_server[length] = '\0'; //加上字符串结束符。
            EMBARC_PRINTF("\n============================ Data sent to the cloud ============================\n\n");

            conn_buf = strstr(http_server_buf, http_IDP) + 5;
            *(conn_buf + 1) = 0;
            esp8266_connect_write(esp8266, send_to_server, conn_buf, 50);
            board_delay_ms(50, 1);

            EMBARC_PRINTF("\n================================ Send finished =================================\n\n");
        }

    }
    board_delay_ms(50, 1);
}

*/
