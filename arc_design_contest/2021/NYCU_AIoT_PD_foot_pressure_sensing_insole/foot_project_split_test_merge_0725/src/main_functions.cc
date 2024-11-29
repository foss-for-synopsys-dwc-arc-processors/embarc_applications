/* SPDX-License-Identifier: BSD-3-Clause*/
/*
 *Copyright 2019 The TensorFlow Authors. All Rights Reserved.
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 *==============================================================================
 */

#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "tensorflow/lite/c/common.h"

#include "main_functions.h"
#include "prediction_responder.h"

#include "hx_drv_tflm.h"

#include "model_settings.h"

#include "C:\Users\YJSU\VM\arc_contest\Synopsys_SDK\User_Project/foot_project_split_test_merge_0725/inc/model_r.h"
#include "C:\Users\YJSU\VM\arc_contest\Synopsys_SDK\User_Project/foot_project_split_test_merge_0725/inc/model_l.h"

#include <stdio.h>
#include <stdlib.h>

#include "synopsys_wei_uart.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>


volatile float left_test_data[400];
volatile float right_test_data[400];

/*float left_test_data[400] ;
 *float right_test_data[400] ;
 *
 * Globals, used for compatibility with Arduino-style sketches.
 */
namespace {
	tflite::ErrorReporter * error_reporter_l = nullptr;
	tflite::ErrorReporter * error_reporter_r = nullptr;
	const tflite::Model * model_r = nullptr;
	const tflite::Model * model_l = nullptr;
	tflite::MicroInterpreter * interpreter_l = nullptr;
	tflite::MicroInterpreter * interpreter_r = nullptr;
	volatile TfLiteTensor *input_l = nullptr;
	volatile TfLiteTensor *input_r = nullptr;

/*
 *In order to use optimized tensorflow lite kernels, a signed int8_t quantized
 *model is preferred over the legacy unsigned model format. This means that
 * throughout this project, input images must be converted from unisgned to
 * signed format. The easiest and quickest way to convert from unsigned to
 * signed 8-bit integers is to subtract 128 from the unsigned value to get a
 *  signed value.
 *
 * An area of memory to use for input, output, and intermediate arrays.
 */
	constexpr int kTensorArenaSize = 68 * 1024;
	static uint8_t tensor_arena_l[kTensorArenaSize];

	static uint8_t tensor_arena_r[kTensorArenaSize];
}

#define x_dim 400
uint8_t get_char[8] = { 0 };

/* The name of this function is important for Arduino compatibility. */
void setup()
{
	hx_drv_uart_initial(UART_BR_115200);

/*---------------------------------------------------------------------------------*/

	hx_drv_uart_initial(UART_BR_115200);

	/*
	 *Set up logging. Google style is to avoid globals or statics because of
	 * lifetime uncertainty, but since this has a trivial destructor it's okay.
	 * NOLINTNEXTLINE(runtime-global-variables)
	 */
	static tflite::MicroErrorReporter micro_error_reporter_l;
	error_reporter_l = &micro_error_reporter_l;

	static tflite::MicroErrorReporter micro_error_reporter_r;
	error_reporter_r = &micro_error_reporter_r;

	/*
	 * Map the model into a usable data structure. This doesn't involve any
	 * copying or parsing, it's a very lightweight operation.
	 */
	model_r = tflite::GetModel(right_model_float32_v3_tflite);
	if (model_r->version() != TFLITE_SCHEMA_VERSION) {
		hx_drv_uart_print
		    ("Model provided is schema version %d not equal "
		     "to supported version %d.", model_r->version(),
		     TFLITE_SCHEMA_VERSION);
		TF_LITE_REPORT_ERROR(error_reporter_r,
				     "Model provided is schema version %d not equal "
				     "to supported version %d.",
				     model_r->version(), TFLITE_SCHEMA_VERSION);
		return;
	}

	static tflite::MicroMutableOpResolver < 7 > micro_op_resolver_r;
	micro_op_resolver_r.AddFullyConnected();
	micro_op_resolver_r.AddRelu();
	micro_op_resolver_r.AddSoftmax();
	micro_op_resolver_r.AddMul();
	micro_op_resolver_r.AddAdd();
	micro_op_resolver_r.AddQuantize();
	micro_op_resolver_r.AddDequantize();

	static tflite::MicroInterpreter static_interpreter_r(model_r,
							     micro_op_resolver_r,
							     tensor_arena_r,
							     kTensorArenaSize,
							     error_reporter_r);

	interpreter_r = &static_interpreter_r;

	TfLiteStatus allocate_status_r = interpreter_r->AllocateTensors();

	if (allocate_status_r != kTfLiteOk) {
		hx_drv_uart_print("AllocateTensors() failed");
		TF_LITE_REPORT_ERROR(error_reporter_r,
				     "AllocateTensors() failed");
		return;
	}
/*------------------------------------------------------------------------------*/

	model_l = tflite::GetModel(left_model_float32_v3_tflite);
	if (model_l->version() != TFLITE_SCHEMA_VERSION) {
		hx_drv_uart_print
		    ("Model provided is schema version %d not equal "
		     "to supported version %d.", model_l->version(),
		     TFLITE_SCHEMA_VERSION);
		TF_LITE_REPORT_ERROR(error_reporter_l,
				     "Model provided is schema version %d not equal "
				     "to supported version %d.",
				     model_l->version(), TFLITE_SCHEMA_VERSION);
		return;
	}

	static tflite::MicroMutableOpResolver < 7 > micro_op_resolver_l;
	micro_op_resolver_l.AddFullyConnected();
	micro_op_resolver_l.AddRelu();
	micro_op_resolver_l.AddSoftmax();
	micro_op_resolver_l.AddMul();
	micro_op_resolver_l.AddAdd();
	micro_op_resolver_l.AddQuantize();
	micro_op_resolver_l.AddDequantize();

	static tflite::MicroInterpreter static_interpreter_l(model_l,
							     micro_op_resolver_l,
							     tensor_arena_l,
							     kTensorArenaSize,
							     error_reporter_l);

	interpreter_l = &static_interpreter_l;

	TfLiteStatus allocate_status_l = interpreter_l->AllocateTensors();

	if (allocate_status_l != kTfLiteOk) {
		hx_drv_uart_print("Left_AllocateTensors() failed");
		TF_LITE_REPORT_ERROR(error_reporter_l,
				     "Left_AllocateTensors() failed");
		return;
	}
	/*
	 *Pull in only the operation implementations we need.
	 * This relies on a complete list of all the ops needed by this graph.
	 * An easier approach is to just use the AllOpsResolver, but this will
	 *  incur some penalty in code space for op implementations that are not
	 * needed by this graph.
	 *
	 * tflite::AllOpsResolver resolver;
	 * NOLINTNEXTLINE(runtime-global-variables)
	 *
	 * Build an interpreter to run the model with.
	 * NOLINTNEXTLINE(runtime-global-variables)
	 *
	 *Allocate memory from the tensor_arena for the model's tensors.
	 *
	 * Get information about the memory area to use for the model's input.
	 */
}


void loop()
{

/*--------------------------------------data uart recieve------------------------------------------*/
	bool data_flag = false;

	uint8_t data_buf;
	uint16_t cnt = 0;
	uint32_t data = 0;

	float datainfloat = 0;

	while (1) {
		if (hal_uart_get_char(&data_buf) == HAL_OK)
		{
			datainfloat = data_buf;
			if (data_buf == 97) {
				data = 0;
			} else if (data_buf == 100) {
				hx_drv_uart_print("data_buf = %d\n", data_buf);
			} else if (data_buf == 46) {
			} else if (data_buf == 98) {
				if (cnt < x_dim) {
					right_test_data[cnt] = data / 10;
				} else if ((cnt >= x_dim)
					   && (cnt < (x_dim * 2))) {
					left_test_data[cnt - x_dim] = data / 10;
				} else {
					data_flag = true;
					break;
				}
				cnt++;
			} else if (data_buf == 99) {
				data_flag = true;
				break;
			} else if (data_buf >= 48 && data_buf <= 57) {
				data += data_buf - 48;
				data *= 10;
			} else {
			}
		}
	}

	hx_drv_uart_print("data_flag = %d\n", data_flag);
/*---------------------------------------model prediction-------------------------------------------*/
	uint32_t tick_start, tick_end;
	if (data_flag) {
		input_l = interpreter_l->input(0);
		input_r = interpreter_r->input(0);

		volatile int i;

		for (i = 0; i < 400; i++) {
			input_l->data.f[i] = left_test_data[i] / 1000;
			input_r->data.f[i] = right_test_data[i] / 1000;
		}

		hx_drv_tick_start();
		hx_drv_tick_get(&tick_start);

		TfLiteStatus left_invoke = interpreter_l->Invoke();
		TfLiteStatus right_invoke = interpreter_r->Invoke();

		hx_drv_tick_get(&tick_end);

		/* Run the model on this input and make sure it succeeds.*/
		if (left_invoke != kTfLiteOk) {
			TF_LITE_REPORT_ERROR(error_reporter_l,
					     "Invoke failed.");
		}
		if (right_invoke != kTfLiteOk) {
			TF_LITE_REPORT_ERROR(error_reporter_r,
					     "Invoke failed.");
		}

		TfLiteTensor *output_l = interpreter_l->output(0);
		TfLiteTensor *output_r = interpreter_r->output(0);

		volatile double second_1000, second, second_small;
		second_1000 = (tick_start - tick_end) / 400000;
		second = int (second_1000 / 1000);
		second_small = second_1000 - second * 1000;

		hx_drv_uart_print("Time to run model : %d.%d", second_1000,
				  second_small);

		volatile float co_score_l = output_l->data.f[kCoIndex];
		volatile float pt_score_l = output_l->data.f[kPtIndex];
		volatile float co_score_r = output_r->data.f[kCoIndex];
		volatile float pt_score_r = output_r->data.f[kPtIndex];

		Prediction(pt_score_l, co_score_l, pt_score_r, co_score_r);
		data_flag = false;
	}
}
