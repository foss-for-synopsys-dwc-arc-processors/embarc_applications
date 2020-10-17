#include "LD3320a.h"

extern DEV_IIC * iic_master_0;

const char TEXT01_Buffer[] = {"qie huan kong zhi mo shi"};		// switch control method
const char TEXT02_Buffer[] = {"qie huan bo fang mo shi"};		// switch play mode
const char TEXT03_Buffer[] = {"xia yi qu"};						// next song
const char TEXT04_Buffer[] = {"shang yi qu"};					// prev song
const char TEXT05_Buffer[] = {"bo fang"};						// play
const char TEXT06_Buffer[] = {"zan ting"};						// pause
const char TEXT07_Buffer[] = {"jia da yin liang"};				// vol up
const char TEXT08_Buffer[] = {"jian xiao yin liang"};			// vol down
const char TEXT09_Buffer[] = {"la ji"};							// null
const char TEXT10_Buffer[] = {"sheng yin hen da"};				// null
const char TEXT11_Buffer[] = {"sheng yin hen xiao"};			// null
const char TEXT12_Buffer[] = {"wei shen me"};					// null
const char TEXT13_Buffer[] = {"shen yin"};						// null
const char TEXT14_Buffer[] = {"hen da"};						// null
const char TEXT15_Buffer[] = {"hen xiao"};						// null
const char TEXT16_Buffer[] = {"jiu xing"};						// null
const char TEXT17_Buffer[] = {"zhan shi"};						// null
const char TEXT18_Buffer[] = {"yan shi"};						// null



//Erase all of the words
void AsrErase(void)
{
	uint8_t buf[1];

	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	buf[0] = ASR_WORDS_ERASE_ADDR; //Erase reg addr
	iic_master_0->iic_write(buf, 1);
}

/* 设置识别模式
 * 识别模式设置，值范围1~3
 * 1：循环识别模式。状态灯常亮（默认模式）
 * 2：口令模式，以第一个词条为口令。状态灯常灭，当识别到口令词时常亮，等待识别到新的语音,并且读取识别结果后即灭掉
 * 3：按键模式，按下开始识别，不按不识别。支持掉电保存。状态灯随按键按下而亮起，不按不亮
 */
void AsrSetMode(uint8_t mode)
{
	uint8_t buf[2];

	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	buf[0] = ASR_MODE_ADDR;
	buf[1] = mode;

	iic_master_0->iic_write(buf, 2);
}

 /* 添加词条函数
  * idNum：词条对应的识别号，1~255随意设置。识别到该号码对应的词条语音时，
  *        会将识别号存放到ASR_RESULT_ADDR处，等待主机读取，读取后清0
  * words：要识别汉字词条的拼音，汉字之间用空格隔开
  * 执行该函数，词条是自动往后排队添加的。
  * 支持掉电保存
  */
void AsrAddWords(uint8_t idNum, uint8_t words[], uint8_t len)
{
	uint8_t buf[2+len];
	uint8_t i;

	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	buf[0] = ASR_ADD_WORDS_ADDR;
	buf[1] = idNum; //send words numberID

	for (i = 0; i < len; ++i)
		buf[2+i] = words[i]; //send words data

	iic_master_0->iic_write(buf, 2+len);
}



// Get detection result
uint8_t Ars_Result(void)
{
	uint8_t result[1] = {0};
	uint8_t buf[1] = {0};
	int32_t ercd;

	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	buf[0] = ASR_RESULT_ADDR; // the register address to store detecting result
	iic_master_0->iic_write(buf, 1); /* Master write IIC_slv reg addr to IIC wire */
	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = iic_master_0->iic_read(result, 1); /* Read data from slvdev */

	return result[0];
}



void LD3320A_init(void)
{
	uint8_t buf[1];

	// I2C initialization
	Master0_slvdev_init(ASR_ADDR);

/* the following code will be written to NVM of LD3302A module,
 * so only needs to write once
 */

/*
	if (1) {
		AsrErase();
		delay_ms(60);

		AsrAddWords(1 , (uint8_t *)TEXT01_Buffer,strlen(TEXT01_Buffer));
		delay_ms(60);
		AsrAddWords(2 , (uint8_t *)TEXT02_Buffer,strlen(TEXT02_Buffer));
		delay_ms(60);
		AsrAddWords(3 , (uint8_t *)TEXT03_Buffer,strlen(TEXT03_Buffer));
		delay_ms(60);
		AsrAddWords(4 , (uint8_t *)TEXT04_Buffer,strlen(TEXT04_Buffer));
		delay_ms(60);
		AsrAddWords(5 , (uint8_t *)TEXT05_Buffer,strlen(TEXT05_Buffer));
		delay_ms(60);
		AsrAddWords(6 , (uint8_t *)TEXT06_Buffer,strlen(TEXT06_Buffer));
		delay_ms(60);
		AsrAddWords(7 , (uint8_t *)TEXT07_Buffer,strlen(TEXT07_Buffer));
		delay_ms(60);
		AsrAddWords(8 , (uint8_t *)TEXT08_Buffer,strlen(TEXT08_Buffer));
		delay_ms(60);

		AsrAddWords(9 , (uint8_t *)TEXT09_Buffer, strlen(TEXT09_Buffer));
		delay_ms(60);
		AsrAddWords(10, (uint8_t *)TEXT10_Buffer, strlen(TEXT10_Buffer));
		delay_ms(60);
		AsrAddWords(11, (uint8_t *)TEXT11_Buffer, strlen(TEXT11_Buffer));
		delay_ms(60);
		AsrAddWords(12, (uint8_t *)TEXT12_Buffer, strlen(TEXT12_Buffer));
		delay_ms(60);
		AsrAddWords(13, (uint8_t *)TEXT13_Buffer, strlen(TEXT13_Buffer));
		delay_ms(60);
		AsrAddWords(14, (uint8_t *)TEXT14_Buffer, strlen(TEXT14_Buffer));
		delay_ms(60);
		AsrAddWords(15, (uint8_t *)TEXT15_Buffer, strlen(TEXT15_Buffer));
		delay_ms(60);
		AsrAddWords(16, (uint8_t *)TEXT16_Buffer, strlen(TEXT16_Buffer));
		delay_ms(60);
		AsrAddWords(17, (uint8_t *)TEXT17_Buffer, strlen(TEXT17_Buffer));
		delay_ms(60);
		AsrAddWords(18, (uint8_t *)TEXT18_Buffer, strlen(TEXT18_Buffer));
		// delay_ms(60);

		AsrSetMode(1); //set mode to always running

		delay_ms(60);
	}
*/

}

uint8_t get_audio_cmd(void)
{
	uint8_t result = 0;

	result = Ars_Result(); //reture the detecting result —— the words number ID

	if (result) {
		return result;
	}
}
