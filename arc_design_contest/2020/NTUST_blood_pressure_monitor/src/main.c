/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

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
#include "arc.h"
#include "arc_builtin.h"
#include "embARC.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "embARC_debug.h"
#include "math.h"
#include "dsplib.h"
#include <stdlib.h>
#include <stdio.h>
#include "dev_uart.h"
//////////////////////////////////LDA DEFINE//////////////////////////
#define LEN(x) sizeof(x) / sizeof(x[0])



/////////////////////////////////////////////////////////////////////
/////////////////////////////R-PEAK DETECT////////////////////////////
typedef int boolean;
#define true  1
#define false 0
#define M 				1
#define N 				6
#define winSize			10
#define HP_CONSTANT		((float) 1 / (float) M)
typedef unsigned char u8;
typedef float f32;
typedef unsigned long u32;
void detect(float* ecg, float* result, int len);
void m_clr(float*, int size);
void lda(void);
int m_init(float ** a,int size);
int recive(float * data,int data_len);
int trans(float * data, int data_len);
int recive_row(int data1,int data2);
#define NTAPS 16
#define C 1.0/NTAPS
float coeff[NTAPS] = { C, C, C, C, C, C, C, C, C, C, C, C, C, C, C, C };
#define BUFFER_SIZE 2048
////////////////////////////////////////////////////////////////////////
//////////////////////////////////////FLOAT TRANSMIT///////////////////
void dataSplit(f32 data,u8 *buf){	
	for(int i=0;i<4;i++)
		buf[i]=(*((u8 *)(&data)+i));
}

void dataAssmeble(f32 **data , u8 *buf){
	u32 temp=buf[3];
	for(int i=2;i>=0;i--)
		temp=(temp<<8)|buf[i];
	
	f32 *p=(f32*)(&temp);//暫存
	**data=*p; 
}
void dataAssmeblei(int32_t **data , u8 *buf){
	u32 temp=buf[3];
	for(int i=2;i>=0;i--)
		temp=(temp<<8)|buf[i];
	
	int32_t *p=(int32_t*)(&temp);//暫存
	**data=*p; 
}
//////////////////////////////////////////////////////////////////////
DEV_UART *ua;
float *mean;
int32_t status=0;
int pd_ready=0;
float  *pd;
float  *nearest_tran;
float people=0;

int main(void){
	matrix_f32_t near,pdm;
	mean=NULL;
	pd=NULL;
	int pd_row,pd_col,mean_row,mean_col;
	int flag=0;
	fir_f32_t fir;
	float* pState = (float*)malloc(dsp_fir_getsize_f32(NTAPS));
	int i;
	int rec=0;
	int n=0,m=100;
	uint8_t b=0;
	int mal=0;
	f32 res;
	f32 *p=&res;//暫存
	u8 buf[4];
	char buffer[10];
	float f ;
	char sav[4];
	char* pByte = (char*)&f;
	int count=0;
	int step=0;
	int state=0;
	int32_t row,col;
	ua = uart_get_dev(0);
	float  *wave=malloc(sizeof(float)*3000);
	float  *result=malloc(sizeof(float)*1500);
	float  *result1=malloc(sizeof(float)*1500);
	float  *nearest=malloc(sizeof(float)*52*5);
	float  *av=malloc(sizeof(float)*52);
	float  *o=malloc(sizeof(float)*52);
	while(ua==0)
	status=ua->uart_open(115200);
	status=ua->uart_control(UART_CMD_SET_BAUD, CONV2VOID(115200));
	int beat[30];
	float rcmd[10];
	char  raw_cmd[10];
	float tcmd[10];
	dsp_fir_init_f32(&fir, NTAPS, (__xy float*)coeff, (__xy float*)pState);
	while(1){
		switch(step){
			case 0:{
				////////////////////////////////////////////////////////////////系統開始///////////
				state=recive(rcmd,1);
				if(state){
					raw_cmd[0]='X';
					raw_cmd[1]='X';
					raw_cmd[2]='A';
					raw_cmd[3]='A';
					for(int q=0;q<4;q++){
						ua->uart_write((const void *)(&raw_cmd[q]), 1);
								
					}
					step=1;
				}
				else step=0;
				break;
			}
			case 1:{
				////////////////////////////////////////////////////////////////ECG,PPG陣列ROW COL///////////
				int32_t res1;
				int32_t *p1=&res1;//暫存
				state=recive(rcmd,1);
				if(state==2){
					ua->uart_read((void *)&buffer[0], 4);
					dataAssmeblei(&p1,buffer);
					row=res1;
					ua->uart_read((void *)&buffer[0], 4);
					dataAssmeblei(&p1,buffer);
					col=res1;
				}
				else{
					step=0;
					break;
				}
				state=recive(rcmd,1);
				f32 res2;
				f32 *p2=&res2;
				////////////////////////////////////////////////////////////////ECG,PPG陣列///////////
				if(state==3){
					for(int q=0;q<3000;q++){
						wave[q]=0;
					}
					while (1) {
					ua->uart_read((void *)&buffer[0], 4);
					dataAssmeble(&p2,buffer);
					wave[count]=res2;
					if(count==2999){
						count=0;
						break;
					}
					else count=count+1;
					}
				}
				else{
					step=0;
					break;
				}
				state=recive(rcmd,1);
				state=recive(rcmd,1);
				if(state==2){
					ua->uart_read((void *)&buffer[0], 4);
					dataAssmeblei(&p1,buffer);
					row=res1;
					ua->uart_read((void *)&buffer[0], 4);
					dataAssmeblei(&p1,buffer);
					col=res1;
				}
				else{
					step=0;
					break;
				}
				state=recive(rcmd,1);
				if(state==3){
					while (1) {
					ua->uart_read((void *)&buffer[0], 4);
					dataAssmeble(&p2,buffer);
					result1[count]=res2;
					if(count==1499){
						count=0;
						break;
					}
					else count=count+1;
					}
				}
				///////////////////////////////////////////確定傳輸完成//////////////////
				state=recive(rcmd,1);
				if(state==4){
						raw_cmd[0]='B';
						raw_cmd[1]='B';
						raw_cmd[2]='X';
						raw_cmd[3]='X';
							for(int q=0;q<4;q++){
								ua->uart_write((const void *)(&raw_cmd[q]), 1);
										
							}
					}
					else{
						step=0;
						break;
					}
				/////////////////////////////////////////////R PEAK DETECT//////////
				detect(wave,result,1500);
				count=0;
				int beat_num;
				//////////////////////////////////////////R PEAK POSITION/////////
				for(int q=0;q<1499;q++){
					if(result1[q]==1){
						beat[count]=q;
						count++;
					}
				}
				beat_num=count;
				///////////////////////////////抓出每一筆心跳///////////////////////
				float  *one_beat=malloc(sizeof(float)*52*count);
				for(int q=0;q<count;q++){
					for(int s=0;s<52;s++){
						one_beat[q*52+s]=wave[beat[q]-33+s];
					}
				}
				/*free(wave);
				free(result);*/
				////////////////////////////////////////////////////////////////////
				////////////////////////////消除DC//////////////////////////////////
				float avg=0;
				for(int q=0;q<count;q++){
					avg=0;
					for(int s=0;s<52;s++){
						avg=one_beat[q*52+s]+avg;
					}
					avg=avg/52;
					for(int s=0;s<52;s++){
						one_beat[q*52+s]=one_beat[q*52+s]-avg;
					}
				}
				////////////////////////////全部心跳的平均值////////////////////////////
				for(int s=0;s<52;s++){
					for(int q=0;q<count;q++){
						av[s]=one_beat[q*52+s]+av[s];
					}
					av[s]=av[s]/count;
				}
				////////////////////////////////////////////////////////////////////////
				//////////////////////////歐式距離//////////////////////////////////////
				float distance[count];
				for(int q=0;q<count;q++){
					for(int s=0;s<52;s++){
						o[s]=pow(one_beat[q*52+s]-av[s],2);
					}
					distance[q]=0;
					for(int s=0;s<52;s++){
						distance[q]=o[s]+distance[q];
					}
				}
				////////////////////////////////////////////////////////////////////////
				//////////////////////////抓出5個歐式距離最近的//////////////////////////
				int list[count];
				float save;
				int   save_list;
				for(int q=0;q<count;q++){
					list[q]=q;
				}
				for(int q=0;q<count-1;q++){
					for(int s=q+1;s<count;s++){
						if(distance[q]>distance[s]){
							save=distance[q];
							distance[q]=distance[s];
							distance[s]=save;
							save_list=list[q];
							list[q]=list[s];
							list[s]=save_list;
						}
					}
				}
				for(int q=0;q<5;q++){
					for(int s=0;s<52;s++){
						nearest[q*52+s]=one_beat[list[q]*52+s];
					}
				}
				//////////////////////////////////////////////////////////////////
				//////////////////////////////血壓預測////////////////////////////
				for (int q = 0; q < 1500; q++){
					wave[q+1500]=wave[q+1500]/1000;
				}
				int flag=0;
				int position[50];
				int position_down[50];
				count=0;
				int ptt_num;
				int ptt_down;
				for (int q = 0; q < 1499; q++){
					if((wave[q+1500+1]-wave[q+1500])>0 & (wave[q+1500+2]-wave[q+1500])>0 & (wave[q+1500+3]-wave[q+1500])>0 & (wave[q+1500+4]-wave[q+1500])>0){
						flag=1;
					}
					else{
						if(flag==1){
							if(wave[q+1500+30]<wave[q+1500-1]){
								position[count]=q-1;
								count=count+1;
							}
							flag=0;
						}
					}
				}
				ptt_num=count;
				count=0;
				for (int q = 0; q < 1499; q++){
					if((wave[q+1500-3]-wave[q+1500])>0 & (wave[q+1500-5]-wave[q+1500])>0 & (wave[q+1500+3]-wave[q+1500])>0 & (wave[q+1500+5]-wave[q+1500])>0){
						flag=1;
					}
					else{
						if(flag==1){
							if(wave[q+1500+30]>wave[q+1500-1]){
								position_down[count]=q-1;
								count=count+1;
							}
							flag=0;
						}
					}
				}
				////////////////////////////////////////////////////////////////
				ptt_down=count;
				count=0;
				int heart_rate[30];
				int heart_swap;
				int sum=0;
				float heart=0;
				state=recive(rcmd,1);
				if(state==5){
					for(int q=0;q<1500;q++){
						if(q==position_down[count]){
							count=count+1;
						}			
					}
					//////////////////////////////////////////////////////Heart rate////////////////////////
					for(int q=0;q<count-1;q++){
						heart_rate[q]=position_down[q+1]-position_down[q];
					}
					for(int q=0;q<count-2;q++){
						for(int s=q+1;s<count-1;s++){
							if(heart_rate[s]<heart_rate[q]){
								heart_swap=heart_rate[s];
								heart_rate[s]=heart_rate[q];
								heart_rate[q]=heart_swap;
							}
						}
					}
					for (int q = 0; q < count-7; q++){
						sum=heart_rate[q+3]+sum;
					}
					heart=(float)sum;
					heart=heart/(count-7);
					heart=(1/(heart/100))*60;
					ua->uart_write((const void *)(&heart), 4);
					/*for(int q=0;q<1500;q++){
						ua->uart_write((const void *)(&wave[q+1500]), 4);
								
					}*/
					result[0]=0;
					count=0;
					for(int q=0;q<1500;q++){
						if(q==position_down[count]){
							result[0]=1;
							//ua->uart_write((const void *)(&result[0]), 4);
							count=count+1;
						}
						else {
							result[0]=0;
							//	ua->uart_write((const void *)(&result[0]), 4);
						}
								
					}
					/*for(int q=0;q<1500;q++)
					{
						ua->uart_write((const void *)(&wave[q]), 4);
								
					}*/
					result[0]=0;
					/*for(int q=0;q<1500;q++)
					{
						ua->uart_write((const void *)(&result1[q]), 4);
								
					}*/
					/*for(int q=0;q<260;q++)
					{
						ua->uart_write((const void *)(&nearest[q]), 4);
								
					}*/
					int ptt_save[20];
					int ptt_swap;
					float ptt_sum=0;
					float blood=0;
					for(int q=0;q<beat_num;q++){
						for(int s=0;s<ptt_num;s++){
							if(beat[q]<position[s]){
								ptt_save[q]=position[s]-beat[q];
								break;
							}
						}
					}
					for(int q=0;q<beat_num-2;q++){
						for(int s=q+1;s<beat_num-1;s++){
							if(ptt_save[q]>ptt_save[s]){
								ptt_swap=ptt_save[q];
								ptt_save[q]=ptt_save[s];
								ptt_save[s]=ptt_swap;
							}
						}
					}
					for(int q=0;q<beat_num-5;q++){
						ptt_sum+=ptt_save[q+2];
					}
					ptt_sum=ptt_sum/(beat_num-5);
					ptt_sum*=10;
					//ua->uart_write((const void *)(&ptt_sum), 4);
					ptt_sum=0;
					for(int q=0;q<beat_num;q++){
						for(int s=0;s<ptt_down;s++){
							if(beat[q]<position_down[s]){
								ptt_save[q]=position_down[s]-beat[q];
								break;
							}
						}
					}
					for(int q=0;q<beat_num-2;q++){
						for(int s=q+1;s<beat_num-1;s++){
							if(ptt_save[q]>ptt_save[s]){
								ptt_swap=ptt_save[q];
								ptt_save[q]=ptt_save[s];
								ptt_save[s]=ptt_swap;
							}
						}
					}
					for(int q=0;q<beat_num-5;q++){
						ptt_sum+=ptt_save[q+2];
					}
					ptt_sum=ptt_sum/(beat_num-5);
					ptt_sum/=100;
					////////////////////////////////////////////////////////血壓預測//////////////////
					blood=(-113.6678)*ptt_sum+(-0.3128)*heart+191.4082;
					/*ua->uart_write((const void *)(&ptt_sum), 4);
					ua->uart_write((const void *)(&blood), 4);*/
					ua->uart_write((const void *)(&blood), 4);
					if(!pd_ready){
						people=9998;
						ua->uart_write((const void *)(&people), 4);
						state=recive(rcmd,1);
						if(state==6){
							raw_cmd[0]='X';
							raw_cmd[1]='X';
							raw_cmd[2]='C';
							raw_cmd[3]='C';
								for(int q=0;q<4;q++){
									ua->uart_write((const void *)(&raw_cmd[q]), 1);
											
								}
						}
						state=recive(rcmd,1);
						if(state==2){
							ua->uart_read((void *)&buffer[0], 4);
							dataAssmeblei(&p1,buffer);
							row=res1;
							ua->uart_read((void *)&buffer[0], 4);
							dataAssmeblei(&p1,buffer);
							col=res1;
						}
						else{
							step=0;
							break;
						}
						if(pd==NULL){
								pd=malloc(sizeof(float)*row*col);
						}
						else{
							free(pd);
							pd=malloc(sizeof(float)*row*col);
						}
						state=recive(rcmd,1);
						pd_row=row;
						pd_col=col;
						f32 res2;
						f32 *p2=&res2;
						if(state==3){
							for(int q=0;q<row*col;q++){
								pd[q]=0;
							}
							while (1) {
							ua->uart_read((void *)&buffer[0], 4);
							dataAssmeble(&p2,buffer);
							pd[count]=res2;
							if(count==(row*col-1)){
								count=0;
								break;
							}
							else count=count+1;
							}
						}
						else{
							step=0;
							break;
						}
						state=recive(rcmd,1);
						if(state==7){
							raw_cmd[0]='X';
							raw_cmd[1]='X';
							raw_cmd[2]='D';
							raw_cmd[3]='D';
								for(int q=0;q<4;q++){
									ua->uart_write((const void *)(&raw_cmd[q]), 1);
											
								}
						}
						state=recive(rcmd,1);
						if(state==2){
							ua->uart_read((void *)&buffer[0], 4);
							dataAssmeblei(&p1,buffer);
							row=res1;
							ua->uart_read((void *)&buffer[0], 4);
							dataAssmeblei(&p1,buffer);
							col=res1;
						}
						else{
							step=0;
							break;
						}
						if(mean==NULL){
							mean=malloc(sizeof(float)*row*col);
						}
						else{
							free(mean);
							mean=malloc(sizeof(float)*row*col);
						}
						mean_row=row;
						mean_col=col;
						state=recive(rcmd,1);
						if(state==3){
							while (1) {
							ua->uart_read((void *)&buffer[0], 4);
							dataAssmeble(&p2,buffer);
							mean[count]=res2;
							if(count==(col*row-1)){
								count=0;
								break;
							}
							else count=count+1;
							}
						}
						state=recive(rcmd,1);
					}
					else{
						matrix_f32_t near_tr;
						matrix_f32_t data_pdm;
						nearest_tran=malloc(sizeof(float)*5*52);
						float  *data_pd=malloc(sizeof(float)*5*col);
						dsp_mat_init_f32(&near,52,5,(__xy f32_t*)nearest);
						dsp_mat_init_f32(&near_tr,5,52,(__xy f32_t*)nearest_tran);
						dsp_mat_init_f32(&pdm,pd_row,pd_col,(__xy f32_t*)pd);
						dsp_mat_init_f32(&data_pdm,5,pd_col,(__xy f32_t*)data_pd);
						dsp_mat_trans_f32(&near,&near_tr);
						dsp_mat_mult_f32(&near_tr,&pdm,&data_pdm);
						float *mean_save=malloc(sizeof(float)*pd_col);
						float *o1=malloc(sizeof(float)*52);
						m_clr(&mean_save,pd_col);
						for(int s=0;s<col;s++){
							for(int q=0;q<5;q++){
								mean_save[s]=nearest[s+q*52]+mean_save[s];
							}
						}
						float distance1[5];
						for(int q=0;q<mean_row;q++){
							for(int s=0;s<mean_col;s++){
								o1[s]=pow(mean[q*mean_row+s]-mean_save[s],2);
							}
							distance1[q]=0;
							for(int s=0;s<mean_row;s++){
								distance1[q]=o1[s]+distance1[q];
							}
						}
						float save_dis=0;
						int user=0;
						save_dis=distance1[0];
						for(int q=1;q<5;q++){
							if(save_dis>distance1[q]){
								save_dis=distance1[q];
								user=q;
							}
						}
						if(save_dis>4000){
							people=9999;
							ua->uart_write((const void *)(&people), 4);
							state=recive(rcmd,1);
							if(state==6){
								raw_cmd[0]='X';
								raw_cmd[1]='X';
								raw_cmd[2]='C';
								raw_cmd[3]='C';
									for(int q=0;q<4;q++){
										ua->uart_write((const void *)(&raw_cmd[q]), 1);
												
									}
							}
							state=recive(rcmd,1);
							if(state==2){
								ua->uart_read((void *)&buffer[0], 4);
								dataAssmeblei(&p1,buffer);
								row=res1;
								ua->uart_read((void *)&buffer[0], 4);
								dataAssmeblei(&p1,buffer);
								col=res1;
							}
							else{
								step=0;
								break;
							}
							state=recive(rcmd,1);
							f32 res2;
							f32 *p2=&res2;
							if(state==3){
								for(int q=0;q<row*col;q++){
									pd[q]=0;
								}
								while (1) {
								ua->uart_read((void *)&buffer[0], 4);
								dataAssmeble(&p2,buffer);
								pd[count]=res2;
								if(count==(row*col-1)){
									count=0;
									break;
								}
								else count=count+1;
								}
							}
							else{
								step=0;
								break;
							}

							state=recive(rcmd,1);
							if(state==7){
								raw_cmd[0]='X';
								raw_cmd[1]='X';
								raw_cmd[2]='D';
								raw_cmd[3]='D';
									for(int q=0;q<4;q++){
										ua->uart_write((const void *)(&raw_cmd[q]), 1);
												
									}
							}
							state=recive(rcmd,1);
							if(state==2){
								ua->uart_read((void *)&buffer[0], 4);
								dataAssmeblei(&p1,buffer);
								row=res1;
								ua->uart_read((void *)&buffer[0], 4);
								dataAssmeblei(&p1,buffer);
								col=res1;
							}
							else{
								step=0;
								break;
							}
							if(mean==NULL){
								mean=malloc(sizeof(float)*row*col);
							}
							else{
								free(mean);
								mean=malloc(sizeof(float)*row*col);
							}
							
							state=recive(rcmd,1);
							if(state==3){
								while (1) {
								ua->uart_read((void *)&buffer[0], 4);
								dataAssmeble(&p2,buffer);
								mean[count]=res2;
								if(count==(col*row-1)){
									count=0;
									break;
								}
								else count=count+1;
								}
							}
							state=recive(rcmd,1);
						}
						else{
							people=(float)user;
							ua->uart_write((const void *)(&people), 4);
						}

					}
					step=0;
					break;
				}
				/*dsp_fir_f32(&fir,(__xy float*) wave, (__xy float*)result, 2048);
				if(state)
				{
					step=0;
				}
				else 
				{
					trans(result,2048);
					step=0;
				}
				break;*/
			}
		}
		//m_clr(result,2000);
		//detect(wave,result,2000);
		/*for(int q=0;q<2000;q++)
			{
				ua->uart_write((const void *)(&wave[q]), 4);
				
			}*/
		//printf("%d\n",10);
		/*while(1)
		{
			for(uint8_t i = 0; i < 6; i++) {
			smic_adc_read_polling(adc_test, i, &(adc_value[i]));
		}
			printf("%4d\r\n", adc_value[4]);
			board_delay_ms(10,1);

		}*/
		
	}
}
int trans(float * data, int data_len){
	for(int q=0;q<data_len;q++){
		ua->uart_write((const void *)(&data[q]), 4);
				
	}
	return 0;
}
int recive(float * data,int data_len){
	f32 res;
	f32 *p=&res;//暫存
	char buffer[10];
	float f ;
	char* pByte = (char*)&f;
	for(int i=0;i<data_len;i++){
		ua->uart_read((void *)&buffer[0], 4);
		pByte[0]=buffer[0];
		pByte[1]=buffer[1];
		pByte[2]=buffer[2];
		pByte[3]=buffer[3];
		dataAssmeble(&p,pByte);
		data[i]=res;
	}
	if(data_len==1){
		if(buffer[0]=='A' & buffer[1]=='A' & buffer[2]=='X' & buffer[3]=='X'){
			return 1;
		}
		else if(buffer[0]=='Z' & buffer[1]=='X' & buffer[2]=='X' & buffer[3]=='X'){
			return 2;
		}
		else if(buffer[0]=='X' & buffer[1]=='X' & buffer[2]=='Z' & buffer[3]=='X'){
			return 3;
		}
		else if(buffer[0]=='Z' & buffer[1]=='Z' & buffer[2]=='Z' & buffer[3]=='X'){
			return 4;
		}
		else if(buffer[0]=='X' & buffer[1]=='X' & buffer[2]=='B' & buffer[3]=='B'){
			return 5;
		}
		else if(buffer[0]=='C' & buffer[1]=='C' & buffer[2]=='X' & buffer[3]=='X'){
			return 6;
		}
		else if(buffer[0]=='D' & buffer[1]=='D' & buffer[2]=='X' & buffer[3]=='X'){
			return 7;
		}

	}
	return 0;
}
/*int recive_row(int data1,int data2)
{
	char buffer[10];
	int row=0;
	int col=0;
	int res;
	int *p=&res;//暫存
	int res1;
	int *p1=&res1;//暫存
	ua->uart_read((void *)&buffer[0], 4);
	dataAssmeblei(&p,&p1,buffer);
	data1=res;
	data2=res1;
	return res;
}*/
void m_clr(float* clr,int size){
	for(int clr_size=0;clr_size<size;clr_size++){
		clr[clr_size]=0;
	}
}
void detect(float* ecg, float* result, int len) {
	// circular buffer for input ecg signal
	// we need to keep a history of M + 1 samples for HP filter
	float ecg_buff[M + 1] = {0};
	int ecg_buff_WR_idx = 0;
	int ecg_buff_RD_idx = 0;
	
	// circular buffer for input ecg signal
	// we need to keep a history of N+1 samples for LP filter
	float hp_buff[N + 1] = {0};
	int hp_buff_WR_idx = 0;
	int hp_buff_RD_idx = 0;
	
	// LP filter outputs a single point for every input point
	// This goes straight to adaptive filtering for eval
	float next_eval_pt = 0;
	
	// running sums for HP and LP filters, values shifted in FILO
	float hp_sum = 0;
	float lp_sum = 0;
	
	// parameters for adaptive thresholding
	float treshold = 0;
	boolean triggered = false;
	int trig_time = 0;
	float win_max = 0;
	int win_idx = 0;
	
	int i = 0;
	
	for(i = 0; i < len; i++){
		ecg_buff[ecg_buff_WR_idx++] = ecg[i];
		ecg_buff_WR_idx %= (M+1);
		
		//printf("i - %d\n", i);
		
		/* High pass filtering */
		if(i < M){
			// first fill buffer with enough points for HP filter
			hp_sum += ecg_buff[ecg_buff_RD_idx];
			hp_buff[hp_buff_WR_idx] = 0;
			
			//printf("hp_buff[hp_buff_WR_idx] - %f\n", hp_buff[hp_buff_WR_idx]);
		}
		else{
			hp_sum += ecg_buff[ecg_buff_RD_idx];
			
			int tmp = ecg_buff_RD_idx - M;
			if(tmp < 0) tmp += M + 1;
			
			hp_sum -= ecg_buff[tmp];
			
			float y1 = 0;
			float y2 = 0;
			
			tmp = (ecg_buff_RD_idx - ((M+1)/2));
			if(tmp < 0) tmp += M + 1;
			
			y2 = ecg_buff[tmp];
			
			y1 = HP_CONSTANT * hp_sum;
			
			hp_buff[hp_buff_WR_idx] = y2 - y1;
			
			//printf("hp_buff[hp_buff_WR_idx] - %f\n", hp_buff[hp_buff_WR_idx]);
		}
		
		// done reading ECG buffer, increment position
		ecg_buff_RD_idx++;
		ecg_buff_RD_idx %= (M+1);
		
		// done writing to HP buffer, increment position
		hp_buff_WR_idx++;
		hp_buff_WR_idx %= (N+1);
		
		/* Low pass filtering */
		
		// shift in new sample from high pass filter
		lp_sum += hp_buff[hp_buff_RD_idx] * hp_buff[hp_buff_RD_idx];
		
		if(i < N){
			// first fill buffer with enough points for LP filter
			next_eval_pt = 0;
			
		}
		else{
			// shift out oldest data point
			int tmp = hp_buff_RD_idx - N;
			if(tmp < 0) tmp += N+1;
			
			lp_sum -= hp_buff[tmp] * hp_buff[tmp];
			
			next_eval_pt = lp_sum;
		}
		
		// done reading HP buffer, increment position
		hp_buff_RD_idx++;
		hp_buff_RD_idx %= (N+1);
		
		/* Adapative thresholding beat detection */
		// set initial threshold				
		if(i < winSize) {
			if(next_eval_pt > treshold) {
				treshold = next_eval_pt;
			}
		}
		
		// check if detection hold off period has passed
		if(triggered){
			trig_time++;
			
			if(trig_time >= 100){
				triggered = false;
				trig_time = 0;
			}
		}
		
		// find if we have a new max
		if(next_eval_pt > win_max) win_max = next_eval_pt;
		
		// find if we are above adaptive threshold
		if(next_eval_pt > treshold && !triggered) {
			result[i] = 1;
			
			triggered = true;
		}
		else {
			result[i] = 0;
		}
		
		// adjust adaptive threshold using max of signal found 
		// in previous window            
		if(win_idx++ >= winSize){
			// weighting factor for determining the contribution of
			// the current peak value to the threshold adjustment
			float gamma = 0.175;
			
			// forgetting factor - 
			// rate at which we forget old observations
			float alpha = 0.01 + ( ((float) rand() / (float) RAND_MAX) * ((0.1 - 0.01)));
			
			treshold = alpha * gamma * win_max + (1 - alpha) * treshold;
			
			// reset current window ind
			win_idx = 0;
			win_max = -10000;
		}
	}
}
int m_init(float ** a,int size){
    float *t = malloc(size * sizeof(double*)); //r
    if (t==NULL){
        printf("malloc 失敗");
        return -1;
    }
    *a=t;
    return 0;
}
