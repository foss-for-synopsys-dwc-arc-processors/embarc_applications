# Application name
APPL ?= freertos_iot_aws_smarthomedemo

# Heap Size Settings
HEAPSZ ?= 1048676
STACKSZ ?= 1048576

# Logging level control in AWS SDK
# LOG_FLAGS += -DENABLE_IOT_DEBUG
LOG_FLAGS += -DENABLE_IOT_INFO
LOG_FLAGS += -DENABLE_IOT_WARN
LOG_FLAGS += -DENABLE_IOT_ERROR

APPL_DEFINES  = $(LOG_FLAGS) -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\"
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

ifeq ($(DEMO_HAVE_OLED),1)
APPL_DEFINES += -DAWSDEMO_HAVE_OLED
MID_SEL += u8glib
endif

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
include $(EMBARC_ROOT)/options/options.mk
