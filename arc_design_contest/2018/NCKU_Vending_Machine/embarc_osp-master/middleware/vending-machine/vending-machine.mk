# dir declaration
MID_VM_DIR = $(MIDDLEWARES_ROOT)/vending-machine

MID_VM_ASMSRCDIR	= $(MID_VM_DIR)
MID_VM_CSRCDIR		= $(MID_VM_DIR)/Source
MID_VM_INCDIR		= $(MID_VM_DIR)/Include

# find all the source files in the target directories
MID_VM_CSRCS = $(call get_csrcs, $(MID_VM_CSRCDIR))
MID_VM_ASMSRCS = $(call get_asmsrcs, $(MID_VM_ASMSRCDIR))

APPL_DEFINES  += -DNTCONF_EDITOR_MAXLEN=256 -DMBEDTLS_CONFIG_FILE=\"mbedtls_config.h\"

# get object files
MID_VM_COBJS = $(call get_relobjs, $(MID_VM_CSRCS))
MID_VM_ASMOBJS = $(call get_relobjs, $(MID_VM_ASMSRCS))
MID_VM_OBJS = $(MID_VM_COBJS) $(MID_VM_ASMOBJS)

# get dependency files
MID_VM_DEPS = $(call get_deps, $(MID_VM_OBJS))

# extra macros to be defined
MID_VM_DEFINES = -DMID_VM

# genearte library
MID_LIB_VM = $(OUT_DIR)/libmidvm.a

# library generation rule
$(MID_LIB_VM): $(MID_VM_OBJS)
	$(TRACE_ARCHIVE)
	$(Q)$(AR) $(AR_OPT) $@ $(MID_VM_OBJS)

# specific compile rules
# user can add rules to compile this middleware
# if not rules specified to this middleware, it will use default compiling rules

# Middleware Definitions
MID_INCDIR += $(MID_VM_INCDIR)
MID_CSRCDIR += $(MID_VM_CSRCDIR)
MID_ASMSRCDIR += $(MID_VM_ASMSRCDIR)

MID_CSRCS += $(MID_VM_CSRCS)
MID_CXXSRCS +=
MID_ASMSRCS += $(MID_VM_ASMSRCS)
MID_ALLSRCS += $(MID_VM_CSRCS) $(MID_VM_ASMSRCS)

MID_COBJS += $(MID_VM_COBJS)
MID_CXXOBJS +=
MID_ASMOBJS += $(MID_VM_ASMOBJS)
MID_ALLOBJS += $(MID_VM_OBJS)

MID_DEFINES += $(MID_VM_DEFINES)
MID_DEPS += $(MID_VM_DEPS)
MID_LIBS += $(MID_LIB_VM)