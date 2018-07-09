# Application name
APPL ?= ${workspaceFolderBasename}

EXT_DEV_LIST += wifi/rw009 sensor/temperature/adt7420

# Heap Size Settings
HEAPSZ ?= 1048576
STACKSZ ?= 1048576

#
# root dir of embARC
#
EMBARC_ROOT = ${ARC_BSP_SRC}

OS_SEL = freertos

MID_SEL = vending-machine common u8glib mbedtls fatfs lwip lwip-contrib

APPL_DEFINES += -DEMSK_PMWIFI_USE_J6

# application source dirs
APPL_CSRC_DIR = ./Source
APPL_ASMSRC_DIR = ./Source

# application include dirs
APPL_INC_DIR = ./Include


# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk