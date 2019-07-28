#include "kws.h"
#include "mli_api.h"

int8_t* scratch_pad;
int8_t* col_buffer;
static int8_t *mfcc_buffer;
static MFCC_STR mfcc_str;

int8_t conv1_wt[CONV1_OUT_CH*CONV1_KX*CONV1_KY]=CONV1_WT;
int8_t conv1_bias[CONV1_OUT_CH]=CONV1_BIAS;
int8_t conv2_ds_wt[CONV1_OUT_CH*CONV2_DS_KX*CONV2_DS_KY]=CONV2_DS_WT;
int8_t conv2_ds_bias[CONV1_OUT_CH]=CONV2_DS_BIAS;
int8_t conv2_pw_wt[CONV2_OUT_CH*CONV1_OUT_CH]=CONV2_PW_WT;
int8_t conv2_pw_bias[CONV2_OUT_CH]=CONV2_PW_BIAS;
int8_t conv3_ds_wt[CONV2_OUT_CH*CONV3_DS_KX*CONV3_DS_KY]=CONV3_DS_WT;
int8_t conv3_ds_bias[CONV2_OUT_CH]=CONV3_DS_BIAS;
int8_t conv3_pw_wt[CONV3_OUT_CH*CONV2_OUT_CH]=CONV3_PW_WT;
int8_t conv3_pw_bias[CONV3_OUT_CH]=CONV3_PW_BIAS;
int8_t conv4_ds_wt[CONV3_OUT_CH*CONV4_DS_KX*CONV4_DS_KY]=CONV4_DS_WT;
int8_t conv4_ds_bias[CONV3_OUT_CH]=CONV4_DS_BIAS;
int8_t conv4_pw_wt[CONV4_OUT_CH*CONV3_OUT_CH]=CONV4_PW_WT;
int8_t conv4_pw_bias[CONV4_OUT_CH]=CONV4_PW_BIAS;
int8_t conv5_ds_wt[CONV4_OUT_CH*CONV5_DS_KX*CONV5_DS_KY]=CONV5_DS_WT;
int8_t conv5_ds_bias[CONV4_OUT_CH]=CONV5_DS_BIAS;
int8_t conv5_pw_wt[CONV5_OUT_CH*CONV4_OUT_CH]=CONV5_PW_WT;
int8_t conv5_pw_bias[CONV5_OUT_CH]=CONV5_PW_BIAS;
int8_t final_fc_wt[CONV5_OUT_CH*OUT_DIM]=FINAL_FC_WT;
int8_t final_fc_bias[OUT_DIM]=FINAL_FC_BIAS;

mli_tensor BufferA_tensor,BufferB_tensor;
mli_relu_cfg relu_cfg = { MLI_RELU_GEN };
mli_tensor input_tensor = { NULL, CONV1_IN_X*CONV1_IN_Y, 1, CONV1_IN_Y, CONV1_IN_X, 0, 3, MLI_EL_FX_8, {3} };

mli_tensor conv1_wt_tensor = { conv1_wt, CONV1_OUT_CH*CONV1_KX*CONV1_KY, CONV1_OUT_CH, 1, CONV1_KY, CONV1_KX, 4, MLI_EL_FX_8, {3} };
mli_tensor conv1_bias_tensor = { conv1_bias, CONV1_OUT_CH, CONV1_OUT_CH, 0, 0, 0, 1, MLI_EL_FX_8, {4} };
mli_conv2d_cfg conv1_cfg = { { MLI_RELU_GEN }, CONV1_SX, CONV1_SY, CONV1_PX, CONV1_PX, CONV1_PY, CONV1_PY + 1};

mli_tensor conv2dw_wt_tensor = { conv2_ds_wt, CONV1_OUT_CH*CONV2_DS_KX*CONV2_DS_KY, CONV2_OUT_CH, CONV1_OUT_CH, CONV2_DS_KY, CONV2_DS_KX, 4, MLI_EL_FX_8, {2} };
mli_tensor conv2dw_bias_tensor = { conv2_ds_bias, CONV2_OUT_CH, CONV2_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {3} };
mli_conv2d_cfg conv2dw_cfg = { { MLI_RELU_GEN }, CONV2_DS_SX, CONV2_DS_SY, CONV2_DS_PX, CONV2_DS_PX, CONV2_DS_PY, CONV2_DS_PY};

mli_tensor conv2pw_wt_tensor = { conv2_pw_wt, CONV2_OUT_CH*CONV1_OUT_CH, CONV2_OUT_CH, CONV2_OUT_CH, 1, 1, 4, MLI_EL_FX_8, {5} };
mli_tensor conv2pw_bias_tensor = { conv2_pw_bias, CONV2_OUT_CH, CONV2_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {4} };
mli_conv2d_cfg conv2pw_cfg = { { MLI_RELU_GEN }, 1, 1, 0, 0, 0, 0};

mli_tensor conv3dw_wt_tensor = { conv3_ds_wt, CONV2_OUT_CH*CONV3_DS_KX*CONV3_DS_KY, CONV3_OUT_CH, CONV2_OUT_CH, CONV3_DS_KY, CONV3_DS_KX, 4, MLI_EL_FX_8, {1} };
mli_tensor conv3dw_bias_tensor = { conv3_ds_bias, CONV3_OUT_CH, CONV3_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {2} };
mli_conv2d_cfg conv3dw_cfg = { { MLI_RELU_GEN }, CONV3_DS_SX, CONV3_DS_SY, CONV3_DS_PX, CONV3_DS_PX, CONV3_DS_PY, CONV3_DS_PY};

mli_tensor conv3pw_wt_tensor = { conv3_pw_wt, CONV3_OUT_CH*CONV2_OUT_CH, CONV3_OUT_CH, CONV3_OUT_CH, 1, 1, 4, MLI_EL_FX_8, {5} };
mli_tensor conv3pw_bias_tensor = { conv3_pw_bias, CONV3_OUT_CH, CONV3_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {3} };
mli_conv2d_cfg conv3pw_cfg = { { MLI_RELU_GEN }, 1, 1, 0, 0, 0, 0};

mli_tensor conv4dw_wt_tensor = { conv4_ds_wt, CONV3_OUT_CH*CONV4_DS_KX*CONV4_DS_KY, CONV4_OUT_CH, CONV3_OUT_CH, CONV4_DS_KY, CONV4_DS_KX, 4, MLI_EL_FX_8, {3} };
mli_tensor conv4dw_bias_tensor = { conv4_ds_bias, CONV4_OUT_CH, CONV4_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {2} };
mli_conv2d_cfg conv4dw_cfg = { { MLI_RELU_GEN }, CONV4_DS_SX, CONV4_DS_SY, CONV4_DS_PX, CONV4_DS_PX, CONV4_DS_PY, CONV4_DS_PY};

mli_tensor conv4pw_wt_tensor = { conv4_pw_wt, CONV4_OUT_CH*CONV3_OUT_CH, CONV4_OUT_CH, CONV4_OUT_CH, 1, 1, 4, MLI_EL_FX_8, {4} };
mli_tensor conv4pw_bias_tensor = { conv4_pw_bias, CONV4_OUT_CH, CONV4_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {2} };
mli_conv2d_cfg conv4pw_cfg = { { MLI_RELU_GEN }, 1, 1, 0, 0, 0, 0};

mli_tensor conv5dw_wt_tensor = { conv5_ds_wt, CONV4_OUT_CH*CONV5_DS_KX*CONV5_DS_KY, CONV5_OUT_CH, CONV4_OUT_CH, CONV5_DS_KY, CONV5_DS_KX, 4, MLI_EL_FX_8, {2} };
mli_tensor conv5dw_bias_tensor = { conv5_ds_bias, CONV5_OUT_CH, CONV5_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {2} };
mli_conv2d_cfg conv5dw_cfg = { { MLI_RELU_GEN }, CONV5_DS_SX, CONV5_DS_SY, CONV5_DS_PX, CONV5_DS_PX, CONV5_DS_PY, CONV5_DS_PY};

mli_tensor conv5pw_wt_tensor = { conv5_pw_wt, CONV5_OUT_CH*CONV4_OUT_CH, CONV5_OUT_CH, CONV5_OUT_CH, 1, 1, 4, MLI_EL_FX_8, {5} };
mli_tensor conv5pw_bias_tensor = { conv5_pw_bias, CONV5_OUT_CH, CONV5_OUT_CH, 1, 1, 1, 1, MLI_EL_FX_8, {3} };
mli_conv2d_cfg conv5pw_cfg = { { MLI_RELU_GEN }, 1, 1, 0, 0, 0, 0};

const mli_pool_cfg pool_cfg = { 5, 25, 1, 1, 0, 0, 0, 0 };

const mli_tensor fc_wt_tensor = { final_fc_wt, CONV5_OUT_CH*OUT_DIM, OUT_DIM, CONV5_OUT_CH, 1, 1, 2, MLI_EL_FX_8, {4} };
const mli_tensor fc_bias_tensor = { final_fc_bias, OUT_DIM, OUT_DIM, 1, 1, 1, 1, MLI_EL_FX_8, {5} };

int get_top_class(int8_t* prediction)
{
	int max_ind=0;
	int max_val=-128;
	for(int i=0;i<OUT_DIM;i++) {
		if(max_val<prediction[i]) {
			max_val = prediction[i];
			max_ind = i;
		}    
	}
	return max_ind;
}

void ds_cnn_init(void)
{

	mfcc_init(&mfcc_str, NUM_MFCC_COEFFS, FRAME_LEN, MFCC_DEC_BITS);
	mfcc_buffer = (int8_t*)malloc(sizeof(int8_t) * NUM_FRAMES * NUM_MFCC_COEFFS);
	if (mfcc_buffer == NULL)
	{
		EMBARC_PRINTF("The mfcc_buffer malloc failed\r\n");
	}

	scratch_pad = (int8_t *)malloc(sizeof(int8_t) * SCRATCH_BUFFER_SIZE);
	if (scratch_pad == NULL)
	{
		EMBARC_PRINTF("scratch_pad malloc failed\r\n");
	}
	BufferA_tensor.data = scratch_pad;
	BufferB_tensor.data = BufferA_tensor.data + ( 64*5*25 );
	col_buffer = BufferB_tensor.data + ( 64*5*25 );
}

void extract_features(MFCC_STR *mfcc_str, const int16_t *audio_data, int8_t *mfcc_buffer)
{
	uint16_t f;
	int32_t mfcc_buffer_head = 0;
	for (f = 0; f < NUM_FRAMES; f++)
	{
		mfcc_compute(mfcc_str, audio_data + (f * FRAME_SHIFT * 2),&mfcc_buffer[mfcc_buffer_head]);
		mfcc_buffer_head += NUM_MFCC_COEFFS;
	}
}

void run_nn_to_classify(int8_t* in_data, int8_t* out_data)
{
	input_tensor.data = in_data;
	BufferA_tensor.el_type = MLI_EL_FX_8;
	BufferB_tensor.el_type = MLI_EL_FX_8;

	BufferA_tensor.el_params.fx.frac_bits = 0;//specify the fractioin bits to maximum accuracy.
	BufferB_tensor.el_params.fx.frac_bits = 0;
	mli_krn_conv2d_chw_fx8 (&input_tensor, &conv1_wt_tensor, &conv1_bias_tensor, &conv1_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);
		
	BufferA_tensor.el_params.fx.frac_bits = 0;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_depthwise_conv2d_chw_fx8_k3x3_str1_krnpad(&BufferB_tensor, &conv2dw_wt_tensor, &conv2dw_bias_tensor, &conv2dw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 1;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_conv2d_chw_fx8_k1x1_str1_nopad(&BufferB_tensor, &conv2pw_wt_tensor, &conv2pw_bias_tensor, &conv2pw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 0;
	BufferB_tensor.el_params.fx.frac_bits = 4;
	mli_krn_depthwise_conv2d_chw_fx8_k3x3_str1_krnpad(&BufferB_tensor, &conv3dw_wt_tensor, &conv3dw_bias_tensor, &conv3dw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 1;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_conv2d_chw_fx8_k1x1_str1_nopad(&BufferB_tensor, &conv3pw_wt_tensor, &conv3pw_bias_tensor, &conv3pw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 0;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_depthwise_conv2d_chw_fx8_k3x3_str1_krnpad(&BufferB_tensor, &conv4dw_wt_tensor, &conv4dw_bias_tensor, &conv4dw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 0;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_conv2d_chw_fx8_k1x1_str1_nopad(&BufferB_tensor, &conv4pw_wt_tensor, &conv4pw_bias_tensor, &conv4pw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 0;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_depthwise_conv2d_chw_fx8_k3x3_str1_krnpad(&BufferB_tensor, &conv5dw_wt_tensor, &conv5dw_bias_tensor, &conv5dw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	BufferA_tensor.el_params.fx.frac_bits = 0;
	BufferB_tensor.el_params.fx.frac_bits = 3;
	mli_krn_conv2d_chw_fx8_k1x1_str1_nopad(&BufferB_tensor, &conv5pw_wt_tensor, &conv5pw_bias_tensor, &conv5pw_cfg, &BufferA_tensor);
	mli_krn_relu_fx8(&BufferA_tensor, &relu_cfg, &BufferB_tensor);

	mli_krn_avepool_chw_fx8_generic( &BufferB_tensor, &pool_cfg, &BufferA_tensor );

	BufferA_tensor.el_params.fx.frac_bits = 1;
	BufferB_tensor.el_params.fx.frac_bits = 0;
	mli_krn_fully_connected_fx8( &BufferA_tensor, &fc_wt_tensor, &fc_bias_tensor, &BufferB_tensor );

	mli_krn_softmax_fx8( &BufferB_tensor, &BufferA_tensor );
}

int kws_using_ds_cnn(int16_t *audio_buf)
{
	extract_features(&mfcc_str, audio_buf, mfcc_buffer);
	run_nn_to_classify(mfcc_buffer, NULL);
	return get_top_class(BufferA_tensor.data);
}
