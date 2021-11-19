#include "embARC.h"
#include "embARC_debug.h"
#include "process.h"
#include "memory.h"
#include "input_buffer.h"
#include "settings.h"
#include "stdio.h"
#include <math.h>
#include <stdlib.h>
#include "Weight.h"
#include "Bias.h"
#include "settings.h"

float   lstm_c[lSTM_H_SIZE] = {0.0};
float   lstm_h[lSTM_H_SIZE] = {0.0};
static  float   lstm_out[lSTM_H_SIZE];
static  float*  W_fh = &lstm_hw[lSTM_H_SIZE*lSTM_H_SIZE*0]; 
static  float*  W_ih = &lstm_hw[lSTM_H_SIZE*lSTM_H_SIZE*1]; 
static  float*  W_ch = &lstm_hw[lSTM_H_SIZE*lSTM_H_SIZE*2];
static  float*  W_oh = &lstm_hw[lSTM_H_SIZE*lSTM_H_SIZE*3];
static  float*  W_fx = &lstm_iw[lSTM_H_SIZE*lSTM_H_SIZE*0]; 
static  float*  W_ix = &lstm_iw[lSTM_H_SIZE*lSTM_H_SIZE*1]; 
static  float*  W_cx = &lstm_iw[lSTM_H_SIZE*lSTM_H_SIZE*2]; 
static  float*  W_ox = &lstm_iw[lSTM_H_SIZE*lSTM_H_SIZE*3]; 
        
static  float*  b_fh = &lstm_hb[lSTM_H_SIZE*0];
static  float*  b_fx = &lstm_ib[lSTM_H_SIZE*0];
static  float*  b_ih = &lstm_hb[lSTM_H_SIZE*1];
static  float*  b_ix = &lstm_ib[lSTM_H_SIZE*1];
static  float*  b_ch = &lstm_hb[lSTM_H_SIZE*2];
static  float*  b_cx = &lstm_ib[lSTM_H_SIZE*2];
static  float*  b_oh = &lstm_hb[lSTM_H_SIZE*3];
static  float*  b_ox = &lstm_ib[lSTM_H_SIZE*3];

static char state[CLASS_NUM][30] = 
{
    "stop",
    "running",
    "zuo_you",
    "falling"
};

void SIGMOD(float* in,float* out,int dim)
{
    int index;
	for(index = 0 ; index < dim ; index++){
        out[index] = 1.0 / (1.0 + exp(-1.0 * in[index]));
    }
}

void TANH(float* in,float* out,int dim)
{
    int index;
	for(index = 0 ; index < dim ; index++){
        out[index] = tanh(in[index]);
    }
}

void FC_ADD(float* h,float* x,float* out,int dim)
{
    int index;
	for(index = 0 ; index < dim ; index++){
        out[index] = h[index] + x[index];
    }
}

void FC_MUL(float* h,float* x,float* out,int dim)
{
    int index;
	for(index = 0 ; index < dim ; index++){
        out[index] = h[index] * x[index];
    }
}

void RELU(float* in,int dim)
{
    int index;
	for(index = 0 ; index < dim ; index++){
        if(in[index] < 0.0){
            in[index] = 0.0;
        }
        else{
            in[index] = in[index];
        }  
    }
}

void LSTM_FC(float* x,float* w,float* b,float* y,int input_dim,int output_dim)
{
	int index_inDim,index_outDim;
	for(index_outDim = 0 ; index_outDim < output_dim ; index_outDim++)
	{
        y[index_outDim] = 0.0;
		for(index_inDim = 0 ; index_inDim < input_dim ; index_inDim++)
		{   
			y[index_outDim] = y[index_outDim] + x[index_inDim] * w[index_outDim*input_dim+index_inDim];  	
        }
        y[index_outDim]= y[index_outDim] + b[index_outDim];
	}
}

void lstmlib_run_unit(float* in,float* out)
{
    static  float unit_fx[lSTM_X_SIZE];
    static  float unit_fh[lSTM_X_SIZE];
    static  float unit_f[lSTM_X_SIZE];
    static  float f[lSTM_X_SIZE];
    LSTM_FC(in,W_fx,b_fx,unit_fx,lSTM_X_SIZE,lSTM_H_SIZE);
    LSTM_FC(lstm_h,W_fh,b_fh,unit_fh,lSTM_X_SIZE,lSTM_H_SIZE);
    FC_ADD(unit_fx,unit_fh,unit_f,lSTM_H_SIZE);
    SIGMOD(unit_f,f,lSTM_H_SIZE);

    static  float unit_ix[lSTM_X_SIZE];
    static  float unit_ih[lSTM_X_SIZE];
    static  float unit_i[lSTM_X_SIZE];
    static  float i[lSTM_X_SIZE];
    LSTM_FC(in,W_ix,b_ix,unit_ix,lSTM_X_SIZE,lSTM_H_SIZE);
    LSTM_FC(lstm_h,W_ih,b_ih,unit_ih,lSTM_X_SIZE,lSTM_H_SIZE);
    FC_ADD(unit_ix,unit_ih,unit_i,lSTM_H_SIZE);
    SIGMOD(unit_i,i,lSTM_H_SIZE);
    
    static  float unit_cx[lSTM_X_SIZE];
    static  float unit_ch[lSTM_X_SIZE];
    static  float unit_tilde_c[lSTM_X_SIZE];
    static  float tilde_c[lSTM_X_SIZE];
    LSTM_FC(in,W_cx,b_cx,unit_cx,lSTM_X_SIZE,lSTM_H_SIZE);
    LSTM_FC(lstm_h,W_ch,b_ch,unit_ch,lSTM_X_SIZE,lSTM_H_SIZE); 
    FC_ADD(unit_cx,unit_ch,unit_tilde_c,lSTM_H_SIZE);
    TANH(unit_tilde_c,tilde_c,lSTM_H_SIZE); 

    static  float unit_ox[lSTM_X_SIZE];
    static  float unit_oh[lSTM_X_SIZE];
    static  float unit_o[lSTM_X_SIZE];
    static  float o[lSTM_X_SIZE];
    LSTM_FC(in,W_ox,b_ox,unit_ox,lSTM_X_SIZE,lSTM_H_SIZE);
    LSTM_FC(lstm_h,W_oh,b_oh,unit_oh,lSTM_X_SIZE,lSTM_H_SIZE); 
    FC_ADD(unit_ox,unit_oh,unit_o,lSTM_H_SIZE);
    SIGMOD(unit_o,o,lSTM_H_SIZE);

    static  float unit_fc[lSTM_X_SIZE];
    static  float unit_ic[lSTM_X_SIZE];
    FC_MUL(f,lstm_c,unit_fc,lSTM_H_SIZE);
    FC_MUL(i,tilde_c,unit_ic,lSTM_H_SIZE);
    FC_ADD(unit_fc,unit_ic,lstm_c,lSTM_H_SIZE);

    static  float unit_h_c[lSTM_X_SIZE];
    TANH(lstm_c,unit_h_c,lSTM_H_SIZE);
    FC_MUL(unit_h_c,o,lstm_h,lSTM_H_SIZE);
    FC_MUL(unit_h_c,o,out,lSTM_H_SIZE);  
}

uint16_t process_data(float *in)
{
    int i;
    int j;
    //FC_Layer1
    float*  fc1_weight = fc1_w;      
    float*  fc1_bais = fc1_b;  
    static float   fc1_out[lSTM_SEQEN*lSTM_H_SIZE];
    fc1_weight[0] = 6.644853685483438632e-02;
    for(i=0;i<lSTM_SEQEN;i++){
        LSTM_FC(&in[i*FC1_IN_SIZE],fc1_weight,fc1_bais,&fc1_out[i*lSTM_H_SIZE],FC1_IN_SIZE,lSTM_H_SIZE); 
        RELU(&fc1_out[i*lSTM_H_SIZE],lSTM_H_SIZE);
    }

    //lSTM_Layer
    for(j=0;j<lSTM_H_SIZE;j++){
        lstm_c[j] = 0.0; 
        lstm_h[j] = 0.0;    
    }
    for(i=0;i<lSTM_SEQEN;i++){
        lstmlib_run_unit(&fc1_out[i*lSTM_H_SIZE],lstm_out);
    }

    //FC_Layer2
    static  float*  fc2_weight = fc2_w;      
    static  float*  fc2_bais = fc2_b;  
    static  float   fc2_out[CLASS_NUM];
    LSTM_FC(lstm_out,fc2_weight,fc2_bais,fc2_out,lSTM_H_SIZE,CLASS_NUM);

    for(j=0;j<CLASS_NUM;j++){
        printf("fc2_out[%d]: =%f,  ",j,fc2_out[j]);    
    }
    printf("\n");
    float temp;
    uint16_t index;
    uint16_t dim;
    temp = fc2_out[0];
    dim = 0;
    for(index=1; index<CLASS_NUM; index++) {
        if(temp < fc2_out[index]){
            dim = index;
            temp = fc2_out[index];
        }
    }
    printf("%s\n",state[dim]);
    return dim;
}



