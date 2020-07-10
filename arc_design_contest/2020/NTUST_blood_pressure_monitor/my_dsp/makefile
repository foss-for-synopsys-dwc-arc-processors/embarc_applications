SL=/
SRC=E:/metaware_tool/MetaWare/arc/lib/src
OBJ=obj
MWLIB = $(SRC)
OBJ_DIR=av2em_p$(SL)custom_o
MOBJ_DIR=av2em_p$(SL)custom_o
LIBDSP=1
LIBITUT_CUSTOM=1
LIBPGO=1
AOM=1
CUSTOM_BUILD=1
# Set the path of the C compiler and assembler here if necessary.
# Alternatively, specify on the command-line to override, such as:
#     gmake CC=/path/to/MetaWare/arc/bin/ccac
# Note that the AS setting must refer to a "ccac" driver.
CCAC_EM_BUILD=1
CC=E:/metaware_tool/MetaWare/arc/bin/ccac
AS=$(CCAC)

ifeq ($(CCAC),)
    ifeq ("$(dir $(CC))","./")
        CCAC=ccac
    else
        CCAC=$(dir $(CC))ccac
    endif
endif
ifeq ($(MCC),)
    ifeq ("$(dir $(CC))","./")
        MCC=mcc
    else
        MCC=$(dir $(CC))mcc
    endif
endif

# change cflags and aflags here (or override on the command-line) if you
# wish to build with your own additional options or optimization level:
processor_opt=-av2em
cflags=-av2em -core2 -HL -Hccm -Hfxapi -Hrgf_banked_regs=32 -Xagu_small -Xbarrel_shifter -Xbitscan -Xcode_density -Xdiv_rem=radix2 -Xdmac -Xdsp2 -Xdsp_accshift=full -Xdsp_complex -Xdsp_divsqrt=radix2 -Xdsp_itu -Xfpu_mac -Xfpuda -Xfpus -Xfpus_div -Xfpus_div_slow -Xfpus_mpy_slow -Xmpy_option=mpyd -Xshift_assist -Xstack_check -Xswap -Xtimer0 -Xtimer1 -Xunaligned -Xxy -ffp-contract=off .tcf $(libflags)
aflags=$(cflags)

include  $(SRC)/libcomm.mk
