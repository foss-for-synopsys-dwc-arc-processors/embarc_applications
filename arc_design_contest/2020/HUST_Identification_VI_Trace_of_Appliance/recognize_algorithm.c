#include "recognize_algorithm.h"
#include "embARC.h"
#include "embARC_debug.h"

#ifndef MAX
/** @def Common Maximum macro function (two values)*/
#define MAX(A,B) (((A) > (B))? (A): (B))
#endif

/** @struct error measurement metrics for two vectors  */
typedef struct ref_to_pred_output_t {
	float max_abs_err;          /**< maximum absolute error  */
	float pred_vec_length;      /**< Length of predicted vector: SQRT(sum_i(pred[i]^2))  */
	float ref_vec_length;       /**< Length of reference vector: SQRT(sum_i(ref[i]^2))  */
	float noise_vec_length;     /**< Length of reference vector: SQRT(sum_i((ref[i]-pred[i])^2))  */
	float quant_err_vec_length; /**< Length of quantized error vector: SQRT(sum_i((ref[i]-Quantized(ref[i]))^2))  */
	float ref_to_noise_snr;     /**< Signal-to-noise ratio 10*log_10((ref_vec_length+eps)/(noise_vec_length+eps))  [dB]*/
	float noise_to_quant_ratio; /**< Noise-to-quantization_err ratio (noise_vec_length)/(quant_err_vec_length+eps)  */
} ref_to_pred_output;



//========================================================================================
//
// Other internal functions and routines
//
//========================================================================================

//================================================================================
// Transform MLI FX tensor to float array
//=================================================================================
static mli_status mli_hlp_fx_tensor_to_float (const mli_tensor * src, float *pred_data, uint32_t pred_data_size) {
	uint32_t elem_num = mli_hlp_count_elem_num(src, 0);
	if (elem_num > pred_data_size)
		return MLI_STATUS_LENGTH_ERROR;
	if (elem_num == 0)
		return MLI_STATUS_BAD_TENSOR;

	const float scale_val = 1.0f / (float) (1u << (src->el_params.fx.frac_bits));
	if (src->el_type == MLI_EL_FX_16) {
		int16_t *src_arr = src->data;
		for (int idx = 0; idx < elem_num; idx++)
			pred_data[idx] = (float) (scale_val * src_arr[idx]);
	} else {
		int8_t *src_arr = src->data;
		for (int idx = 0; idx < elem_num; idx++)
			pred_data[idx] = (float) (scale_val * src_arr[idx]);
	}
	return MLI_STATUS_OK;
}

//========================================================================================
// Image pre-processing for LeNet
//========================================================================================
static void LeNet_preprocessing(const void * image_, mli_tensor * net_input_) {
	const unsigned char * in = image_;
	d_type * const pred_data = (d_type * const)net_input_->data;

	// Copying data  to input tensor with subtraction of average.
	// Data shift may be required depending on tensor format
	if (net_input_->el_params.fx.frac_bits == 7) {
		for (int idx = 0; idx < IN_POINTS; idx++)
			pred_data[idx] = (d_type)((int)in[idx] - 128);
	} else if (net_input_->el_params.fx.frac_bits > 7) {
		int shift_left = net_input_->el_params.fx.frac_bits - 7;
		for (int idx = 0; idx < IN_POINTS; idx++)
			pred_data[idx] = (d_type)((int)in[idx] - 128) << shift_left;
	} else {
		int shift_right = 7 - net_input_->el_params.fx.frac_bits;
		for (int idx = 0; idx < IN_POINTS; idx++)
			pred_data[idx] = (d_type)((int)in[idx] - 128)  >> shift_right; // w/o rounding
	}
}

//================================================================================
// Measure various error metrics between two float vectors
//=================================================================================
static int measure_err_vfloat(const float * ref_vec, const float * pred_vec, const int len,
			ref_to_pred_output *out) {
	float ref_accum = 0.f;
	float pred_accum = 0.f;
	float noise_accum = 0.f;
	float max_err = -1.f;

	if (len <= 0) {
		return -1;
	}
	for (int i = 0; i < len; i++) {
		ref_accum += ref_vec[i] * ref_vec[i];
		pred_accum += pred_vec[i] * pred_vec[i];
		noise_accum += (ref_vec[i] - pred_vec[i]) * (ref_vec[i] - pred_vec[i]);
		max_err = MAX(fabsf(ref_vec[i] - pred_vec[i]), max_err);
	}

	const float eps = 0.000000000000000001f;
	out->max_abs_err = max_err;
	out->noise_to_quant_ratio = 1.0f;
	out->quant_err_vec_length = 1.0f;
	out->pred_vec_length = sqrtf(pred_accum);
	out->ref_vec_length = sqrtf(ref_accum);
	out->noise_vec_length = sqrtf(noise_accum);
	out->ref_to_noise_snr = 10.f * log10f((ref_accum + eps) / (noise_accum + eps));

	return 0;
}



size_t output_elements;
void model_init(void){
    output_elements= mli_hlp_count_elem_num(lenet_output, 0);
}

uint8_t get_result(float * pred_data){
    // Check result
	if (MLI_STATUS_OK == mli_hlp_fx_tensor_to_float(lenet_output, pred_data, output_elements)) {
		float tmp = pred_data[0];
		float tmp_1=pred_data[1],tmp_2=pred_data[2],tmp_3=pred_data[3],tmp_4=pred_data[4],tmp_5=pred_data[5];
		int max_idx = 0;
		uint8_t _result = 0;
		for(int idx = 1; idx < output_elements; idx++){
			if(tmp < pred_data[idx]){
				tmp = pred_data[idx];
				max_idx = idx;
			}
		}
		switch(max_idx)
		{
			case 0:
				_result = 0;
				break;
			case 1:
				_result = 1;
				break;
			case 2:
				_result = 2;
				break;
			case 3:
				_result = 3;
				break;
			case 4:
				_result = 4;
				break;
			case 5:
				_result = 5;
				break;
			case 6:
				//_result = 7;
				break;
			case 7:
				//_result = 8;
				break;
			case 8:
				//_result = 9;
				break;
			case 9:
				//_result = 10;
				break;
			default: _result = 0xff;
		}
		return _result;
    }
    return 0x55;
}

int recognize_algorithm(void* input,uint8_t* result){
    float * pred_data = malloc(output_elements * sizeof(float));
    if (pred_data == NULL) {
		printf("ERROR: Can't allocate memory for output\n");
		return -1;
	}

    LeNet_preprocessing(input, lenet_input);

    my_net();

    *result = get_result(pred_data);
	if((*result)==0x55){
		EMBARC_PRINTF("error!!!\r\n");
		return -1;
	}
	else{
		switch(*result)
		{
			case 0:
				EMBARC_PRINTF("Fan!\r\n");
				break;
			case 1:
				EMBARC_PRINTF("Compact Fluorescent Lamp!\r\n");
				break;
			case 2:
				EMBARC_PRINTF("Incandescent Light Bulb!\r\n");
				break;
			case 3:
				EMBARC_PRINTF("Hairdryer!\r\n");
				break;
			case 4:
				EMBARC_PRINTF("Laptop!\r\n");
				break;
			default:
				EMBARC_PRINTF("None!!!\r\n");
				break;
		}
	}
    free(pred_data);
    return 1;
}
