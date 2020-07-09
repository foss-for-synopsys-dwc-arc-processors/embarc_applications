# Application name
APPL ?= NCKU 
BOARD = iotdk
TOOLCHAIN = mw
CFLAGS = -HPurge
APPL_DEFINES = -DUSE_APPL_MEM_CONFIG -DV2DSP_XY -DMODEL_BIT_DEPTH=16 
# root dir of embARC
EMBARC_ROOT = ../../../../

LIB_SEL = embarc_mli
MID_SEL = common

# application source dirs
APPL_CSRC_DIR = ./src
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = ./inc

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk
