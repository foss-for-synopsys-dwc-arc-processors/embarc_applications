# Application name
APPL ?= bluetooth_speaker
BOARD ?= emsk
TOOLCHAIN ?= gnu
OLEVEL ?= 02
BD_VER ?= 22
CUR_CORE ?= arcem7d

#EXT_DEV_LIST += ble/hm1x
#
# root dir of embARC
#
EMBARC_ROOT = ../../../../..

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
