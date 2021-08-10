# Application name
APPL ?= NILM_IOTDK

APPL_DEFINES = 

#
# root dir of embARC
#
EMBARC_ROOT = ../../..

BOARD = iotdk
CUR_CORE = arcem9d
TOOLCHAIN = mw

#LIB_SEL = embarc_mli
LIB_SEL ?=
MID_SEL = common u8glib

# application source dirs
APPL_CSRC_DIR = . \
tensorflow/lite/c \
tensorflow/lite/experimental/microfrontend/lib \
./models

APPL_ASMSRC_DIR = .

APPL_CXXSRC_DIR = . \
./models \
tensorflow/lite/core/api \
tensorflow/lite/experimental/microfrontend/lib \
tensorflow/lite/kernels \
tensorflow/lite/kernels/internal \
tensorflow/lite/micro \
tensorflow/lite/micro/arc_iotdk \
tensorflow/lite/micro/kernels \
tensorflow/lite/micro/kernels/arc_mli \
tensorflow/lite/micro/memory_planner \
tensorflow/lite/micro/testing \
# application include dirs
APPL_INC_DIR = . \
./models \
tensorflow/lite \
tensorflow/lite/c \
tensorflow/lite/core/api \
tensorflow/lite/experimental/microfrontend/lib \
tensorflow/lite/kernels \
tensorflow/lite/kernels/internal \
tensorflow/lite/micro \
tensorflow/lite/micro/kernels \
tensorflow/lite/micro/kernels/arc_mli \
tensorflow/lite/micro/memory_planner \
tensorflow/lite/micro/testing \
tensorflow/lite/schema \
third_party/arc_mli_iotdk_arcem9d/include \
third_party/arc_mli_iotdk_arcem9d/include/api \
third_party/flatbuffers/include/flatbuffers \
third_party/gemmlowp/fixedpoint \
third_party/gemmlowp/internal \
third_party/kissfft \
third_party/ruy 

# application library
APPL_LIBS ?= third_party/arc_mli_iotdk_arcem9d/bin/libmli.a
# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk
