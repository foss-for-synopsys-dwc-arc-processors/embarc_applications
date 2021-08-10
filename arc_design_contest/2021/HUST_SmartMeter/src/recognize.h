#ifndef _RECOGNIZE_H_
#define _RECOGNIZE_H_


#include <stdlib.h>
#include <math.h>
#include "embARC.h"
#include <stdio.h>
#include "embARC_debug.h"
#include "models/model.h"
#include "data_provider.h"

#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#define CLASS_COUNT 4

bool is_event(sample_data_node data[], uint32_t winlength, sample_data_node_ptr frontdata);
uint16_t recognize(uint16_t,tflite::MicroInterpreter *interpreter, TfLiteTensor *model_output, tflite::ErrorReporter *error_reporter);

void tflm_ini(const tflite::Model*,
              TfLiteTensor*,
              tflite::MicroInterpreter*, 
              uint8_t *tensor_arena, 
              int kTensorArenaSize, 
              tflite::ErrorReporter*);

void run_model(tflite::MicroInterpreter*, TfLiteTensor* model_output, tflite::ErrorReporter*);

#endif
