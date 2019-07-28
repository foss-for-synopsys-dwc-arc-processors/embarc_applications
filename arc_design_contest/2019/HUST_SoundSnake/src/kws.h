#ifndef _KWS_H_
#define _KWS_H_

#include <stdlib.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "ds_cnn_weights.h"
#include "mfcc.h"

#define IN_DIM 490//(NUM_FRAMES*NUM_MFCC_COEFFS)
#define OUT_DIM 12

#define CONV1_OUT_CH 64
#define CONV1_IN_X 10
#define CONV1_IN_Y 49
#define CONV1_KX 4
#define CONV1_KY 10
#define CONV1_SX 2
#define CONV1_SY 2
#define CONV1_PX 1
#define CONV1_PY 4
#define CONV1_OUT_X 5
#define CONV1_OUT_Y 25
#define CONV1_BIAS_LSHIFT 2
#define CONV1_OUT_RSHIFT 6

#define CONV2_OUT_CH 64
#define CONV2_IN_X CONV1_OUT_X
#define CONV2_IN_Y CONV1_OUT_Y
#define CONV2_DS_KX 3
#define CONV2_DS_KY 3
#define CONV2_DS_SX 1
#define CONV2_DS_SY 1
#define CONV2_DS_PX 1
#define CONV2_DS_PY 1
#define CONV2_OUT_X 5
#define CONV2_OUT_Y 25
#define CONV2_DS_BIAS_LSHIFT 2
#define CONV2_DS_OUT_RSHIFT 5
#define CONV2_PW_BIAS_LSHIFT 4
#define CONV2_PW_OUT_RSHIFT 8

#define CONV3_OUT_CH 64
#define CONV3_IN_X CONV2_OUT_X
#define CONV3_IN_Y CONV2_OUT_Y
#define CONV3_DS_KX 3
#define CONV3_DS_KY 3
#define CONV3_DS_SX 1
#define CONV3_DS_SY 1
#define CONV3_DS_PX 1
#define CONV3_DS_PY 1
#define CONV3_OUT_X CONV3_IN_X
#define CONV3_OUT_Y CONV3_IN_Y
#define CONV3_DS_BIAS_LSHIFT 2
#define CONV3_DS_OUT_RSHIFT 4
#define CONV3_PW_BIAS_LSHIFT 5
#define CONV3_PW_OUT_RSHIFT 8

#define CONV4_OUT_CH 64
#define CONV4_IN_X CONV3_OUT_X
#define CONV4_IN_Y CONV3_OUT_Y
#define CONV4_DS_KX 3
#define CONV4_DS_KY 3
#define CONV4_DS_SX 1
#define CONV4_DS_SY 1
#define CONV4_DS_PX 1
#define CONV4_DS_PY 1
#define CONV4_OUT_X CONV4_IN_X
#define CONV4_OUT_Y CONV4_IN_Y
#define CONV4_DS_BIAS_LSHIFT 3
#define CONV4_DS_OUT_RSHIFT 5
#define CONV4_PW_BIAS_LSHIFT 5
#define CONV4_PW_OUT_RSHIFT 7

#define CONV5_OUT_CH 64
#define CONV5_IN_X CONV4_OUT_X
#define CONV5_IN_Y CONV4_OUT_Y
#define CONV5_DS_KX 3
#define CONV5_DS_KY 3
#define CONV5_DS_SX 1
#define CONV5_DS_SY 1
#define CONV5_DS_PX 1
#define CONV5_DS_PY 1
#define CONV5_OUT_X CONV5_IN_X
#define CONV5_OUT_Y CONV5_IN_Y
#define CONV5_DS_BIAS_LSHIFT 3
#define CONV5_DS_OUT_RSHIFT 5
#define CONV5_PW_BIAS_LSHIFT 5
#define CONV5_PW_OUT_RSHIFT 8

#define FINAL_FC_BIAS_LSHIFT 2
#define FINAL_FC_OUT_RSHIFT 7

#define SCRATCH_BUFFER_SIZE (2*(2*64*3*3+64*5*25))

#if defined(__cplusplus)
extern "C" {
#endif

/*
 * @brief cnn相关初始化
 */
void ds_cnn_init(void);
/*
 * @brief 对缓存的语音数据做识别
 * @param audio_buf 音频数据
 * @return 返回识别结果对应的编号
 */
int kws_using_ds_cnn(int16_t *audio_buf);
/*
 * @brief 提取MFCC特征
 * @param mfcc_buffer 返回mfcc特征的buf
 */
//void extract_features(MFCC_STR *str, const int16_t *audio_data, q7_t *mfcc_buffer);
/*
 * @brief 提取完特征后执行cnn进行推理
 * @param in_data 输入特征
 * @param out_data 输出结果
 */
void run_nn_to_classify(int8_t* in_data, int8_t* out_data);

#if defined(__cplusplus)
}
#endif

#endif
