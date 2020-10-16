# Application name
APPL ?= SHMS


CUR_CORE = arcem9d

BOARD = iotdk
TOOLCHAIN = gnu

APPL_DEFINES = -DUSE_APPL_MEM_CONFIG -DV2DSP_XY -DMODEL_BIT_DEPTH=8
#
# root dir of embARC
#
EMBARC_ROOT = ../../../..

MID_SEL = common 
LIB_SEL = embarc_mli
# application source dirs
APPL_CSRC_DIR = ./src
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = ./src

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk
