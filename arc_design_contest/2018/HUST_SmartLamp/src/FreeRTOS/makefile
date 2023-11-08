# Application name
APPL ?= lab_9_freertos

# Optimization Level
# Please Refer to toolchain_xxx.mk for this option
OLEVEL ?= O2

##
# Current Board And Core
##
BOARD ?= emsk
BD_VER ?= 22
CUR_CORE ?= arcem7d

##
# select debugging jtag
##
JTAG ?= usb

##
# Set toolchain
##
TOOLCHAIN ?= gnu
#TOOLCHAIN ?= gnu

# Selected OS
OS_SEL ?= freertos

#
# root dir of embARC
#
EMBARC_ROOT = ../../..

MID_SEL = common

# application source dirs
APPL_CSRC_DIR = . ./app/crc32 ./app/encrypt ./app/ESP8266 ./app/initsys ./app/package ./app/strpos \
		./app/usart1task ./app/usart3task ./hardware/timer ./hardware/i2c ./app/usart4task ./app/fifo \
		./app/cm3232 ./app/bright ./app/irf ./app/Cjson
		
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = . ./app/crc32 ./app/encrypt ./app/ESP8266 ./app/initsys ./app/package ./app/strpos \
		./app/usart1task ./app/usart3task ./hardware/timer ./hardware/i2c ./appinc ./app/usart4task ./app/fifo \
		./app/cm3232 ./app/bright ./app/irf ./app/Cjson

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
