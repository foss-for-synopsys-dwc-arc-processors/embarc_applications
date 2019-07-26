# Application name
APPL ?= freertos_iot_aws_smarthome_secureshield

EXT_DEV_LIST += wifi/mrf24g sensor/temperature/adt7420

# Heap Size Settings
HEAPSZ ?= 1048576
STACKSZ ?= 1048576

# Logging level control in AWS SDK
# LOG_FLAGS += -DENABLE_IOT_DEBUG
LOG_FLAGS += -DENABLE_IOT_INFO
LOG_FLAGS += -DENABLE_IOT_WARN
LOG_FLAGS += -DENABLE_IOT_ERROR

APPL_DEFINES  = $(LOG_FLAGS) -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\" \
				-DTASK_STACK_SIZE_MAIN=4096*2 -DARC_FEATURE_STACK_CHECK=0 \
				-DSECURESHIELD_SECURE_RAM_USAGE=0x1800000
## Define whether demo include OLED
# 1: Yes
# 0: No
##
DEMO_HAVE_OLED ?= 0

#
# root dir of embARC
#
EMBARC_ROOT = ../../..

# Selected OS
OS_SEL ?= freertos
MID_SEL = common fatfs mbedtls lwip lwip-contrib aws
LIB_SEL = secureshield

ifeq ($(DEMO_HAVE_OLED),1)
APPL_DEFINES += -DAWSDEMO_HAVE_OLED
MID_SEL += u8glib
endif

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

# application include dirs
APPL_INC_DIR = . ./secure


# the dir where src files should be compiled and linked with secure world
APPL_SECURE_CSRC_DIR = ./secure
SECURE_SYMBOL_LIST = ./secure/secure_symbol.txt

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
