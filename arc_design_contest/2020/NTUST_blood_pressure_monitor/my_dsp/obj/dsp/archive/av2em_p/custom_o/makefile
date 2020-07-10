# libdsp arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -DCACHE_LINE_SIZE=32 -DEMU__Xmul32x16 -DEMU__Xxmac_24 -DEMU__Xxmac_d16 -DEMU__Xea -DEMU__Xbarrel_shifter -Hsdata0 -Hnoxcheck -av2em -O -Hon=long_enums
CC_INC= -I$(MWLIB)/dsp/helper_g -I$(MWLIB)/dsp/hl_api_g/transforms_g -I$(MWLIB)/dsp/include -I$(MWLIB)/dsp/include/platform_lib -I$(MWLIB)/c/inc/internal -I$(MWLIB)/c/inc
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libdsp.a
AR_OPT=""

LIST_hl_api_vectors=dsp_cmpl_dot_prod_q31.o dsp_abs_f32.o dsp_add_f32.o\
	dsp_sub_f32.o dsp_mult_f32.o dsp_negate_f32.o dsp_offset_f32.o\
	dsp_scale_f32.o dsp_cmpl_conj_f32.o dsp_cmpl_mag_f32.o\
	dsp_cmpl_mag_squared_f32.o dsp_dot_prod_f32.o dsp_max_f32.o\
	dsp_min_f32.o dsp_cmpl_mag_q15.o dsp_abs_q7.o dsp_add_q7.o\
	dsp_negate_q7.o dsp_offset_q7.o dsp_sub_q7.o dsp_cast_vect_f32.o\
	dsp_cast_vect_q7.o dsp_cast_vect_q15.o dsp_cast_vect_q31.o\
	dsp_shift_q7.o dsp_scale_q7.o dsp_mult_q7.o dsp_add_q15.o dsp_add_q31.o\
	dsp_sub_q15.o dsp_sub_q31.o dsp_cmpl_conj_q15.o dsp_abs_q15.o\
	dsp_abs_q31.o dsp_cmpl_mult_real_q31.o dsp_max_q15.o dsp_max_q31.o\
	dsp_max_q7.o dsp_min_q15.o dsp_min_q31.o dsp_min_q7.o dsp_negate_q15.o\
	dsp_negate_q31.o dsp_offset_q15.o dsp_offset_q31.o dsp_shift_q15.o\
	dsp_cmpl_dot_prod_f32.o dsp_cmpl_mult_real_f32.o dsp_dot_prod_q31.o\
	dsp_dot_prod_q7.o dsp_cmpl_dot_prod_q15.o dsp_cmpl_mag_q31.o\
	dsp_mult_q15.o dsp_scale_q31.o dsp_shift_q31.o dsp_cmpl_conj_q31.o\
	dsp_cmpl_mult_cmpl_q15.o dsp_cmpl_mult_cmpl_q31.o\
	dsp_cmpl_mult_real_q15.o dsp_dot_prod_q15.o dsp_cmpl_mag_squared_q15.o\
	dsp_cmpl_mag_squared_q31.o dsp_mult_q31.o dsp_scale_q15.o\
	dsp_cmpl_mult_cmpl_f32.o

LIST_hl_api_filtering=dsp_conv_f32.o dsp_correlate_f32.o dsp_fir_decimate_f32.o\
	dsp_fir_f32.o dsp_fir_interpolate_f32.o dsp_lms_f32.o\
	dsp_iir_lattice_f32.o dsp_fir_decimate_q15.o dsp_fir_decimate_q31.o\
	dsp_fir_interpolate_q15.o dsp_fir_interpolate_q31.o dsp_fir_q15.o\
	dsp_fir_q15_complex.o dsp_fir_q31.o dsp_fir_q31_complex.o dsp_fir_q7.o\
	dsp_lms_q15.o dsp_lms_q31.o dsp_biquad_cascade_df1_q15.o\
	dsp_biquad_cascade_df1_q31.o dsp_conv_q7.o dsp_conv_q15.o\
	dsp_conv_q31.o dsp_correlate_q7.o dsp_correlate_q15.o\
	dsp_iir_lattice_q15.o dsp_biquad_cascade_df1_f32.o dsp_correlate_q31.o\
	dsp_iir_lattice_q31.o dsp_biquad_cascade_df2T_f32.o\
	dsp_biquad_cascade_df2T_f64.o

LIST_hl_api_interpolation=dsp_linear_interp_f32.o dsp_bilinear_interp_f32.o\
	dsp_pol_interp_f32.o dsp_bilinear_interp_q15.o\
	dsp_bilinear_interp_q31.o dsp_bilinear_interp_q7.o\
	dsp_linear_interp_q31.o dsp_linear_interp_q7.o dsp_pol_interp_q7.o\
	dsp_linear_interp_q15.o dsp_pol_interp_q31.o dsp_pol_interp_q15.o

LIST_hl_api_math=dsp_sincos_f32.o dsp_sqrt_f32.o dsp_atan2_q15.o\
	dsp_atan2_q31.o dsp_div_q15.o dsp_div_q31.o dsp_log2_q15.o\
	dsp_log2_q31.o dsp_asin_q31.o dsp_asin_q15.o dsp_acos_q31.o\
	dsp_acos_q15.o dsp_atan_q31.o dsp_atan_q15.o dsp_tan_q31.o\
	dsp_tan_q15.o dsp_pow2x_q15.o dsp_pow2x_q31.o dsp_recip_q15.o\
	dsp_sqrt_q15.o dsp_recip_q31.o dsp_sqrt_q31.o dsp_reciprt_q31.o\
	dsp_sincos_q15.o dsp_sincos_q31.o

LIST_hl_api_matrix=dsp_mat_add_f32.o dsp_mat_init_f32.o dsp_mat_init_f64.o\
	dsp_mat_inverse_f32.o dsp_mat_inverse_f64.o dsp_mat_mult_f32.o\
	dsp_mat_scale_f32.o dsp_mat_sub_f32.o dsp_mat_trans_f32.o\
	dsp_cmpl_mat_add_f32.o dsp_cmpl_mat_sub_f32.o dsp_cmpl_mat_scale_f32.o\
	dsp_cmpl_mat_mult_f32.o dsp_cmpl_mat_trans_f32.o dsp_cmpl_mat_add_q31.o\
	dsp_cmpl_mat_sub_q31.o dsp_cmpl_mat_scale_q31.o dsp_cmpl_mat_mult_q31.o\
	dsp_cmpl_mat_trans_q31.o dsp_cmpl_mat_add_q15.o dsp_cmpl_mat_sub_q15.o\
	dsp_cmpl_mat_scale_q15.o dsp_cmpl_mat_mult_q15.o\
	dsp_cmpl_mat_trans_q15.o dsp_mat_add_q31.o dsp_mat_init_q15.o\
	dsp_mat_init_q31.o dsp_mat_init_q7.o dsp_mat_scale_q31.o\
	dsp_mat_sub_q31.o dsp_mat_trans_q15.o dsp_mat_trans_q31.o\
	dsp_mat_mult_q15.o dsp_mat_mult_q31.o dsp_mat_add_q15.o\
	dsp_mat_scale_q15.o dsp_mat_sub_q15.o

LIST_hl_api_transforms=dsp_cfft_q15.o dsp_cifft_q15.o dsp_cfft_q31.o\
	dsp_cfft_f32.o dsp_cifft_q31.o dsp_cifft_f32.o dsp_dct2_q31.o\
	dsp_dct2_q15.o dsp_dct3_q31.o dsp_dct3_q15.o dsp_imdct_q31.o\
	dsp_imdct_q15.o dsp_mdct_q31.o dsp_mdct_q15.o dsp_rfft_q15.o\
	dsp_rfft_q31.o dsp_rfft_f32.o dsp_rifft_q15.o dsp_rifft_q31.o\
	dsp_rifft_f32.o dsp_dct4_q31.o dsp_dct4_q15.o dsp_transform_tables.o\
	dsp_transform_tables_fct4.o

LIST_hl_api_motor_ctrl=dsp_clarke_f32.o dsp_clarke_q31.o dsp_clarke_q15.o\
	dsp_clarke_inv_f32.o dsp_clarke_inv_q31.o dsp_clarke_inv_q15.o\
	dsp_park_f32.o dsp_park_q31.o dsp_park_q15.o dsp_park_inv_f32.o\
	dsp_park_inv_q31.o dsp_park_inv_q15.o dsp_pid_control_f32.o\
	dsp_pid_control_q31.o dsp_pid_control_q15.o

LIST_helper=dsplib_helper.o

LIST_platform=arc_emu.o

LIST_ALL=$(LIST_hl_api_vectors) $(LIST_hl_api_filtering) $(LIST_hl_api_interpolation) $(LIST_hl_api_math) $(LIST_hl_api_matrix) $(LIST_hl_api_transforms) $(LIST_hl_api_motor_ctrl) $(LIST_helper) $(LIST_platform)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_vectors)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_filtering)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_interpolation)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_math)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_matrix)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_transforms)
	$(AR) rv $(AR_NAME) $(LIST_hl_api_motor_ctrl)
	$(AR) rv $(AR_NAME) $(LIST_helper)
	$(AR) rv $(AR_NAME) $(LIST_platform)

hl_api_vectors: $(LIST_hl_api_vectors)
hl_api_filtering: $(LIST_hl_api_filtering)
hl_api_interpolation: $(LIST_hl_api_interpolation)
hl_api_math: $(LIST_hl_api_math)
hl_api_matrix: $(LIST_hl_api_matrix)
hl_api_transforms: $(LIST_hl_api_transforms)
hl_api_motor_ctrl: $(LIST_hl_api_motor_ctrl)
helper: $(LIST_helper)
platform: $(LIST_platform)

dsp_cmpl_dot_prod_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_dot_prod_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_dot_prod_q31.c

dsp_conv_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_f32.c

dsp_correlate_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_f32.c

dsp_fir_decimate_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_decimate_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_decimate_f32.c

dsp_fir_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_f32.c

dsp_fir_interpolate_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_interpolate_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_interpolate_f32.c

dsp_lms_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_lms_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_lms_f32.c

dsp_linear_interp_f32.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_f32.c

dsp_bilinear_interp_f32.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_f32.c

dsp_pol_interp_f32.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_f32.c

dsp_iir_lattice_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_iir_lattice_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_iir_lattice_f32.c

dsp_sincos_f32.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_sincos_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_sincos_f32.c

dsp_sqrt_f32.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_sqrt_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_sqrt_f32.c

dsp_mat_add_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_add_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_add_f32.c

dsp_mat_init_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_f32.c

dsp_mat_init_f64.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_f64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_f64.c

dsp_mat_inverse_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_inverse_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_inverse_f32.c

dsp_mat_inverse_f64.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_inverse_f64.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_inverse_f64.c

dsp_mat_mult_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_mult_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_mult_f32.c

dsp_mat_scale_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_scale_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_scale_f32.c

dsp_mat_sub_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_sub_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_sub_f32.c

dsp_mat_trans_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_trans_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_trans_f32.c

dsp_cmpl_mat_add_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_add_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_add_f32.c

dsp_cmpl_mat_sub_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_sub_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_sub_f32.c

dsp_cmpl_mat_scale_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_scale_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_scale_f32.c

dsp_cmpl_mat_mult_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_mult_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) -mllvm -arc-postinc-sched $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_mult_f32.c

dsp_cmpl_mat_trans_f32.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_trans_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_trans_f32.c

dsp_cmpl_mat_add_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_add_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_add_q31.c

dsp_cmpl_mat_sub_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_sub_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_sub_q31.c

dsp_cmpl_mat_scale_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_scale_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_scale_q31.c

dsp_cmpl_mat_mult_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_mult_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_mult_q31.c

dsp_cmpl_mat_trans_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_trans_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_trans_q31.c

dsp_cmpl_mat_add_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_add_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_add_q15.c

dsp_cmpl_mat_sub_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_sub_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_sub_q15.c

dsp_cmpl_mat_scale_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_scale_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_scale_q15.c

dsp_cmpl_mat_mult_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_mult_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_mult_q15.c

dsp_cmpl_mat_trans_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_trans_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_cmpl_mat_trans_q15.c

dsp_abs_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_f32.c

dsp_add_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_f32.c

dsp_sub_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_f32.c

dsp_mult_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_f32.c

dsp_negate_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_f32.c

dsp_offset_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_f32.c

dsp_scale_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_f32.c

dsp_cmpl_conj_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_conj_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_conj_f32.c

dsp_cmpl_mag_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) -menable-unsafe-fp-math $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_f32.c

dsp_cmpl_mag_squared_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_squared_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_squared_f32.c

dsp_dot_prod_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_f32.c

dsp_max_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_f32.c

dsp_min_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_f32.c

dsp_atan2_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan2_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan2_q15.c

dsp_atan2_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan2_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan2_q31.c

dsp_bilinear_interp_q15.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_q15.c

dsp_bilinear_interp_q31.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_q31.c

dsp_bilinear_interp_q7.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_bilinear_interp_q7.c

dsp_cfft_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cfft_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) -fno-allow-null-deref $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cfft_q15.c

dsp_cifft_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cifft_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) -fno-allow-null-deref $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cifft_q15.c

dsp_div_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_div_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_div_q15.c

dsp_div_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_div_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_div_q31.c

dsp_fir_decimate_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_decimate_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_decimate_q15.c

dsp_fir_decimate_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_decimate_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_decimate_q31.c

dsp_fir_interpolate_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_interpolate_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_interpolate_q15.c

dsp_fir_interpolate_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_interpolate_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_interpolate_q31.c

dsp_fir_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q15.c

dsp_fir_q15_complex.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q15_complex.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q15_complex.c

dsp_fir_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q31.c

dsp_fir_q31_complex.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q31_complex.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q31_complex.c

dsp_fir_q7.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_fir_q7.c

dsp_linear_interp_q31.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_q31.c

dsp_linear_interp_q7.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_q7.c

dsp_lms_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_lms_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_lms_q15.c

dsp_lms_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_lms_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_lms_q31.c

dsp_log2_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_log2_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_log2_q15.c

dsp_log2_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_log2_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_log2_q31.c

dsp_asin_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_asin_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_asin_q31.c

dsp_asin_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_asin_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_asin_q15.c

dsp_acos_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_acos_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_acos_q31.c

dsp_acos_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_acos_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_acos_q15.c

dsp_atan_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan_q31.c

dsp_atan_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_atan_q15.c

dsp_tan_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_tan_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_tan_q31.c

dsp_tan_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_tan_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_tan_q15.c

dsp_mat_add_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_add_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_add_q31.c

dsp_mat_init_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_q15.c

dsp_mat_init_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_q31.c

dsp_mat_init_q7.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_init_q7.c

dsp_mat_scale_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_scale_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_scale_q31.c

dsp_mat_sub_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_sub_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_sub_q31.c

dsp_mat_trans_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_trans_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_trans_q15.c

dsp_mat_trans_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_trans_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_trans_q31.c

dsp_pol_interp_q7.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_q7.c

dsp_pow2x_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_pow2x_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_pow2x_q15.c

dsp_pow2x_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_pow2x_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_pow2x_q31.c

dsp_recip_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_recip_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_recip_q15.c

dsp_sqrt_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_sqrt_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_sqrt_q15.c

dsp_linear_interp_q15.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_linear_interp_q15.c

dsp_recip_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_recip_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_recip_q31.c

dsp_sqrt_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_sqrt_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_sqrt_q31.c

dsp_reciprt_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_reciprt_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_reciprt_q31.c

dsp_mat_mult_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_mult_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_mult_q15.c

dsp_mat_mult_q31.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_mult_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) -D_XY_HAND_OPT -fno-allow-null-deref $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_mult_q31.c

dsp_cmpl_mag_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_q15.c

dsp_abs_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_q7.c

dsp_add_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_q7.c

dsp_negate_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_q7.c

dsp_offset_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_q7.c

dsp_sub_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_q7.c

dsp_cast_vect_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_f32.c

dsp_cast_vect_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_q7.c

dsp_cast_vect_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_q15.c

dsp_cast_vect_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cast_vect_q31.c

dsp_shift_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_shift_q7.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_shift_q7.c

dsp_clarke_f32.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_f32.c

dsp_clarke_q31.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_q31.c

dsp_clarke_q15.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_q15.c

dsp_clarke_inv_f32.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_inv_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_inv_f32.c

dsp_clarke_inv_q31.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_inv_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_inv_q31.c

dsp_clarke_inv_q15.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_inv_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_clarke_inv_q15.c

dsp_park_f32.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_f32.c

dsp_park_q31.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_q31.c

dsp_park_q15.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_q15.c

dsp_park_inv_f32.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_inv_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_inv_f32.c

dsp_park_inv_q31.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_inv_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_inv_q31.c

dsp_park_inv_q15.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_inv_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_park_inv_q15.c

dsp_pid_control_f32.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_pid_control_f32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_pid_control_f32.c

dsp_pid_control_q31.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_pid_control_q31.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_pid_control_q31.c

dsp_pid_control_q15.o: $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_pid_control_q15.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/motor_ctrl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/motor_ctrl_g/dsp_pid_control_q15.c

dsp_scale_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_q7.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_q7.c

dsp_mult_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_q7.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_q7.c

dsp_add_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_q15.c

dsp_add_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_add_q31.c

dsp_sub_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_q15.c

dsp_sub_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_sub_q31.c

dsp_cmpl_conj_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_conj_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_conj_q15.c

dsp_abs_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_q15.c

dsp_abs_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_abs_q31.c

dsp_cmpl_mult_real_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_real_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_real_q31.c

dsp_biquad_cascade_df1_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df1_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 -mllvm -regalloc=chaitin $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df1_q15.c

dsp_biquad_cascade_df1_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df1_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df1_q31.c

dsp_conv_q7.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_q7.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_q7.c

dsp_conv_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_q15.c

dsp_conv_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 -Hunswitch=0 -mllvm -regalloc=chaitin $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_conv_q31.c

dsp_correlate_q7.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_q7.c
	$(CCAC) $(CC_OPT) -Hunswitch=0 -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_q7.c

dsp_correlate_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunswitch=0 -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_q15.c

dsp_iir_lattice_q15.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_iir_lattice_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_iir_lattice_q15.c

dsp_max_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_q15.c

dsp_max_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_q31.c

dsp_max_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_q7.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_max_q7.c

dsp_min_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_q15.c

dsp_min_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_q31.c

dsp_min_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_q7.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_min_q7.c

dsp_negate_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_q15.c

dsp_negate_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_negate_q31.c

dsp_offset_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_q15.c

dsp_offset_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_offset_q31.c

dsp_shift_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_shift_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_shift_q15.c

dsp_cmpl_dot_prod_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_dot_prod_f32.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_dot_prod_f32.c

dsp_cmpl_mult_real_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_real_f32.c
	$(CCAC) $(CC_OPT) -mllvm -arc-postinc-sched -Hunroll=2,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_real_f32.c

dsp_dot_prod_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_q31.c

dsp_dot_prod_q7.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_q7.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_q7.c

dsp_cmpl_dot_prod_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_dot_prod_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_dot_prod_q15.c

dsp_cmpl_mag_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_q31.c

dsp_mult_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_q15.c

dsp_scale_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_q31.c

dsp_shift_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_shift_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_shift_q31.c

dsp_biquad_cascade_df1_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df1_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -menable-unsafe-fp-math -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df1_f32.c

dsp_cfft_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cfft_q31.c
	$(CCAC) $(CC_OPT) -Hmisched -Hunswitch=150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cfft_q31.c

dsp_cfft_f32.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cfft_f32.c
	$(CCAC) $(CC_OPT) -Hmisched -Hunswitch=150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cfft_f32.c

dsp_cifft_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cifft_q31.c
	$(CCAC) $(CC_OPT) -Hmisched -Hunswitch=150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cifft_q31.c

dsp_cifft_f32.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cifft_f32.c
	$(CCAC) $(CC_OPT) -Hmisched -Hunswitch=150 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_cifft_f32.c

dsp_dct2_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct2_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct2_q31.c

dsp_dct2_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct2_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct2_q15.c

dsp_dct3_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct3_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct3_q31.c

dsp_dct3_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct3_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct3_q15.c

dsp_imdct_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_imdct_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_imdct_q31.c

dsp_imdct_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_imdct_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_imdct_q15.c

dsp_mdct_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_mdct_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_mdct_q31.c

dsp_mdct_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_mdct_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_mdct_q15.c

dsp_pol_interp_q31.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_q31.c

dsp_rfft_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rfft_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rfft_q15.c

dsp_rfft_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rfft_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rfft_q31.c

dsp_rfft_f32.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rfft_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rfft_f32.c

dsp_rifft_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rifft_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rifft_q15.c

dsp_rifft_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rifft_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rifft_q31.c

dsp_rifft_f32.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rifft_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_rifft_f32.c

dsp_sincos_q15.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_sincos_q15.c
	$(CCAC) $(CC_OPT) -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_sincos_q15.c

dsp_sincos_q31.o: $(MWLIB)/dsp/hl_api_g/math_g/dsp_sincos_q31.c
	$(CCAC) $(CC_OPT) -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/math_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/math_g/dsp_sincos_q31.c

dsp_dct4_q31.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct4_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct4_q31.c

dsp_pol_interp_q15.o: $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/interpolation_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/interpolation_g/dsp_pol_interp_q15.c

dsp_correlate_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 -mllvm -regalloc=chaitin -Hunswitch=0 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_correlate_q31.c

dsp_cmpl_conj_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_conj_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=2,150 -mllvm -regalloc=chaitin $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_conj_q31.c

dsp_cmpl_mult_cmpl_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_cmpl_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_cmpl_q15.c

dsp_cmpl_mult_cmpl_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_cmpl_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched -D_XY_HAND_OPT $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_cmpl_q31.c

dsp_cmpl_mult_real_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_real_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_real_q15.c

dsp_dct4_q15.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct4_q15.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_dct4_q15.c

dsp_dot_prod_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_dot_prod_q15.c

dsp_iir_lattice_q31.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_iir_lattice_q31.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -mllvm -ind-var=0 -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_iir_lattice_q31.c

dsp_cmpl_mag_squared_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_squared_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_squared_q15.c

dsp_cmpl_mag_squared_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_squared_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mag_squared_q31.c

dsp_mat_add_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_add_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_add_q15.c

dsp_mat_scale_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_scale_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_scale_q15.c

dsp_mat_sub_q15.o: $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_sub_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/matrix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/matrix_g/dsp_mat_sub_q15.c

dsp_mult_q31.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_q31.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_mult_q31.c

dsp_scale_q15.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_q15.c
	$(CCAC) $(CC_OPT) -Hunroll=4,150 -Hmisched $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_scale_q15.c

dsp_biquad_cascade_df2T_f32.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df2T_f32.c
	$(CCAC) $(CC_OPT) -mllvm -regalloc=chaitin $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df2T_f32.c

dsp_biquad_cascade_df2T_f64.o: $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df2T_f64.c
	$(CCAC) $(CC_OPT) -mllvm -regalloc=chaitin $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/filtering_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/filtering_g/dsp_biquad_cascade_df2T_f64.c

dsp_cmpl_mult_cmpl_f32.o: $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_cmpl_f32.c
	$(CCAC) $(CC_OPT) -D_XY_HAND_OPT -mllvm -arc-postinc-sched -menable-unsafe-fp-math -Hunroll=2,150 -Hld_cycles=2 $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/vectors_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/vectors_g/dsp_cmpl_mult_cmpl_f32.c

dsplib_helper.o: $(MWLIB)/dsp/helper_g/dsplib_helper.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/helper_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/helper_g/dsplib_helper.c

arc_emu.o: $(MWLIB)/dsp/platform_g/arc_emu.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/platform_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/platform_g/arc_emu.c

dsp_transform_tables.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_transform_tables.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_transform_tables.c

dsp_transform_tables_fct4.o: $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_transform_tables_fct4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/dsp/hl_api_g/transforms_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/dsp/hl_api_g/transforms_g/dsp_transform_tables_fct4.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
