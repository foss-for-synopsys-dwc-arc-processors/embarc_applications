#include "recognize.h"

void tflm_ini(const tflite::Model *model,
              TfLiteTensor *model_input,
              tflite::MicroInterpreter *interpreter,
              uint8_t *tensor_arena,
              int kTensorArenaSize,
              tflite::ErrorReporter *error_reporter){
	/*设置日志记录*/
	static tflite::MicroErrorReporter micro_error_reporter;
	error_reporter = &micro_error_reporter;
	/*加载模型*/
	model = ::tflite::GetModel(micro_model);
	if (model->version() != TFLITE_SCHEMA_VERSION) {
    	TF_LITE_REPORT_ERROR(error_reporter,
    	                    "Model provided is schema version %d not equal "
   	                        "to supported version %d.",
    	                    model->version(), TFLITE_SCHEMA_VERSION);
    	return;
	}
	/*实例化操作解析器resolver*/
	static tflite::MicroMutableOpResolver<3> micro_op_resolver(error_reporter);
  	if (micro_op_resolver.AddBuiltin(
       	  tflite::BuiltinOperator_FULLY_CONNECTED,
          tflite::ops::micro::Register_FULLY_CONNECTED()) != kTfLiteOk) {
    	return;
 	}
  	if (micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
          tflite::ops::micro::Register_SOFTMAX()) != kTfLiteOk) {
    	return;
  	}
	if (micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_RESHAPE,
          tflite::ops::micro::Register_RESHAPE()) != kTfLiteOk) {
    	return;
  	}
	/*实例化解释器*/
	static tflite::MicroInterpreter static_interpreter(model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
	interpreter = &static_interpreter;

	/*分布内存*/
	TfLiteStatus allocate_status = interpreter->AllocateTensors();
  	if (allocate_status != kTfLiteOk) {
    	TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    	return;
  	}
	/*验证输入维度*/
	model_input = interpreter->input(0);
	EMBARC_PRINTF(" DIMS_SIZE:%d, data[0]:%d, data[1]:%d, type:%d\r\n", model_input->dims->size, 
				model_input->dims->data[0], model_input->dims->data[1], model_input->type);
	if ((model_input->dims->size != 2) || (model_input->dims->data[0] != 1) ||
    	(model_input->dims->data[1] != 12) || 
   	 	(model_input->type != kTfLiteFloat32)) {
  		error_reporter->Report("Bad input tensor parameters in model");
  		return;
	}
	EMBARC_PRINTF(" INPUT TENSOR DIMS OK! ");
};

void run_model(tflite::MicroInterpreter *interpreter, TfLiteTensor *model_output, tflite::ErrorReporter *error_reporter) {
	/*运行模型*/
	TfLiteStatus invoke_status = interpreter->Invoke();
  	if (invoke_status != kTfLiteOk) {
    	error_reporter->Report("Invoke failed");
    	return;
	}
	model_output = interpreter->output(0);
};

bool is_event(sample_data_node data[], uint32_t winlength, sample_data_node_ptr frontdata){
	/*事件检测算法实现*/
	//判断窗口中心两个点有效电流之差是否过阈值；
	int32_t diff=0;
	for(int i=0;i<winlength/2;i++){
		frontdata = frontdata->next_ptr;
	}
	diff =  frontdata->next_ptr->IRMS - frontdata->IRMS;
	if(abs(diff)>0x1000){
		EMBARC_PRINTF("***Event happened*** \n");
		return true;
	}
	// EMBARC_PRINTF("  no  Event   \r\n");
	return false;
}

uint16_t recognize(uint16_t app_state,tflite::MicroInterpreter *interpreter, TfLiteTensor *model_output, tflite::ErrorReporter *error_reporter){
    run_model(interpreter,model_output,error_reporter);
	uint16_t state = 0;
	uint8_t index = 0;
	float max = 0.0;
	for(int i=0; i < CLASS_COUNT; i++){
		if(model_output->data.f[i]>max){
			max = model_output->data.f[i];
			index = i;
		}
	}
	switch (index)
	{
	case 0:
		state = app_state | (uint16_t)1<<0;
		break;
	case 1:
		state = app_state & ~((uint16_t)1<<0);
		break;
	case 2:
		state = app_state | (uint16_t)1<<1;
		break;
	case 3:
		state = app_state | ~((uint16_t)1<<1);
		break;
	default:
		break;
	}
	return state;
}
