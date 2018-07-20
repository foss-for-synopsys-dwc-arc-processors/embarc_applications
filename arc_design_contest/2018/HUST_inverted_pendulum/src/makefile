# Application name
APPL ?= invert-pendulum-dsp

# Selected OS
OS_SEL ?= freertos

#
# root dir of embARC
#
EMBARC_ROOT = ../../../../..

MID_SEL = common fatfs

# application source dirs
APPL_CSRC_DIR = . ./TskDbg ./Queue ./TskMotor ./drivers ./TskTop ./FastMath_Table 
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = . ./TskDbg ./Queue ./TskTop ./drivers ./TskMotor ./FastMath_Table 

# include current project makefile 
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
