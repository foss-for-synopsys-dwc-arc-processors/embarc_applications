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

void panTompkins(void);
#define WINDOWSIZE 15   // Integrator window size, in samples. The article recommends 150ms. So, FS*0.15.
						// However, you should check empirically if the waveform looks ok.
#define NOSAMPLE -32000 // An indicator that there are no more samples to read. Use an impossible value for a sample.
#define FS 100          // Sampling frequency.
#define BUFFSIZE 170    // The size of the buffers (in samples). Must fit more than 1.66 times an RR interval, which
                        // typically could be around 1 second.

#define DELAY 15		// Delay introduced by the filters. Filter only output samples after this one.
						// Set to 0 if you want to keep the delay. Fixing the delay results in DELAY less samples
						// in the final end result.

#include "panTompkins.h"
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
void m_clr(float*, int size);
void lda(void);
int m_init(float ** a,int size);
int recive(float * data,int data_len);
int trans(float * data, int data_len);
int recive_row(int data1,int data2);
void DSPF_dp_mat_mul_cplx(const float* x, int r1, int c1, 
						  const float* y, int c2, float* r);
float identity(void);
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
/////////////////////////UART////////////////////////
DEV_UART *ua;
///////////////////////PD MEAN_TRAN///////////////////
int pd_row,pd_col,mean_row,mean_col;
float  *pd;
float  *pd1;

float *mean;
float *mean1;
int pd_ready=0;
//////////////////////INDENTITY////////////////////
float  *nearest_tran;
float people=0;
float *data_pd;
float *data_pd1;
float *distance1;
float *data_tran;
float *data_tran1;
int list1[5];
float swap[5];
float ans[5];
float peo;
////////////////////HEART BEAT//////////////////
int heart_rate[30];
int heart_swap;
int position[30];
int position_down[30];
int ptt_num;
int ptt_down;
int sum=0;
int beat_num;
float heart=0;
int flag=0;
float avg=0;
//////////////////BLOOD PRESSURE
int ptt_save[20];
int ptt_swap;
					float ptt_sum=0;
					float blood=0;

/////////////////歐式距離/////////////////
float  *one_beat;
float  *o;
float  *av;
float  *wave;
float  *result1;
float  *nearest;
float save;
int   save_list;
/////////////////TRANSMISION STATUS////////////////////
int32_t status=0;
int state=0;
int32_t res1;
int32_t *p1=&res1;//暫存
f32 res;
f32 *p=&res;//暫存
float f ;
char sav[4];
int count=0;
int step=0;
int32_t row,col;
int beat[30];
float rcmd[4];
char  raw_cmd[10];
f32 res2;
f32 *p2=&res2;
int cou=0;
int cou1=0;
int main(void){
	char buffer[10];
	mean=NULL; 
	pd=NULL;
	mean1=NULL;
	pd1=NULL;
	ua = uart_get_dev(0);
	distance1=malloc(sizeof(float)*5*4);
	data_tran=malloc(sizeof(float)*5*52);
	//data_tran1=malloc(sizeof(float)*5*52*2);
	data_pd=malloc(5*4);
	data_pd1=malloc(5*4);
	wave=malloc(sizeof(float)*3000);
	result1=malloc(sizeof(float)*1500);
	nearest=malloc(sizeof(float)*52*5);
	while(ua==0)
	status=ua->uart_open(115200);
	status=ua->uart_control(UART_CMD_SET_BAUD, CONV2VOID(115200));
	while(1){
		switch(step){
			case 0:{
				////////////////////////////////////////////////////////////////系統開始///////////
				status=ua->uart_close();
				status=ua->uart_open(115200);
				cou=0;
				cou1=0;
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
				////////////////////////////////////////////////////////////////ECG,PPG陣列///////////
				if(state==3){
					state=recive(wave,3000);
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
					state=recive(result1,1500);
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
				//panTompkins();
				count=0;
				//////////////////////////////////////////R PEAK POSITION/////////
				for(int q=0;q<1499;q++){
					if(result1[q]==1){
						beat[count]=q;
						count++;
					}
				}
				beat_num=count;
				float wa;
				int ma1;
				for(int q=0;q<beat_num;q++){
					wa=wave[beat[q]-11];
					ma1=beat[q]-11;
					for(int s=0;s<20;s++){
						if(wa<wave[beat[q]-10+s]){
							wa=wave[beat[q]-10+s];
							ma1=beat[q]-10+s;
						}
					}
					beat[q]=ma1;
				}
				///////////////////////////////抓出每一筆心跳///////////////////////
				one_beat=malloc(sizeof(float)*52*beat_num);
				for(int q=0;q<count;q++){
					for(int s=0;s<52;s++){
						one_beat[q*52+s]=wave[beat[q]-18+s];
					}
				}
				////////////////////////////////////////////////////////////////////
				////////////////////////////消除DC//////////////////////////////////
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
				av=malloc(sizeof(float)*52);
				for(int s=0;s<52;s++){
					for(int q=0;q<count;q++){
						av[s]=one_beat[q*52+s]+av[s];
					}
					av[s]=av[s]/count;
				}
				////////////////////////////////////////////////////////////////////////
				//////////////////////////歐式距離//////////////////////////////////////
				float distance[count];
				o=malloc(sizeof(float)*52);
				for(int q=0;q<count;q++){
					for(int s=0;s<52;s++){
						o[s]=pow(one_beat[q*52+s]-av[s],2);
					}
					distance[q]=0;
					for(int s=0;s<52;s++){
						distance[q]=o[s]+distance[q];
					}
				}
				free(o);
				free(av);
				////////////////////////////////////////////////////////////////////////
				//////////////////////////抓出5個歐式距離最近的//////////////////////////
				int list[count];
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
				free(one_beat);
				//////////////////////////////////////////////////////////////////
				//////////////////////////////血壓預測////////////////////////////
				for (int q = 0; q < 1500; q++){
					wave[q+1500]=wave[q+1500]/1000;
				}
				count=0;
				for (int q = 0; q < 1499; q++){
					if(wave[q+1500+1]<wave[q+1500] & wave[q+1500+3]<wave[q+1500] & wave[q+1500+5]<wave[q+1500] & wave[q+1500+7]<wave[q+1500]){
						flag=1;
					}
					if(flag==1){
						if(wave[q+1500+15]<wave[q+1500]){
							position[count]=q;
							q+=40;
							count=count+1;
						}
						flag=0;
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
							if(wave[q+1500+15]>wave[q+1500-1]){
								position_down[count]=q-1;
								count=count+1;
								q+=40;
							}
							flag=0;
						}
					}
				}
				////////////////////////////////////////////////////////////////
				ptt_down=count;
				count=0;
				state=recive(rcmd,1);
				if(state==5){
					for(int q=0;q<1500;q++){
						if(q==position[count]){
							count=count+1;
						}			
					}
					//////////////////////////////////////////////////////Heart rate////////////////////////
					for(int q=0;q<count-1;q++){
						heart_rate[q]=position[q+1]-position[q];
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
					sum=0;
					for (int q = 0; q < count-4; q++){
						sum=heart_rate[q+2]+sum;
					}

					heart=(float)sum;
					heart=heart/(count-4);
					heart=(1/(heart/100))*60;


					float r;
					count=0;
					r=0;
					count=0;
					ptt_sum=0;
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
					ua->uart_write((const void *)(&heart), 4);
					//blood=(-0.3495)*ptt_sum+(0.2041)*heart+165.9724;
					//blood=(-16.5838)*log(ptt_sum)+(11.524)*log(heart)+156.36;
					blood=(-30.3744)*log(ptt_sum)+(23.7657)*log(heart)+166.5005;
					ua->uart_write((const void *)(&blood), 4);
					//blood=(321200)*pow(ptt_sum,-2)+(-89967)*pow(heart,-2)+118.1332;
					//blood=(1038700)*pow(ptt_sum,-2)+(-21567)*pow(heart,-2)+89.0058;
					blood=(328130)*pow(ptt_sum,-2)+(-253474)*pow(heart,-2)+114.4814;
					ua->uart_write((const void *)(&blood), 4);
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
					ptt_sum*=10;
					////////////////////////////////////////////////////////血壓預測//////////////////
					//blood=(7106400)*pow(ptt_sum,-2)+(287000)*pow(heart,-2)+-28.2754;
					//blood=(2310000)*pow(ptt_sum,-2)+(59955)*pow(heart,-2)+40.211;
					blood=(-1548400)*pow(ptt_sum,-2)+(-73399)*pow(heart,-2)+90.378;
					//ua->uart_write((const void *)(&ptt_sum), 4);
					//ua->uart_write((const void *)(&heart), 4);
					ua->uart_write((const void *)(&blood), 4);
					blood=(-28.8452)*log(ptt_sum)+(35.5442)*log(heart)-246.6653;
					ua->uart_write((const void *)(&blood), 4);
					//ua->uart_write((const void *)(&blood), 4);
					if (!pd_ready)
					{
						people=9999;
						float chan;
						for(int q=0;q<261;q++)
						ua->uart_write((const void *)(&people), 4);
						for(int q=0;q<10;q++){
							chan=(float)beat[q];
							ua->uart_write((const void *)(&chan), 4);
						}
						for(int q=0;q<10;q++)
						ua->uart_write((const void *)(&people), 4);
						//ua->uart_write((const void *)(&people), 4);
						step=3;
						break;
					}
					else{
						
						peo=identity();
						//peo=1;
						if(peo==9999)
							step=3;
						else step=0;

						break;

					}
					
				}
			}
			case 3:{
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
				pd_col=col;
				pd_row=row;
				if(pd==NULL){
					pd=malloc(sizeof(float)*pd_row*pd_col);
				}
				else{
					free(pd);
					pd=malloc(sizeof(float)*pd_row*pd_col);
				}
				state=recive(rcmd,1);
				f32 res2;
				f32 *p2=&res2;
				count=0;
				if(state==3){
					for(int q=0;q<pd_col*pd_row*2;q++){
						pd[q]=0;
					}
					while (1) {
						ua->uart_read((void *)&buffer[0], 4);
						dataAssmeble(&p2,buffer);
						pd[count]=res2;
						if(count==(pd_row*pd_col-1)){
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

				/////////////////////
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
				pd_col=col;
				pd_row=row;
				if(pd1==NULL){
					pd1=malloc(sizeof(float)*pd_row*pd_col);
				}
				else{
					free(pd1);
					pd1=malloc(sizeof(float)*pd_row*pd_col);
				}
				state=recive(rcmd,1);
				count=0;
				if(state==3){
					/*for(int q=0;q<pd_col*pd_row;q++){
						pd[q]=0;
					}*/
					while (1) {
						ua->uart_read((void *)&buffer[0], 4);
						dataAssmeble(&p2,buffer);
						pd1[count]=res2;
						if(count==(pd_row*pd_col-1)){
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
				/////////////////////
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
				/////////////////////
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
				if(mean1==NULL){
					mean1=malloc(sizeof(float)*row*col);
				}
				else{
					free(mean1);
					mean1=malloc(sizeof(float)*row*col);
				}
				mean_row=row;
				mean_col=col;
				state=recive(rcmd,1);
				if(state==3){
					while (1) {
						ua->uart_read((void *)&buffer[0], 4);
						dataAssmeble(&p2,buffer);
						mean1[count]=res2;
						if(count==(col*row-1)){
							count=0;
							break;
						}
						else count=count+1;
					}
				}
				state=recive(rcmd,1);
				/////////////////////
				pd_ready=1;
				step=0;
				break;
			}
		}
		
	}
}
float identity(void){
	//distance1=realloc(distance1,sizeof(float)*5*mean_col);
	float *distance1=malloc(sizeof(float)*5*mean_col);
	float m,m1;
	matrix_f32_t near,pdm,pd1m,data_pdm,data_pd1m;
	dsp_mat_init_f32(&near,5,52,(__xy f32_t *)nearest);
	//dsp_mat_init_f32(&nearest_tran,5,52,(__xy f32_t *)data_tran);
	dsp_mat_init_f32(&pdm,52,mean_col,(__xy f32_t *)pd);
	dsp_mat_init_f32(&pd1m,52,mean_col,(__xy f32_t *)pd1);
	//dsp_mat_trans_f32(&near,&nearest_tran);
	data_pd=realloc(data_pd,sizeof(float)*5*mean_col);
	data_pd1=realloc(data_pd1,sizeof(float)*5*mean_col);
	dsp_mat_init_f32(&data_pdm,5,mean_col,(__xy f32_t *)data_pd);
	dsp_mat_init_f32(&data_pd1m,5,mean_col,(__xy f32_t *)data_pd1);
	dsp_mat_mult_f32(&near,&pdm,&data_pdm);
	dsp_mat_mult_f32(&near,&pd1m,&data_pd1m);
	float osa;
	for(int q=0;q<5;q++){
		for(int s=0;s<mean_col;s++){
			distance1[q*mean_col+s]=0;
			for(int w=0;w<mean_col-1;w++){
				m=(data_pd[q*mean_col+w]-mean[s*mean_col+w]);
				m*=m;
				m1=(data_pd1[q*mean_col+w]-mean1[s*mean_col+w]);
				m1*=m1;
				osa=m+m1;
				
				distance1[q*mean_col+s]+=osa;


			}
		}
	}
	for(int q=0;q<5;q++){
		swap[q]=distance1[mean_col*q];
		//ua->uart_write((const void *)(&distance1[q*mean_col]), 4);
	}
	for(int q=0;q<5;q++){
		list1[q]=0;
		for(int s=1;s<mean_col;s++){
			//ua->uart_write((const void *)(&distance1[q*mean_col+s]), 4);
			if(swap[q]>distance1[q*mean_col+s]){
				swap[q]=distance1[q*mean_col+s];
				list1[q]=s;
			}
		}
	}
	for(int q=0;q<5;q++){
		for(int s=0;s<52;s++){
			ua->uart_write((const void *)(&nearest[q*52+s]), 4);
		}
	}
	float chan;
	for(int q=0;q<10;q++){
		chan=(float)beat[q];
		ua->uart_write((const void *)(&chan), 4);
	}
	for(int q=0;q<5;q++)
	{
		ua->uart_write((const void *)(&swap[q]), 4);
		ans[q]=(float)list1[q];
		ua->uart_write((const void *)(&ans[q]), 4);
	}

	ua->uart_write((const void *)(&ans[0]), 4);
	//ua->uart_write((const void *)(&swap[0]), 4);
	return ans[0];
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
	int sta=0;
	for(int i=0;i<data_len;i++){
		ua->uart_read((void *)&buffer[0], 4);
		pByte[0]=buffer[0];
		pByte[1]=buffer[1];
		pByte[2]=buffer[2];
		pByte[3]=buffer[3];
		dataAssmeble(&p,pByte);
		data[i]=res;
		if(buffer[0]=='A' & buffer[1]=='A' & buffer[2]=='X' & buffer[3]=='X'){
			sta=1;
		}
		else if(buffer[0]=='Z' & buffer[1]=='X' & buffer[2]=='X' & buffer[3]=='X'){
			sta=2;
		}
		else if(buffer[0]=='X' & buffer[1]=='X' & buffer[2]=='Z' & buffer[3]=='X'){
			sta=3;
		}
		else if(buffer[0]=='Z' & buffer[1]=='Z' & buffer[2]=='Z' & buffer[3]=='X'){
			sta=4;
		}
		else if(buffer[0]=='X' & buffer[1]=='X' & buffer[2]=='B' & buffer[3]=='B'){
			sta=5;
		}
		else if(buffer[0]=='C' & buffer[1]=='C' & buffer[2]=='X' & buffer[3]=='X'){
			sta=6;
		}
		else if(buffer[0]=='D' & buffer[1]=='D' & buffer[2]=='X' & buffer[3]=='X'){
			sta=7;
		}
	}
	//if(data_len==1){

	//}
	return sta;
}

void m_clr(float* clr,int size){
	for(int clr_size=0;clr_size<size;clr_size++){
		clr[clr_size]=0;
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
dataType input()
{
	int num = NOSAMPLE;
	float ww;
	if (cou!=1500){
		ww=wave[cou]*1000;
		num=(int)ww;
		cou+=1;
	}
	return num;
}

/*
    Use this function to output the information you see fit (last RR-interval,
    sample index which triggered a peak detection, whether each sample was a R
    peak (1) or not (0) etc), in whatever way you see fit (write on screen, write
    on file, blink a LED, call other functions to do other kinds of processing,
    such as feature extraction etc). Change its parameters to receive the necessary
    information to output.
*/
void output(int out)
{
	result1[cou1]=out;
	cou1+=1;
}

/*
    This is the actual QRS-detecting function. It's a loop that constantly calls the input and output functions
    and updates the thresholds and averages until there are no more samples. More details both above and in
    shorter comments below.
*/
void panTompkins()
{
    // The signal array is where the most recent samples are kept. The other arrays are the outputs of each
    // filtering module: DC Block, low pass, high pass, integral etc.
	// The output is a buffer where we can change a previous result (using a back search) before outputting.
	dataType signal[BUFFSIZE], dcblock[BUFFSIZE], lowpass[BUFFSIZE], highpass[BUFFSIZE], derivative[BUFFSIZE], squared[BUFFSIZE], integral[BUFFSIZE], outputSignal[BUFFSIZE];

	// rr1 holds the last 8 RR intervals. rr2 holds the last 8 RR intervals between rrlow and rrhigh.
	// rravg1 is the rr1 average, rr2 is the rravg2. rrlow = 0.92*rravg2, rrhigh = 1.08*rravg2 and rrmiss = 1.16*rravg2.
	// rrlow is the lowest RR-interval considered normal for the current heart beat, while rrhigh is the highest.
	// rrmiss is the longest that it would be expected until a new QRS is detected. If none is detected for such
	// a long interval, the thresholds must be adjusted.
	int rr1[8], rr2[8], rravg1, rravg2, rrlow = 0, rrhigh = 0, rrmiss = 0;

	// i and j are iterators for loops.
	// sample counts how many samples have been read so far.
	// lastQRS stores which was the last sample read when the last R sample was triggered.
	// lastSlope stores the value of the squared slope when the last R sample was triggered.
	// currentSlope helps calculate the max. square slope for the present sample.
	// These are all long unsigned int so that very long signals can be read without messing the count.
	long unsigned int i, j, sample = 0, lastQRS = 0, lastSlope = 0, currentSlope = 0;

	// This variable is used as an index to work with the signal buffers. If the buffers still aren't
	// completely filled, it shows the last filled position. Once the buffers are full, it'll always
	// show the last position, and new samples will make the buffers shift, discarding the oldest
	// sample and storing the newest one on the last position.
	int current;

	// There are the variables from the original Pan-Tompkins algorithm.
	// The ones ending in _i correspond to values from the integrator.
	// The ones ending in _f correspond to values from the DC-block/low-pass/high-pass filtered signal.
	// The peak variables are peak candidates: signal values above the thresholds.
	// The threshold 1 variables are the threshold variables. If a signal sample is higher than this threshold, it's a peak.
	// The threshold 2 variables are half the threshold 1 ones. They're used for a back search when no peak is detected for too long.
	// The spk and npk variables are, respectively, running estimates of signal and noise peaks.
	dataType peak_i = 0, peak_f = 0, threshold_i1 = 0, threshold_i2 = 0, threshold_f1 = 0, threshold_f2 = 0, spk_i = 0, spk_f = 0, npk_i = 0, npk_f = 0;

	// qrs tells whether there was a detection or not.
	// regular tells whether the heart pace is regular or not.
	// prevRegular tells whether the heart beat was regular before the newest RR-interval was calculated.
	bool qrs, regular = true, prevRegular;

	// Initializing the RR averages
	for (i = 0; i < 8; i++)
    {
        rr1[i] = 0;
        rr2[i] = 0;
    }

    // The main loop where everything proposed in the paper happens. Ends when there are no more signal samples.
    do{
        // Test if the buffers are full.
        // If they are, shift them, discarding the oldest sample and adding the new one at the end.
        // Else, just put the newest sample in the next free position.
        // Update 'current' so that the program knows where's the newest sample.
		if (sample >= BUFFSIZE)
		{
			for (i = 0; i < BUFFSIZE - 1; i++)
			{
				signal[i] = signal[i+1];
				dcblock[i] = dcblock[i+1];
				lowpass[i] = lowpass[i+1];
				highpass[i] = highpass[i+1];
				derivative[i] = derivative[i+1];
				squared[i] = squared[i+1];
				integral[i] = integral[i+1];
				outputSignal[i] = outputSignal[i+1];
			}
			current = BUFFSIZE - 1;
		}
		else
		{
			current = sample;
		}
		signal[current] = input();

		// If no sample was read, stop processing!
		if (signal[current] == NOSAMPLE)
			break;
		sample++; // Update sample counter

		// DC Block filter
		// This was not proposed on the original paper.
		// It is not necessary and can be removed if your sensor or database has no DC noise.
		if (current >= 1)
			dcblock[current] = signal[current] - signal[current-1] + 0.995*dcblock[current-1];
		else
			dcblock[current] = 0;

		// Low Pass filter
		// Implemented as proposed by the original paper.
		// y(nT) = 2y(nT - T) - y(nT - 2T) + x(nT) - 2x(nT - 6T) + x(nT - 12T)
		// Can be removed if your signal was previously filtered, or replaced by a different filter.
		lowpass[current] = dcblock[current];
		if (current >= 1)
			lowpass[current] += 2*lowpass[current-1];
		if (current >= 2)
			lowpass[current] -= lowpass[current-2];
		if (current >= 6)
			lowpass[current] -= 2*dcblock[current-6];
		if (current >= 12)
			lowpass[current] += dcblock[current-12];

		// High Pass filter
		// Implemented as proposed by the original paper.
		// y(nT) = 32x(nT - 16T) - [y(nT - T) + x(nT) - x(nT - 32T)]
		// Can be removed if your signal was previously filtered, or replaced by a different filter.
		highpass[current] = -lowpass[current];
		if (current >= 1)
			highpass[current] -= highpass[current-1];
		if (current >= 16)
			highpass[current] += 32*lowpass[current-16];
		if (current >= 32)
			highpass[current] += lowpass[current-32];

		// Derivative filter
		// This is an alternative implementation, the central difference method.
		// f'(a) = [f(a+h) - f(a-h)]/2h
		// The original formula used by Pan-Tompkins was:
		// y(nT) = (1/8T)[-x(nT - 2T) - 2x(nT - T) + 2x(nT + T) + x(nT + 2T)]
        derivative[current] = highpass[current];
		if (current > 0)
			derivative[current] -= highpass[current-1];

		// This just squares the derivative, to get rid of negative values and emphasize high frequencies.
		// y(nT) = [x(nT)]^2.
		squared[current] = derivative[current]*derivative[current];

		// Moving-Window Integration
		// Implemented as proposed by the original paper.
		// y(nT) = (1/N)[x(nT - (N - 1)T) + x(nT - (N - 2)T) + ... x(nT)]
		// WINDOWSIZE, in samples, must be defined so that the window is ~150ms.

		integral[current] = 0;
		for (i = 0; i < WINDOWSIZE; i++)
		{
			if (current >= (dataType)i)
				integral[current] += squared[current - i];
			else
				break;
		}
		integral[current] /= (dataType)i;

		qrs = false;

		// If the current signal is above one of the thresholds (integral or filtered signal), it's a peak candidate.
        if (integral[current] >= threshold_i1 || highpass[current] >= threshold_f1)
        {
            peak_i = integral[current];
            peak_f = highpass[current];
        }

		// If both the integral and the signal are above their thresholds, they're probably signal peaks.
		if ((integral[current] >= threshold_i1) && (highpass[current] >= threshold_f1))
		{
			// There's a 200ms latency. If the new peak respects this condition, we can keep testing.
			if (sample > lastQRS + FS/5)
			{
			    // If it respects the 200ms latency, but it doesn't respect the 360ms latency, we check the slope.
				if (sample <= lastQRS + (long unsigned int)(0.36*FS))
				{
				    // The squared slope is "M" shaped. So we have to check nearby samples to make sure we're really looking
				    // at its peak value, rather than a low one.
				    currentSlope = 0;
				    for (j = current - 10; j <= current; j++)
                        if (squared[j] > currentSlope)
                            currentSlope = squared[j];

				    if (currentSlope <= (dataType)(lastSlope/2))
                    {
                        qrs = false;
                    }

                    else
                    {
                        spk_i = 0.125*peak_i + 0.875*spk_i;
                        threshold_i1 = npk_i + 0.25*(spk_i - npk_i);
                        threshold_i2 = 0.5*threshold_i1;

                        spk_f = 0.125*peak_f + 0.875*spk_f;
                        threshold_f1 = npk_f + 0.25*(spk_f - npk_f);
                        threshold_f2 = 0.5*threshold_f1;

                        lastSlope = currentSlope;
                        qrs = true;
                    }
				}
				// If it was above both thresholds and respects both latency periods, it certainly is a R peak.
				else
				{
				    currentSlope = 0;
                    for (j = current - 10; j <= current; j++)
                        if (squared[j] > currentSlope)
                            currentSlope = squared[j];

                    spk_i = 0.125*peak_i + 0.875*spk_i;
                    threshold_i1 = npk_i + 0.25*(spk_i - npk_i);
                    threshold_i2 = 0.5*threshold_i1;

                    spk_f = 0.125*peak_f + 0.875*spk_f;
                    threshold_f1 = npk_f + 0.25*(spk_f - npk_f);
                    threshold_f2 = 0.5*threshold_f1;

                    lastSlope = currentSlope;
                    qrs = true;
				}
			}
			// If the new peak doesn't respect the 200ms latency, it's noise. Update thresholds and move on to the next sample.
			else
            {
                peak_i = integral[current];
				npk_i = 0.125*peak_i + 0.875*npk_i;
				threshold_i1 = npk_i + 0.25*(spk_i - npk_i);
				threshold_i2 = 0.5*threshold_i1;
				peak_f = highpass[current];
				npk_f = 0.125*peak_f + 0.875*npk_f;
				threshold_f1 = npk_f + 0.25*(spk_f - npk_f);
                threshold_f2 = 0.5*threshold_f1;
                qrs = false;
				outputSignal[current] = qrs;
				if (sample > DELAY + BUFFSIZE)
                	output(outputSignal[0]);
                continue;
            }

		}

		// If a R-peak was detected, the RR-averages must be updated.
		if (qrs)
		{
			// Add the newest RR-interval to the buffer and get the new average.
			rravg1 = 0;
			for (i = 0; i < 7; i++)
			{
				rr1[i] = rr1[i+1];
				rravg1 += rr1[i];
			}
			rr1[7] = sample - lastQRS;
			lastQRS = sample;
			rravg1 += rr1[7];
			rravg1 *= 0.125;

			// If the newly-discovered RR-average is normal, add it to the "normal" buffer and get the new "normal" average.
			// Update the "normal" beat parameters.
			if ( (rr1[7] >= rrlow) && (rr1[7] <= rrhigh) )
			{
				rravg2 = 0;
				for (i = 0; i < 7; i++)
				{
					rr2[i] = rr2[i+1];
					rravg2 += rr2[i];
				}
				rr2[7] = rr1[7];
				rravg2 += rr2[7];
				rravg2 *= 0.125;
				rrlow = 0.92*rravg2;
				rrhigh = 1.16*rravg2;
				rrmiss = 1.66*rravg2;
			}

			prevRegular = regular;
			if (rravg1 == rravg2)
			{
				regular = true;
			}
			// If the beat had been normal but turned odd, change the thresholds.
			else
			{
				regular = false;
				if (prevRegular)
				{
					threshold_i1 /= 2;
					threshold_f1 /= 2;
				}
			}
		}
		// If no R-peak was detected, it's important to check how long it's been since the last detection.
		else
		{
		    // If no R-peak was detected for too long, use the lighter thresholds and do a back search.
			// However, the back search must respect the 200ms limit and the 360ms one (check the slope).
			if ((sample - lastQRS > (long unsigned int)rrmiss) && (sample > lastQRS + FS/5))
			{
				for (i = current - (sample - lastQRS) + FS/5; i < (long unsigned int)current; i++)
				{
					if ( (integral[i] > threshold_i2) && (highpass[i] > threshold_f2))
					{
					    currentSlope = 0;
                        for (j = i - 10; j <= i; j++)
                            if (squared[j] > currentSlope)
                                currentSlope = squared[j];

                        if ((currentSlope < (dataType)(lastSlope/2)) && (i + sample) < lastQRS + 0.36*lastQRS)
                        {
                            qrs = false;
                        }
                        else
                        {
                            peak_i = integral[i];
                            peak_f = highpass[i];
                            spk_i = 0.25*peak_i+ 0.75*spk_i;
                            spk_f = 0.25*peak_f + 0.75*spk_f;
                            threshold_i1 = npk_i + 0.25*(spk_i - npk_i);
                            threshold_i2 = 0.5*threshold_i1;
                            lastSlope = currentSlope;
                            threshold_f1 = npk_f + 0.25*(spk_f - npk_f);
                            threshold_f2 = 0.5*threshold_f1;
                            // If a signal peak was detected on the back search, the RR attributes must be updated.
                            // This is the same thing done when a peak is detected on the first try.
                            //RR Average 1
                            rravg1 = 0;
                            for (j = 0; j < 7; j++)
                            {
                                rr1[j] = rr1[j+1];
                                rravg1 += rr1[j];
                            }
                            rr1[7] = sample - (current - i) - lastQRS;
                            qrs = true;
                            lastQRS = sample - (current - i);
                            rravg1 += rr1[7];
                            rravg1 *= 0.125;

                            //RR Average 2
                            if ( (rr1[7] >= rrlow) && (rr1[7] <= rrhigh) )
                            {
                                rravg2 = 0;
                                for (i = 0; i < 7; i++)
                                {
                                    rr2[i] = rr2[i+1];
                                    rravg2 += rr2[i];
                                }
                                rr2[7] = rr1[7];
                                rravg2 += rr2[7];
                                rravg2 *= 0.125;
                                rrlow = 0.92*rravg2;
                                rrhigh = 1.16*rravg2;
                                rrmiss = 1.66*rravg2;
                            }

                            prevRegular = regular;
                            if (rravg1 == rravg2)
                            {
                                regular = true;
                            }
                            else
                            {
                                regular = false;
                                if (prevRegular)
                                {
                                    threshold_i1 /= 2;
                                    threshold_f1 /= 2;
                                }
                            }

                            break;
                        }
                    }
				}

				if (qrs)
                {
                    outputSignal[current] = false;
                    outputSignal[i] = true;
                    if (sample > DELAY + BUFFSIZE)
                        output(outputSignal[0]);
                    continue;
                }
			}

			// Definitely no signal peak was detected.
			if (!qrs)
			{
				// If some kind of peak had been detected, then it's certainly a noise peak. Thresholds must be updated accordinly.
				if ((integral[current] >= threshold_i1) || (highpass[current] >= threshold_f1))
				{
					peak_i = integral[current];
					npk_i = 0.125*peak_i + 0.875*npk_i;
					threshold_i1 = npk_i + 0.25*(spk_i - npk_i);
					threshold_i2 = 0.5*threshold_i1;
					peak_f = highpass[current];
					npk_f = 0.125*peak_f + 0.875*npk_f;
					threshold_f1 = npk_f + 0.25*(spk_f - npk_f);
					threshold_f2 = 0.5*threshold_f1;
				}
			}
		}
		// The current implementation outputs '0' for every sample where no peak was detected,
		// and '1' for every sample where a peak was detected. It should be changed to fit
		// the desired application.
		// The 'if' accounts for the delay introduced by the filters: we only start outputting after the delay.
		// However, it updates a few samples back from the buffer. The reason is that if we update the detection
		// for the current sample, we might miss a peak that could've been found later by backsearching using
		// lighter thresholds. The final waveform output does match the original signal, though.
		outputSignal[current] = qrs;
		if (sample > DELAY + BUFFSIZE)
			output(outputSignal[0]);
	} while (signal[current] != NOSAMPLE);

	// Output the last remaining samples on the buffer
	for (i = 1; i < BUFFSIZE; i++)
		output(outputSignal[i]);

}

void DSPF_dp_mat_mul_cplx(const float* x, int r1, int c1, 
						  const float* y, int c2, float* r)
{                                                                 
	float real, imag;                                            
	int i, j, k;                                                  
													  
	for(i = 0; i < r1; i++)                                      
	{                                                             
		for(j = 0; j < c2; j++)                                     
		{                                                           
			real=0;                                                   
			imag=0;                                                   
															   
			for(k = 0; k < c1; k++)                                   
			{                                                         
				real += (x[i*2*c1 + 2*k]*y[k*2*c2 + 2*j]                  
				-x[i*2*c1 + 2*k + 1] * y[k*2*c2 + 2*j + 1]);              
																   
				imag+=(x[i*2*c1 + 2*k] * y[k*2*c2 + 2*j + 1]             
				+ x[i*2*c1 + 2*k + 1] * y[k*2*c2 + 2*j]);              
			}                                                         
			r[i*2*c2 + 2*j] = real;                                  
			r[i*2*c2 + 2*j + 1] = imag;  
		}
	}
}
