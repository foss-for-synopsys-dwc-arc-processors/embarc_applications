# Application name
APPL ?= MacTavish

EXT_DEV_LIST += ble/rn4020

# Optimization Level
# Please Refer to toolchain_xxx.mk for this option
OLEVEL ?= Os

##
# root dir of embARC
##
ifndef HOME
HOME= $(USERPROFILE)
endif

##
# Current Board And Core
##
BOARD ?= iotdk
BD_VER ?= 10
CUR_CORE ?= arcem9d

##
# select debugging jtag
##
JTAG ?= usb

##
# Set toolchain
##
TOOLCHAIN ?= mw

#
# root dir of embARC
#
EMBARC_ROOT = ../..
# Selected OS
OS_SEL ?= freertos

MID_SEL = common
STACKSZ = 16384
HEAPSZ = 34208

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

APPL_LIBS= $(TOOLCHAIN)/libDSpotter.a

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk