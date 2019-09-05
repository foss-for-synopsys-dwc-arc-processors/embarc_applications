#ifndef SEG_H
#define SEG_H
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

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

bool segImage(unsigned char* image, DEV_UART* console_uart);

#endif