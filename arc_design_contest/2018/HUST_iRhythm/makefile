# Attention!!!
# Attention!!!
# the newest embarc osp commit:976b8ed3b24b061df91f78bcc71a726dd2a63fae
# is necessary to compile!!!
# because EZ_SIO has become a part of Middleware in resent update!!!
# 

TOOLCHAIN=gnu
BOARD=emsk
BD_VER=23
CUR_CORE=arcem7d

# Application name
APPL ?= iRhythm

OLEVEL ?= O3


HEAPSZ ?= 2097152
STACKSZ ?= 2097152

#
# root dir of embARC
#
EMBARC_ROOT = ../..
MID_SEL = common fatfs
OS_SEL = freertos
# application source dirs
APPL_CSRC_DIR = ./src/GUI ./src/MUSIC ./src/FFT ./src/MUSIC/mp3_dec ./src/MUSIC/mp3_dec/mp3_Ori ./src/MUSIC/mp3_dec/mp3_Dsp ./src/HW_ASSI ./src/NET ./src/NET/esp8266_wifi ./src/TASK 

APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR =  ./inc ./inc/mp3_dec

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

LINKER_SCRIPT_FILE ?= linker_file.ldf
ADT_COPT = -mlong-calls


### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
