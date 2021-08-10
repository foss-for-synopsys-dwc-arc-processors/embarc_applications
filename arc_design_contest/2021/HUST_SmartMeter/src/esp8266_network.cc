#include "esp8266_network.h"

//#define _DEBUG_NET

static uint8_t Check(void);
static uint8_t Init(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen);
static void Restore(void);
static uint8_t ConnectAP(char* ssid,char* pswd);
static uint8_t ConnectServer(char* mode,char* ip,uint16_t port);
static uint8_t DisconnectServer(void);
static uint8_t OpenTransmission(void);
static void CloseTransmission(void);
static void SendString(char* str);
static void SendBuf(char* buf,uint32_t len);


_typdef_net _net=
{
	0,0,
	0,0,
	Check,
	Init,
	Restore,
	ConnectAP,
	ConnectServer,
	DisconnectServer,
	OpenTransmission,
	CloseTransmission,
	SendString,
	SendBuf
};

/**
 * 功能：初始化ESP8266
 * 参数：None
 * 返回值：初始化结果，非0为初始化成功,0为失败
 */
static uint8_t Init(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen){
	_net.rxbuf = prx;_net.rxlen = rxlen;
	_net.txbuf = ptx;_net.txlen = txlen;
	
	memset(_net.rxbuf,0,_net.rxlen);
	memset(_net.txbuf,0,_net.txlen);
	
	_net.CloseTransmission();          //退出透传 
	board_delay_ms(500,1);
	UART_RX_STA=0;
	_net.SendString("AT+RST\r\n");   //重启ESP8266 
	board_delay_ms(800,1);
	UART_RX_STA=0;
	if(_net.Check()==0){              //使用AT指令检查ESP8266是否存在
        	return 0;
	}
	UART_RX_STA=0;
	memset(_net.rxbuf,0,_net.rxlen);    //清空接收缓冲
	_net.SendString("ATE0\r\n");     	//关闭回显 
	board_delay_ms(50,1);
	if(FindStr((char*)_net.rxbuf,"OK",500)==0){  //设置不成功
        	return 0;      
	}
	return 1;                         //设置成功
}

/**
 * 功能：恢复出厂设置
 * 参数：None
 * 返回值：None
 * 说明:此时ESP8266中的用户设置将全部丢失回复成出厂状态
 */
static void Restore(void)
{
	_net.CloseTransmission();           //退出透传
	board_delay_ms(500,1);
	_net.SendString("AT+RESTORE\r\n");//恢复出厂 	 
}

/**
 * 功能：检查ESP8266是否正常
 * 参数：None
 * 返回值：ESP8266返回状态
 *        非0 ESP8266正常
 *        0 ESP8266有问题  
 */
static uint8_t Check(void){
	uint8_t check_cnt=5;
	while(check_cnt--)
	{
		memset(_net.rxbuf,0,_net.rxlen); 	 //清空接收缓冲
		UART_RX_STA=0;
		_net.SendString("AT\r\n");     		 //发送AT握手指令	
		board_delay_ms(50,1);
		if(FindStr((char*)_net.rxbuf,"OK",200) != 0)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * 功能：连接热点
 * 参数：
 *         ssid:热点名
 *         pwd:热点密码
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. WIFI名和密码不正确
 *         2. 路由器连接设备太多,未能给ESP8266分配IP
 */
static uint8_t ConnectAP(char* ssid,char* pswd){
	uint8_t cnt=5;
	while(cnt--)
	{
		memset(_net.rxbuf,0,_net.rxlen);     
		_net.SendString("AT+CWMODE_CUR=1\r\n");              //设置为STATION模式	
		if(FindStr((char*)_net.rxbuf,"OK",200) != 0)
		{
			break;
		}             		
	}
	if(cnt == 0) 
		return 0;
	cnt=2;
	while(cnt--)
	{
		memset(_net.txbuf,0,_net.txlen);                            //清空发送缓冲
		memset(_net.rxbuf,0,_net.rxlen);                            //清空接收缓冲
		sprintf((char*)_net.txbuf,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);//连接目标AP
		_net.SendString((char*)_net.txbuf);	
		if(FindStr((char*)_net.rxbuf,"OK",8000)!=0)                      //连接成功且分配到IP
		{
			return 1;
		}
	}
	return 0;
}

/**
 * 功能：使用指定协议(TCP/UDP)连接到服务器
 * 参数：
 *         mode:协议类型 "TCP","UDP"
 *         ip:目标服务器IP
 *         port:目标是服务器端口号
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. 远程服务器IP和端口号有误
 *         2. 未连接AP
 *         3. 服务器端禁止添加(一般不会发生)
 */
static uint8_t ConnectServer(char* mode,char* ip,uint16_t port){
	uint8_t cnt;
   
    _net.CloseTransmission();                   //多次连接需退出透传
    board_delay_ms(500,1);

	//连接服务器
	cnt=2;
	while(cnt--)
	{
		UART_RX_STA=0;
		memset(_net.rxbuf,0,_net.rxlen);      
		memset(_net.txbuf,0,_net.txlen);   
		sprintf((char*)_net.txbuf,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
		_net.SendString((char*)_net.txbuf);
		if(FindStr((char*)_net.rxbuf,"CONNECT",8000) !=0 )
		{
			break;
		}
	}
	if(cnt == 0) 
		return 0;
	
	//设置透传模式
	if(_net.OpenTransmission()==0) return 0;
	
	//开启发送状态
	cnt=2;
	while(cnt--)
	{
		memset(_net.rxbuf,0,_net.rxlen); 
		_net.SendString("AT+CIPSEND\r\n");//开始处于透传发送状态
		if(FindStr((char*)_net.rxbuf,">",200)!=0)
		{
			return 1;
		}
	}
	return 0;
}

/**
 * 功能：主动和服务器断开连接
 * 参数：None
 * 返回值：
 *         连接结果,非0断开成功,0断开失败
 */
static uint8_t DisconnectServer(void){
	uint8_t cnt=2;
    _net.CloseTransmission();	//退出透传
    board_delay_ms(500,1);
	while(cnt--)
	{
		memset(_net.rxbuf,0,_net.rxlen);  
		_net.SendString("AT+CIPCLOSE\r\n");//关闭链接

		if(FindStr((char*)_net.rxbuf,"CLOSED",200)!=0)//操作成功,和服务器成功断开
		{
			break;
		}
	}
	if(cnt) return 1;
	return 0;
}

/**
 * 功能：透传模式下的数据发送函数
 * 参数：
 *      buffer:待发送数据
 * 返回值：None
 */
static void SendBuf(char* buf,uint32_t len){
    memset(_net.rxbuf,0,_net.rxlen);	
	Uart_SendBuf(buf,len);
}


/**
 * 功能：透传模式下的数据发送函数
 * 参数：
 *      buffer:待发送数据
 * 返回值：None
 */
static void SendString(char* str){
    memset(_net.rxbuf,0,_net.rxlen);
	Uart_SendString(str);
}

static uint8_t OpenTransmission(void){
	//设置透传模式
	uint8_t cnt=2;
	while(cnt--)
	{
		UART_RX_STA=0;
        memset(_net.rxbuf,0,_net.rxlen);    
        _net.SendString("AT+CIPMODE=1\r\n");  
        if(FindStr((char*)_net.rxbuf,"OK",200)!=0)
        {	
			return 1;
		}			
	}
	return 0;
}
//退出透传
static void CloseTransmission(void){
	_net.SendString("+++"); board_delay_ms(50,1);;
	_net.SendString("+++"); board_delay_ms(50,1);;
}
