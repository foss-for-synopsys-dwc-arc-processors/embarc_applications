# Application name
APPL ?= imu_net
BOARD ?= iotdk
TOOLCHAIN ?= gnu

APPL_DEFINES = -DUSE_APPL_MEM_CONFIG -DV2DSP_XY

EXT_DEV_LIST += sensor/imu/mpu9250

##
# Uncomment following options
# if you want to set your own heap and stack size
# Default settings see options.mk
##
# HEAPSZ ?= 9216
# STACKSZ ?= 9216

#
# root dir of embARC
#
EMBARC_ROOT = ../../../..

LIB_SEL = embarc_mli
MID_SEL = common

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk
