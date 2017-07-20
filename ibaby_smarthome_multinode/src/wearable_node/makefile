# Application name
APPL ?= wearable_node

# Selected OS
OS_SEL ?= freertos

BOARD ?= emsk
BD_VER ?= 22
CUR_CORE ?= arcem7d
TOOLCHAIN ?= gnu

##
# HEAP & STACK SETTINGS
# For LwM2M Stack Usage
##
HEAPSZ ?= 81920
STACKSZ ?= 81920

#
# root dir of embARC
#
EMBARC_ROOT = ../../../..

MID_SEL = common lwip-contrib wakaama fatfs lwip

# application source dirs
APPL_CSRC_DIR = . ./lwm2m_client ./driver
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = . ./lwm2m_client ./driver

# application defines
APPL_DEFINES = -DLWM2M_CLIENT_MODE

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
