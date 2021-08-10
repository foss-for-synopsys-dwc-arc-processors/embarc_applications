#include "data_provider.h"

  const float xmean = 192.0;  //用于归一化的均值，在模型训练前算得
  const float xstd = 168.07;  //用于归一化的标准差
  const float input_scale = 0.0; //模型量化参数
  const int input_zero_point = 0;  //模型量化参数
  const int windows_len = 12;

void get_data(ATT7053_DEF_PTR att7053 ,sample_data_node_ptr getdata){
		getdata->IRMS = Read_Reg(att7053,Current1_Rms_Register);
		getdata->active_power = ((int32_t)Read_Reg(att7053,PowerP1_Register)<<16)>>16;
		getdata->reactive_power = ((int32_t)Read_Reg(att7053,PowerQ1_Register)<<16)>>16;
		getdata->apparent_power = Read_Reg(att7053,PowerS_Register);
}

void data_container_init(sample_data_node data_cont[], uint32_t winlength){
  for(uint32_t i=0;i<(winlength-1);i++)
  {
    data_cont[i].next_ptr = &data_cont[i+1]; 
  }
  data_cont[winlength-1].next_ptr = data_cont;
}

void data_provider(TfLiteTensor* input, sample_data_node_ptr data_buffer, int winlength){
  float *inputdata = input->data.f;
  for(int i=0; i<winlength; i++){
    data_buffer = data_buffer->next_ptr;
    inputdata[i] = (data_buffer->active_power - xmean)/xstd;
  }
}
