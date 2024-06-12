## embARC application makefile template ##
### You can copy this file to your application folder
### and rename it to makefile.
##

# Application name
APPL ?= bed_c

# Optimization Level
# Please Refer to toolchain_xxx.mk for this option
# OLEVEL ?= O2

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
##s
TOOLCHAIN ?= gnu

##
# Uncomment following options
# if you want to set your own heap and stack size
# Default settings see options.mk
##
#HEAPSZ ?= 8192
#STACKSZ ?= 8192

##
# Uncomment following options
# if you want to add your own library into link process
# For example:
# If you want link math lib for gnu toolchain,
# you need to set the option to -lm
##
#APPL_LIBS ?=

#
# root dir of embARC
#
EMBARC_ROOT = ../../..

MID_SEL = common

#add path to ad7991
DEV_CSRCDIR +=  $(EMBARC_ROOT)/device/peripheral/adc/ad7991
DEV_INCDIR +=  $(EMBARC_ROOT)/device/peripheral/adc/ad7991
#end add path in ad7991

#add path in hm1X
DEV_CSRCDIR +=  $(EMBARC_ROOT)/device/peripheral/ble/hm1x
DEV_INCDIR +=  $(EMBARC_ROOT)/device/peripheral/ble/hm1x
#end add path in hm1X

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

# application defines
APPL_DEFINES =

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
