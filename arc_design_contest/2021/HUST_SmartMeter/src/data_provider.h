#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include "embARC.h"
#include "embARC_debug.h"
#include "spi_att7053.h"
#include "tensorflow/lite/c/common.h"

typedef struct _sample_data_node
{
	uint32_t	IRMS;
	int32_t	active_power;
	int32_t	reactive_power;
	uint32_t	apparent_power;
	struct _sample_data_node* next_ptr;
}sample_data_node,*sample_data_node_ptr;


extern void get_data(ATT7053_DEF_PTR att7053, sample_data_node_ptr getdata);
extern void data_container_init(sample_data_node data_cont[], uint32_t winlength);
extern void data_provider(TfLiteTensor* input,sample_data_node_ptr data_buffer, int winlength);
#endif
