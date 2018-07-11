/**
 * net control Core code
 * DDK
 * 20180227
 *
 */

#include "embARC.h"

#define DBG_MORE
#include "embARC_debug.h"

#include "include.h"

#include "esp8266.h"




// #define WIFI_SSID   "\"Andrew_Sun\""
// #define WIFI_PWD    "\"smartcar\""
#define WIFI_SSID   "\"WUT-test\""
#define WIFI_PWD    "\"DDK123456\""


bool flag_net = IN_NET;					//Net Song flag
bool flag_netend = false;

char dllink[500] = { 0 };				//Store Song Download Url
char songpoint[50] = { 0 };				//Store Song Name Download Form Net ,Unnecessart Now
static char http_cmd[500] = {0};		//Structure HTTP Command

static char net_info[102400] = { 0 };	


ESP8266_DEF __ESP8266_A;				//Define ESP8266 Control Struct
ESP8266_DEF_PTR ESP8266_A = &__ESP8266_A;

DEV_UART_PTR uart_obj;					//Pointer to Get UART State in Task


/**
 * \brief       Deal with Receive Respond and extract Song ID,add them into a list
 *
 * \param[in]   jsonstr                Pointer of str need to parse songid
 *
 * \retval      -1                     Process Fail
 *
 * \retval      0                      Process Success
 */
static int get_songid(char *jsonstr)
{

	char *string = (char *)jsonstr;
	char songid[12] = "";

	/* Get Id Here only 20 ,Can Change to Get More Song in one Request  */
	uint8_t i = 20, j = 0;

	while (i--) {
		string = strstr(string, "\"id\":");

		if ( string == NULL ) {
			dbg_printf(DBG_MORE_INFO, "String End\r\n");
			break;
		}

		string += 5;

		for (j = 0 ; (*string) != ',' ; j++, string ++ ) {
			songid[j] = *(string);
		}

		songid[j] = '\0';
		dbg_printf(DBG_MORE_INFO, "\r\n%s\r\n", songid);

		filelist_add(songid, BUFF_SPACE, IN_NET);	//Add Song ID into Song Id List
		songid_cnt ++;
	}

	/* No Song Id Found in a Respond */
	if ( songid_cnt == 0 ) {
		return -1;
	}

	return 0;
}


/**
 * \brief       Deal with Receive Buff and parse Song Information
 *	            Only Download Url use
 *
 * \param[in]   jsonstr                Pointer of str need to parse songid
 *
 * \retval      0                      Parse Success
 */
static int get_songinfo(char *jsonstr)
{
	char *string = (char *)jsonstr;
	char *string_p1;
	char *string_p2 ;

	char queryId[20] = { 0 };		//Store Query Id ,No Use Now
	char artistName[50] = { 0 };	//Store Artist Name ,No Use Now
	char albumName[50] = { 0 };		//Store Album Name ,No Use Now
	char lrcLink[500] = { 0 };		//Store lyrics Download Link in wrong format,No Use Now
	char songLink[500] = { 0 };		//Store Song Download Link in wrong format

	uint16_t i = 0, j = 0;;

	memset(dllink, 0, sizeof(char) * 500);
	memset(songpoint, 0, sizeof(char) * 50);

	/***********queryId************/
	string_p2 = strstr(string, "\"queryId\":");
	string_p1 = string_p2 + 11;
	string_p2 = strstr(string_p1, "\",\"");
	strncpy(queryId, string_p1, (uint8_t)(string_p2 - string_p1));
	dbg_printf(DBG_MORE_INFO, "\r\nqueryId: %s\r\n", queryId);

	/***********songName***********/
	string_p2 = strstr(string, "\"songName\":");
	string_p1 = string_p2 + 12;
	string_p2 = strstr(string_p1, "\",\"");
	strncpy(songpoint, string_p1, (uint8_t)(string_p2 - string_p1));
	dbg_printf(DBG_MORE_INFO, "\r\nsongpoint: %s\r\n", songpoint);

	/***********artistName************/
	string_p2 = strstr(string, "\"artistName\":");
	string_p1 = string_p2 + 14;
	string_p2 = strstr(string_p1, "\",\"");
	strncpy(artistName, string_p1, (uint8_t)(string_p2 - string_p1));
	dbg_printf(DBG_MORE_INFO, "\r\nartistName: %s\r\n", artistName);


	/***********albumName************/
	string_p2 = strstr(string, "\"albumName\":");
	string_p1 = string_p2 + 13;
	string_p2 = strstr(string_p1, "\",\"");
	strncpy(albumName, string_p1, (uint8_t)(string_p2 - string_p1));
	dbg_printf(DBG_MORE_INFO, "\r\nalbumName: %s\r\n", albumName);

	/***********lrcLink*****unformat*******/
	string_p2 = strstr(string, "\"lrcLink\":");
	string_p1 = string_p2 + 11;
	string_p2 = strstr(string_p1, "\",\"");
	strncpy(lrcLink, string_p1, (uint8_t)(string_p2 - string_p1));
	dbg_printf(DBG_MORE_INFO, "\r\nlrcLink: %s\r\n", lrcLink);

	/***********songLink************/
	string_p2 = strstr(string, "\"songLink\":");
	string_p1 = string_p2 + 12;
	string_p2 = strstr(string_p1, "\",\"");
	strncpy(songLink, string_p1, (uint8_t)(string_p2 - string_p1));

	/*Correct Download Link*/
	for ( i = 0 , j = 0; songLink[i] != '\0'; i++ ) {
		if ( songLink[i] != '\\' ) {
			dllink[j] = songLink[i];
			j++;
		}
	}

	dbg_printf(DBG_MORE_INFO, "\r\ndllink: %s\r\n", dllink);

	return 0;
}


/**
 * \brief       Init ESP8266 and Malloc 10MB net buff and Connect to WIfi
 *
 */
int net_init()
{
	dbg_printf(DBG_LESS_INFO, "============================ Init ============================\n");
	// memset( net_buff, 0, sizeof(int8_t) * BUFF_SPACE );

	esp8266_init(ESP8266_A, 3125000);
	at_test(ESP8266_A->p_at);
	_Block_Delay(100);

	dbg_printf(DBG_LESS_INFO, "============================ Set Mode ============================\n");
	esp8266_wifi_mode_get(ESP8266_A, false);
	_Block_Delay(100);

	esp8266_wifi_mode_set(ESP8266_A, 3, false);
	_Block_Delay(100);

	dbg_printf(DBG_LESS_INFO, "============================ Connect WiFi ============================\n");

	if (esp8266_wifi_connect(ESP8266_A, WIFI_SSID, WIFI_PWD, false) != AT_OK) {
		dbg_printf(DBG_LESS_INFO, "WIFI %s connect failed\n", WIFI_SSID);
		_Block_Delay(1000);
		return -1;
	}

	dbg_printf(DBG_LESS_INFO, "WIFI %s connect succeed\n", WIFI_SSID);

	dbg_printf(DBG_LESS_INFO, "============================ Show IP ============================\n");
	esp8266_address_get(ESP8266_A);


	uart_obj = uart_get_dev(ESP8266_UART_ID);	//Get Uart Pointer
	// _Block_Delay(100);
	return 0;
}


/**
 * \brief       Request Song ID or Download link
 *
 * \param[in]   option                 (SONG_ID):0 for SONG ID,
 * 	                                   (SONG_INFO):1 for Download Link
 *
 * \retval      -1                     Request Fail
 *
 * \retval      0                      Request Success
 */
int socket_request(uint8_t option)
{

	uint32_t idlen_int;
	char idlen_char[3] = "";
	DEV_BUFFER Rxintbuf;

	//Initalize net_info as Uart FIFO for Receiving,Disable Now
	DEV_BUFFER_INIT(&Rxintbuf, net_info, sizeof(int8_t) * 102400);

	dbg_printf(DBG_LESS_INFO, "============================ connect socket ============================\n\r");
	esp8266_tcp_connect(ESP8266_A, "180.76.152.222", 80);

	memset(http_cmd, 0, sizeof(char) * 500);
	memset(net_info, 0, sizeof(int8_t) * 102400);

	dbg_printf(DBG_LESS_INFO, "============================ create http command ============================\r\n");

	switch (option) {
		case SONG_ID:
			strcat (http_cmd,
					"GET http://fm.taihe.com/dev/api/?tn=playlist&id=public_tuijian_rege&hashcode=&_=1519727783752 HTTP/1.1\r\nHost: fm.baidu.com\r\nConnection: keep-alive\r\n\r\n");
			break;

		case SONG_INFO:
			strcat (http_cmd,
					"POST http://fm.taihe.com/data/music/songlink HTTP/1.1\r\nHost: fm.baidu.com\r\nConnection: keep-alive\r\nContent-Length: ");
			idlen_int = strlen(Playlist_HEAD->data);
			idlen_int += 8;
			itoa(idlen_int, idlen_char, 10);
			strcat(http_cmd, idlen_char);
			strcat(http_cmd, "\r\n\r\nsongIds=");
			strcat (http_cmd, Playlist_HEAD->data);

			if ( Playlist_HEAD->next != NULL ) {
				filelist_delete();

			} else {
				dbg_printf(DBG_LESS_INFO, "Playlist linkList Empty\r\n");
			}

			break;
	}

	dbg_printf(DBG_MORE_INFO, "\r\n%s\r\n", http_cmd);

	vTaskSuspendAll();
	/*****Enable Passthrough Mode to Deal with +IPD flag Send by ESP8266********/
	esp8266_passthr_start(ESP8266_A);
	_Rtos_Delay(100);
	esp8266_passthr_write( ESP8266_A, http_cmd, strlen(http_cmd) );
	_Rtos_Delay(100);
	/*****Enable net_info as FIFO for receiving********/
	uart_obj->uart_control(UART_CMD_SET_RXINT_BUF, (void *)(&Rxintbuf));
	xTaskResumeAll();

	dbg_printf(DBG_LESS_INFO, "======================== Get all Data Driectly===================\r\n");
	_Rtos_Delay(1000);			//Wait Data to Arrive,only 1 second is enough to parse
	dbg_printf(DBG_MORE_INFO, "%s\r\n", (net_info));

	/*********Receive Complete,Disable Passthrough and Disable Uart FIFO***************/
	esp8266_passthr_end(ESP8266_A);
	_Rtos_Delay(100);
	esp8266_transmission_mode(ESP8266_A, ESP8266_NORMALSEND);
	_Rtos_Delay(100);

	uart_obj->uart_control(UART_CMD_SET_RXINT_BUF, NULL);
	_Rtos_Delay(100);

	/*********Exract Data From Buff******************************/
	switch (option) {
		case SONG_ID :
			if (-1 == get_songid(net_info)) {
				return -1;
			}

			break;

		case SONG_INFO:
			if (-1 == get_songinfo(net_info)) {
				return -1;
			}

			break;
	}

	dbg_printf(DBG_LESS_INFO, "Recv Done.\r\n");
	esp8266_CIPCLOSE(ESP8266_A);

	return 0;
}


/**
 * \brief       Download MP3 using Download link
 *
 */
void download_mp3()
{
	uint32_t http_sum = 0;		//Store the Data size that has been Received
	uint32_t bypass_cnt = 0;	//Get the Newest Data size that has been Received

	uint8_t timeout_cnt = 0;	//Plus 1 if Data Size stop increasing
	uint32_t net_time = 0;		//Get System Tick to caculate Net Speed
	uint32_t net_time_pre = 0;	//Store System Tick to caculate Net Speed

	DEV_BUFFER Rxintbuf;

	//Initalize file_buff as Uart FIFO for Receiving,Disable Now
	DEV_BUFFER_INIT(&Rxintbuf, file_buff, sizeof(int8_t) * BUFF_SPACE);

	dbg_printf(DBG_LESS_INFO, "============================ connect socket ============================\n\r");
	esp8266_tcp_connect(ESP8266_A, "211.91.125.36", 80);

	memset(http_cmd, 0, sizeof(char) * 500);
	memset(file_buff, 0, sizeof(int8_t) * BUFF_SPACE);

	/*****Create HTTP Command directly**********/
	strcat (http_cmd, "GET ");
	strcat (http_cmd, dllink);
	strcat (http_cmd, " HTTP/1.1\r\nHost: zhangmenshiting.qianqian.com\r\nConnection: keep-alive\r\n\r\n");
	dbg_printf(DBG_LESS_INFO, "\r\n%s\r\n", http_cmd);

	/*****Enable passthrough to Deal with +IPD flag********/
	vTaskSuspendAll();
	esp8266_passthr_start(ESP8266_A);
	_Rtos_Delay(100);
	esp8266_passthr_write( ESP8266_A, http_cmd, strlen(http_cmd) );
	_Rtos_Delay(100);
	/*****Enable file_buff as FIFO for receiving********/
	uart_obj->uart_control(UART_CMD_SET_RXINT_BUF, (void *)(&Rxintbuf));
	flag_netend = false;
	xTaskResumeAll();


	net_time_pre = xTaskGetTickCount ();		//Record System to Caculate Net Speed

	while (1) {
		_Rtos_Delay(1000);						//Wait **AT LEAST** 1 second

		net_time = xTaskGetTickCount ();		//Record System to Caculate Net Speed

		//read the buff offset to get Data Size Increasement
		bypass_cnt = uart_obj -> uart_info.rx_buf.ofs;

		if ( http_sum != bypass_cnt  ) {
			/*******Caculate Net Speed and Reflash OLED****************/
			gui_info.network_speed = ( bypass_cnt - http_sum ) * 1000 / 1024 / ( net_time - net_time_pre );
			xEventGroupSetBits( GUI_Ev, BIT_0 );

			dbg_printf(DBG_MORE_INFO, "received : %d KB\r", bypass_cnt / 1024 );
			dbg_printf(DBG_MORE_INFO, "received : %d KB/s\r", gui_info.network_speed);
			http_sum = bypass_cnt;
			timeout_cnt = 0;

			/*****Ensure Net Buff will not Overflow*****************/
			if ( http_sum > 14 * 1024 * 1024 ) {
				dbg_printf(DBG_LESS_INFO, "received More than 14MB! Break！\r\n");
				break;
			}
		} else {
			/********No New Data Received in **AT LEAST** 1 second*****************/
			gui_info.network_speed = -1;
			xEventGroupSetBits( GUI_Ev, BIT_0 );

			timeout_cnt ++;

			dbg_printf(DBG_MORE_INFO, "\r\nTime out\r\n");

			if ( timeout_cnt > 3 ) {
				dbg_printf(DBG_LESS_INFO, "\r\nreceive end , %d B\r\n", bypass_cnt  );
				dbg_printf(DBG_MORE_INFO, "\r\n%s \r\n", file_buff);

				flag_netend = true;
				break;


			}
		}

		net_time_pre = net_time;
	}

	/*********Receive Complete,Disable Passthrough and Disable Uart FIFO***************/
	esp8266_passthr_end(ESP8266_A);
	_Rtos_Delay(100);
	esp8266_transmission_mode(ESP8266_A, ESP8266_NORMALSEND);
	uart_obj->uart_control(UART_CMD_SET_RXINT_BUF, NULL);
	_Rtos_Delay(100);

	if ( http_sum > 1024 ) {
		/*****Net BUff is Big enough,there must be a Song in It ***********/
		flag_net = IN_FILE;
	} else {
		/*****Net BUff is too Small,a 302 or 404 Error may Happen***********/
		dbg_printf(DBG_LESS_INFO, "Receive Fail!\r\n");
	}

	/**********Connect May Close Automatic*********************/
	dbg_printf(DBG_LESS_INFO, "Socket Close.\r\n");
	esp8266_CIPCLOSE(ESP8266_A);

}




