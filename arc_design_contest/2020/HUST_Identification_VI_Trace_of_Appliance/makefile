# Application name
APPL ?= VI_Images_Recognize

APPL_DEFINES = -DUSE_APPL_MEM_CONFIG -DV2DSP_XY -DMODEL_BIT_DEPTH=8

OLEVEL = O3
#
# root dir of embARC
#
EMBARC_ROOT = ../..

BOARD = iotdk
CUR_CORE = arcem9d
TOOLCHAIN = gnu


LIB_SEL = embarc_mli
MID_SEL = common u8glib



# application source dirs
APPL_CSRC_DIR = . ./model
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = . ./model

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk