#include "SYN6288.h"
 
DEV_UART *dev_uart1;

//

int audio_synth_init(){

    dev_uart1 = uart_get_dev(DFSS_UART_1_ID);

    if(dev_uart1 == NULL) return -1;

    // dev_uart1->uart_open(UART_BAUDRATE_9600);

    return 0;
}

void audio_synth(uint8_t *text){
    char     frame_info[50];
    uint8_t  text_len;  
    uint8_t  frame_len;
    uint8_t  ecc  = 0;        
    uint32_t i; 

    text_len = strlen(text);
    frame_len = 5 + text_len + 1;
  
    frame_info[0] = 0xFD;        
    frame_info[1] = 0x00;         
    frame_info[2] = text_len + 3;   
    frame_info[3] = 0x01;            
    frame_info[4] = 0x00;           


      for(i=0; i<text_len; i++){
        frame_info[i+5] = text[i];
      }

      for(i=0; i<frame_len-1; i++){
        ecc = ecc ^ frame_info[i];
      }

    frame_info[frame_len-1] = ecc;

    dev_uart1->uart_write(frame_info, (uint32_t)(frame_len));
        

    delay_ms(2500);
//   EMBARC_PRINTF("play end, next...\n\n");
}

void audio_synth_num(int num){
    char     frame_info[50];
    uint8_t  text_len;  
    uint8_t  frame_len;
    uint8_t  ecc  = 0;        
    uint32_t i; 
    char  text[3];

    Int2String(num,text);

    text_len = 3;
    frame_len = 5 + text_len + 1;
  
    frame_info[0] = 0xFD;        
    frame_info[1] = 0x00;         
    frame_info[2] = text_len + 3;   
    frame_info[3] = 0x01;            
    frame_info[4] = 0x00;           


      for(i=0; i<text_len; i++){
        frame_info[i+5] = text[i];
      }

      for(i=0; i<frame_len-1; i++){
        ecc = ecc ^ frame_info[i];
      }

    frame_info[frame_len-1] = ecc;

    dev_uart1->uart_write(frame_info, (uint32_t)(frame_len));
        

    delay_ms(1500);
}


char* Int2String(int num,char *str)//10进制 
{
    int i = 0;//指示填充str 

    //转换 
    do
    {
        str[i] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0' 
        num /= 10;//去掉最低位    
        i = i+1;
    }while(num);//num不为0继续循环
    
    str[i] = '\0';
    
    //确定开始调整的位置 
    int j = 0;
    
    //对称交换 
    for(;j<i/2;j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b; 
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    } 
    
    return str;//返回转换后的值 
}

