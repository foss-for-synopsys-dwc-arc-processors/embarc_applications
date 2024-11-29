// AUTO GENERATED FILE

#include <mli_api.h>
#include "model_coeff.inc"


//   conv1 params
//======================================================
//const int8_t L1_conv1_wt_buf[] = CONV1_W;
//const int8_t L1_conv1_b_buf[] = CONV1_B;
const uint8_t L1_conv1_in_frac_bits = 4;
const uint8_t L1_conv1_out_frac_bits = 4;

const mli_tensor L1_conv1_wt = {
    .data = (void *)L1_conv1_wt_buf,
    .capacity = sizeof(L1_conv1_wt),
    .shape = {16,1,3,3},
    .rank = 4,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 7
};

const mli_tensor L1_conv1_b = {
    .data = (void *)L1_conv1_b_buf,
    .capacity = sizeof(L1_conv1_b),
    .shape = {16},
    .rank = 1,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 9
};

//   conv2 params
//======================================================
//const int8_t L2_conv2_wt_buf[] = CONV2_W;
//const int8_t L2_conv2_b_buf[] = CONV2_B;
const uint8_t L2_conv2_in_frac_bits = 4;
const uint8_t L2_conv2_out_frac_bits = 4;

const mli_tensor L2_conv2_wt = {
    .data = (void *)L2_conv2_wt_buf,
    .capacity = sizeof(L2_conv2_wt),
    .shape = {16,16,3,3},
    .rank = 4,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 8
};

const mli_tensor L2_conv2_b = {
    .data = (void *)L2_conv2_b_buf,
    .capacity = sizeof(L2_conv2_b),
    .shape = {16},
    .rank = 1,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 10
};

//   fc4 params
//======================================================
//const int8_t L3_fc4_wt_buf[] = FC4_W;
//const int8_t L3_fc4_b_buf[] = FC4_B;
const uint8_t L3_fc4_in_frac_bits = 4;
const uint8_t L3_fc4_out_frac_bits = 4;

const mli_tensor L3_fc4_wt = {
    .data = (void *)L3_fc4_wt_buf,
    .capacity = sizeof(L3_fc4_wt),
    .shape = {16,800},
    .rank = 2,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 9
};

const mli_tensor L3_fc4_b = {
    .data = (void *)L3_fc4_b_buf,
    .capacity = sizeof(L3_fc4_b),
    .shape = {16},
    .rank = 1,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 12
};

//   fc5 params
//======================================================
//const int8_t L4_fc5_wt_buf[] = FC5_W;
//const int8_t L4_fc5_b_buf[] = FC5_B;
const uint8_t L4_fc5_in_frac_bits = 4;
const uint8_t L4_fc5_out_frac_bits = 4;

const mli_tensor L4_fc5_wt = {
    .data = (void *)L4_fc5_wt_buf,
    .capacity = sizeof(L4_fc5_wt),
    .shape = {2,16},
    .rank = 2,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 7
};

const mli_tensor L4_fc5_b = {
    .data = (void *)L4_fc5_b_buf,
    .capacity = sizeof(L4_fc5_b),
    .shape = {2},
    .rank = 1,
    .el_type = MLI_EL_FX_8,
    .el_params.fx.frac_bits = 10
};
