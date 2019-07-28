# Application name
APPL ?= Edge_Crowd_Counting_System

# Optimization Level
# Please Refer to toolchain_xxx.mk for this option
OLEVEL ?= O2

##
# Current Board And Core
##
BOARD ?= emsk
BD_VER ?= 22
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
EMBARC_ROOT = ../../../..
# Selected OS
OS_SEL ?= freertos

MID_SEL = common fatfs

# application source dirs
APPL_CSRC_DIR = ./scr ./scr/wifi ./scr/emsk_sdcard
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = ./inc ./inc/emsk_sdcard

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk