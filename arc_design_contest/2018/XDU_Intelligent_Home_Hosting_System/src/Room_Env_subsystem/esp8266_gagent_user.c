#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"

#include "board.h"
#include "step_motor.h"
#include "esp8266_gagent_user.h"






/*
函数功能：MCU向WiFi模组回复信息,并做相应控制。
控制部分应依照数据点定义以及机智云WiFi类设备接入协议文档做相应处理
|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
|receive_data   |uint8_t         |in       |uint8_t类型的指针，用来传入WiFi发送给MCU的数据                        |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/

void mcu2wifi_wifi_ctrl_dev(DEV_UART_PTR esp8266_uart,uint8_t *receive_data)
{
	//先回复WiFi模组
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x00,      //包长度
		0x04,      //命令
		0x01,      //包序号
		0x00,0x00,      //flags		
		0x14      //校验和
	};
	uint8_t i;
	send_data[2] = (sizeof(send_data)-0x04)>>8; //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;//计算包长度（命令至校验和），低位
	send_data[5] = sn;	//获取包序号
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//向WiFi模组发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));

    //进行相应控制
    i=0; //循环计数
	//获取attr_flags
	uint8_t attr_flags[ ATTR_FLAGS_BYTES ];
	for(i=0;i<ATTR_FLAGS_BYTES;i++) attr_flags[i] = receive_data[9 + i];
	//获取attr_vals
	uint8_t attr_vals[ATTR_VALS_BYTES];
	for(i=0;i<ATTR_VALS_BYTES;i++)  attr_vals[i]  = receive_data[9+ATTR_FLAGS_BYTES+i];

	//更新数据类型为“可写”的数据点的值,并做出相应的立即操作
	
	if (attr_flags[0] & 0x01)
	{
		Leave_home = attr_vals[0] & 0x01;		
	}
	if(attr_flags[0] & 0x02) //在 Room_Lamp 设置为有效时
	{
		Room_Lamp = (attr_vals[0] & 0x06) / 2;
	}
	if (attr_flags[0] & 0x04)
	{
		Fan_switch = (attr_vals[0] & 0x18) / 8;
	}
}



/*
函数功能：MCU向WiFi模组回复设备当前状态。
|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/

void mcu2wifi_wifi_read_dev(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x05,      //包长度
		0x04,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags
		0x03,         //action;
		0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,//设备状态，具体字节长度由DEV_STATUS_BYTES决定
		0x14      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = sn; //获取包序号
	//设备状态,具体字节长度由DEV_STATUS_BYTES决定
	// Leave_home	
	if(Leave_home) send_data[9] = send_data[9] | 0x01;
	else send_data[9] = send_data[9] & 0xFE;
	//Room_Lamp
	if(Room_Lamp == 0) send_data[9] = send_data[9] & 0xF9;
	if(Room_Lamp == 1) {send_data[9] = send_data[9] & 0xFB; send_data[9] = send_data[9] | 0x02;}
	if(Room_Lamp == 2) {send_data[9] = send_data[9] | 0x04; send_data[9] = send_data[9] & 0xFD;}
	//Fan_switch
	if(Fan_switch == 0) send_data[9] = send_data[9] & 0xE7;
	if(Fan_switch == 1) {send_data[9] = send_data[9] & 0xEF; send_data[9] = send_data[9] | 0x08;}
	if(Fan_switch == 2) {send_data[9] = send_data[9] | 0x10; send_data[9] = send_data[9] & 0xF7;}

	
	// Room_Env_Tepm
	send_data[10] = Room_Env_Tepm;
	// Room_Env_Humidity
	send_data[11] = Room_Env_Humidity;	
	// Room_Smoke
	send_data[12] = Room_Smoke;
	// Room_Light
	send_data[13] = Room_Light;	
	// Room_PM25
	send_data[14] = (uint8_t)(Room_PM25 / 256);
	send_data[15] = (uint8_t)(Room_PM25 % 256);

	// Body_infrared
	if(Body_infrared) send_data[16] = send_data[16] | 0x01;
	else send_data[16] = send_data[16] & 0xFE;

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//向WiFi模组发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}

/*
函数功能：MCU向WiFi模组主动设备当前状态。

注意：关于发送频率。当设备MCU收到WiFi模组控制产生的状态变化,设备MCU应立刻主动上报当前状态,发送频率不受限制。
但如设备的状态的变化是由于用户触发或环境变化所产生的,其发送的频率不能快于6秒每次。建议按需上报，有特殊上报需求请
联系机智云。

注意：设备MCU需要每隔10分钟定期主动上报当前状态。
|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|

*/
void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x05,      //包长度
		0x05,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags
		0x04,         //action;
		0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,//设备状态，具体字节长度由DEV_STATUS_BYTES决定
		0x14      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = ++sn; //获取包序号
	//设备状态,具体字节长度由DEV_STATUS_BYTES决定
	// Leave_home	
	if(Leave_home) send_data[9] = send_data[9] | 0x01;
	else send_data[9] = send_data[9] & 0xFE;
	//Room_Lamp
	if(Room_Lamp == 0) send_data[9] = send_data[9] & 0xF9;
	if(Room_Lamp == 1) {send_data[9] = send_data[9] & 0xFB; send_data[9] = send_data[9] | 0x02;}
	if(Room_Lamp == 2) {send_data[9] = send_data[9] | 0x04; send_data[9] = send_data[9] & 0xFD;}
	//Fan_switch
	if(Fan_switch == 0) send_data[9] = send_data[9] & 0xE7;
	if(Fan_switch == 1) {send_data[9] = send_data[9] & 0xEF; send_data[9] = send_data[9] | 0x08;}
	if(Fan_switch == 2) {send_data[9] = send_data[9] | 0x10; send_data[9] = send_data[9] & 0xF7;}
	
	// Room_Env_Tepm
	send_data[10] = Room_Env_Tepm;
	// Room_Env_Humidity
	send_data[11] = Room_Env_Humidity;	
	// Room_Smoke
	send_data[12] = Room_Smoke;
	// Room_Light
	send_data[13] = Room_Light;	
	// Room_PM25
	send_data[14] = (uint8_t)(Room_PM25 / 256);
	send_data[15] = (uint8_t)(Room_PM25 % 256);

	// Body_infrared
	if(Body_infrared) send_data[16] = send_data[16] | 0x01;
	else send_data[16] = send_data[16] & 0xFE;

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//向WiFi模组发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}