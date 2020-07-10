# libmwcl arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hcl -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
CC_INC= -I$(MWLIB)/c/inc/internal -I$(MWLIB)/c/inc
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libmwcl.a
AR_OPT=""

LIST_386em=a5_dcmp.o a5_fcmp.o dcmpIEEE.o fcmpIEEE.o fcmpo.o dcmpo.o add2.o\
	cvtdf.o cvtfd.o dadd.o dcmp.o ddiv.o ddivfpx.o ddivfpuda.o dflt.o\
	dfltllbs.o dmul.o dtrunc.o dtruncllbs.o dtruncu.o fadd.o fcmp.o fdiv.o\
	fflt.o ffltllbs.o fmul.o ftrunc.o ftruncllbs.o ftruncu.o shld.o shrd.o\
	zeros.o zerosll.o dfltll.o dfltull.o dmax.o dtrunll.o dtrunull.o\
	dutil.o ffltll.o ffltull.o ftrunll.o ftrunull.o fmax.o futil.o

LIST_misc=_rterr.o __mw_init_dsp.o __mw_a32_dmpy_nf_v2q15.o\
	__mw_a32_dmac_nf_v2q15.o __mw_a40_dmac_nf_v2q15.o __mw_add_a40.o\
	__mw_add_a40_sd.o __mw_add_a64.o __mw_add_a72.o __mw_add_a72_sd.o\
	__mw_add_v2a32.o __mw_add_v2a40.o __mw_add_v2a40_sd.o __mw_asl_a64.o\
	__mw_get_a64.o __mw_get_a64_limited.o __mw_norm_a64.o __mw_sub_a40.o\
	__mw_sub_a40_sd.o __mw_sub_a64.o __mw_sub_a72.o __mw_sub_a72_sd.o\
	__mw_sub_v2a32.o __mw_sub_v2a40.o __mw_sub_v2a40_sd.o\
	__mw_asr_rnd_up64.o __mw_asr_rnd_conv64.o _ac_init_spe.o\
	_ac_maligned_ld16s.o _ac_maligned_ld16sv.o _ac_maligned_ld16u.o\
	_ac_maligned_ld16uv.o _ac_maligned_ld32.o _ac_maligned_ld32v.o\
	_ac_maligned_ld64.o _ac_maligned_ld64v.o _ac_maligned_st16.o\
	_ac_maligned_st16v.o _ac_maligned_st32.o _ac_maligned_st32v.o\
	_ac_maligned_st64.o _ac_maligned_st64v.o _ac_spe_handler.o _ac_swap.o\
	_ac_swap64.o _disable.o _enable.o _mw_dual_add.o _mw_dual_addu.o\
	_mw_dual_sub.o _mw_dual_subu.o _mw_mpy_32x32y32.o _mw_mpy32_use16.o\
	_mwget_ns_sp.o _mwsll.o _mwsra.o _mwsrl.o _ucdata_end.o ac_chkstk.o\
	ac_mc_13.o ac_mc_va.o arc_sec_timer.o arc_timer.o div.o div0max.o\
	divnorm.o eitab.o jlitab.o sjlitab.o l64x32divnorm.o labs.o lditab.o\
	ldiv.o ldivnorm.o lmax.o lmaxu.o lmin.o lminu.o lmpy.o lmpympy.o lsll.o\
	lsllbs.o lsra.o lsrabs.o lsrl.o lsrlbs.o mcarg.o mwgetbits.o\
	mwgetbitu.o mwgetubits.o mwgetubitu.o mwinsert.o mwmgetbits.o\
	mwmgetbitu.o mwmgetubits.o mwmgetubitu.o mwmsetbits.o mwmsetbitu.o\
	mwmsetubits.o mwmsetubitu.o mwsetbits.o mwsetbitu.o mwsetubits.o\
	mwsetubitu.o _mw_agu_bitrev.o _mw_agu_circ_inc.o _mw_agu_circ_dec.o\
	_mw_aslacc.o _mw_aslsacc.o _mw_getacc.o errnog.o mutexg.o mwclrsb64.o\
	mwclrsb.o mwdctor.o ashlti3.o lshrti3.o mwa40tod.o mwa72tod.o\
	mwa40tof.o mwa72tof.o mwdtoa40.o mwdtoa72.o mwdtoq.o mwftoa40.o\
	mwftoa72.o mwftoq.o mwqtod.o mwqtof.o mwmpya40.o mwmpya40pa.o\
	mwmpya64.o mwmpya64pa.o mwmpya72.o mwmpya72pa.o profclk.o tlsctorg.o\
	__calc_iv_s1.o __calc_iv_u1.o __calc_iv_s2.o __calc_iv_u2.o\
	__calc_iv_s4.o __calc_iv_u4.o __calc_iv_s8.o __calc_iv_u8.o sync_sync.o\
	sync_add_1.o sync_add_2.o sync_add_4.o sync_add_8.o sync_and_1.o\
	sync_and_2.o sync_and_4.o sync_and_8.o sync_nand_1.o sync_nand_2.o\
	sync_nand_4.o sync_nand_8.o sync_or_1.o sync_or_2.o sync_or_4.o\
	sync_or_8.o sync_sub_1.o sync_sub_2.o sync_sub_4.o sync_sub_8.o\
	sync_xor_1.o sync_xor_2.o sync_xor_4.o sync_xor_8.o sync_compare_swap.o\
	sync_compare_swap_1.o sync_compare_swap_2.o atomic_compare_exchange_8.o\
	atomic_compare_exchange_4.o atomic_compare_exchange_2.o\
	atomic_compare_exchange_1.o atomic_exchange_8.o atomic_exchange_4.o\
	atomic_exchange_2.o atomic_exchange_1.o atomic_is_lock_free.o\
	atomic_store_8.o atomic_store_4.o atomic_store_2.o atomic_store_1.o\
	atomic_load_8.o atomic_load_4.o atomic_load_2.o atomic_load_1.o

LIST_startup=_init_ad.o _init_ei.o _init_jli.o _init_ldi.o _init_sjli.o\
	mwhalt.o runctor.o rundtor.o xcheck.o

LIST_extra_objs=div0halt.o

LIST_std=_strrev.o _ultoa.o

LIST_str=_vdmemcpy.o _vsdmemcpy.o _vsmemcpy.o _vdmemcpy_qa.o _vsdmemcpy_qa.o\
	_vsmemcpy_qa.o _vdmemcpy_wa.o _vsdmemcpy_wa.o _vsmemcpy_wa.o\
	_vmemset_qa.o _vmemset_wa.o memcpy.o memcpy_qa.o memcpy_wa.o\
	memset_qa.o memset_wa.o _memclr.o _vdmemmove.o _vmemset.o _vsdmemmove.o\
	_vsmemmove.o memset.o strcpy.o strlen.o strncmp.o memchr.o memcmp.o\
	memmove.o

LIST_stub=_exit.o access.o argv.o clock.o close.o getcwd.o getenv.o getpid.o\
	isatty.o lseek.o open.o read.o rename.o time.o tmpnam.o unlink.o\
	write.o

LIST_hl=environ.o remove.o sbrk.o

LIST_mw=startk.o

LIST_llvm=comparedf2.o comparesf2.o extendhfsf2.o stackchk.o truncdfhf2.o\
	truncsfhf2.o init.o fini.o

LIST_complex=_muldc3.o _mulsc3.o _divdc3.o _divsc3.o

LIST_ALL=$(LIST_386em) $(LIST_misc) $(LIST_startup) $(LIST_std) $(LIST_str) $(LIST_stub) $(LIST_hl) $(LIST_mw) $(LIST_llvm) $(LIST_complex)

all:  extra_objs $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_386em)
	$(AR) rv $(AR_NAME) $(LIST_misc)
	$(AR) rv $(AR_NAME) $(LIST_startup)
	$(AR) rv $(AR_NAME) $(LIST_std)
	$(AR) rv $(AR_NAME) $(LIST_str)
	$(AR) rv $(AR_NAME) $(LIST_stub)
	$(AR) rv $(AR_NAME) $(LIST_hl)
	$(AR) rv $(AR_NAME) $(LIST_mw)
	$(AR) rv $(AR_NAME) $(LIST_llvm)
	$(AR) rv $(AR_NAME) $(LIST_complex)

386em: $(LIST_386em)
misc: $(LIST_misc)
startup: $(LIST_startup)
extra_objs: $(LIST_extra_objs)
std: $(LIST_std)
str: $(LIST_str)
stub: $(LIST_stub)
hl: $(LIST_hl)
mw: $(LIST_mw)
llvm: $(LIST_llvm)
complex: $(LIST_complex)

a5_dcmp.o: $(MWLIB)/mwcl/386em_g/a5_dcmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/a5_dcmp.s

a5_fcmp.o: $(MWLIB)/mwcl/386em_g/a5_fcmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/a5_fcmp.s

dcmpIEEE.o: $(MWLIB)/mwcl/386em_g/dcmpIEEE.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dcmpIEEE.s

fcmpIEEE.o: $(MWLIB)/mwcl/386em_g/fcmpIEEE.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fcmpIEEE.s

fcmpo.o: $(MWLIB)/mwcl/386em_g/fcmpo.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fcmpo.s

dcmpo.o: $(MWLIB)/mwcl/386em_g/dcmpo.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dcmpo.s

add2.o: $(MWLIB)/mwcl/386em_g/add2.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/add2.s

cvtdf.o: $(MWLIB)/mwcl/386em_g/cvtdf.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/cvtdf.s

cvtfd.o: $(MWLIB)/mwcl/386em_g/cvtfd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/cvtfd.s

dadd.o: $(MWLIB)/mwcl/386em_g/dadd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dadd.s

dcmp.o: $(MWLIB)/mwcl/386em_g/dcmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dcmp.s

ddiv.o: $(MWLIB)/mwcl/386em_g/ddiv.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ddiv.s

ddivfpx.o: $(MWLIB)/mwcl/386em_g/ddivfpx.s
	$(AS) $(AS_OPT) $(aflags) -Xdpfp -Xbarrel_shifter -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ddivfpx.s

ddivfpuda.o: $(MWLIB)/mwcl/386em_g/ddivfpuda.s
	$(AS) $(AS_OPT) $(aflags) -Xfpuda -Xbarrel_shifter -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ddivfpuda.s

dflt.o: $(MWLIB)/mwcl/386em_g/dflt.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dflt.s

dfltllbs.o: $(MWLIB)/mwcl/386em_g/dfltllbs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dfltllbs.s

dmul.o: $(MWLIB)/mwcl/386em_g/dmul.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dmul.s

dtrunc.o: $(MWLIB)/mwcl/386em_g/dtrunc.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dtrunc.s

dtruncllbs.o: $(MWLIB)/mwcl/386em_g/dtruncllbs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dtruncllbs.s

dtruncu.o: $(MWLIB)/mwcl/386em_g/dtruncu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/dtruncu.s

fadd.o: $(MWLIB)/mwcl/386em_g/fadd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fadd.s

fcmp.o: $(MWLIB)/mwcl/386em_g/fcmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fcmp.s

fdiv.o: $(MWLIB)/mwcl/386em_g/fdiv.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fdiv.s

fflt.o: $(MWLIB)/mwcl/386em_g/fflt.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fflt.s

ffltllbs.o: $(MWLIB)/mwcl/386em_g/ffltllbs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ffltllbs.s

fmul.o: $(MWLIB)/mwcl/386em_g/fmul.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/fmul.s

ftrunc.o: $(MWLIB)/mwcl/386em_g/ftrunc.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ftrunc.s

ftruncllbs.o: $(MWLIB)/mwcl/386em_g/ftruncllbs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ftruncllbs.s

ftruncu.o: $(MWLIB)/mwcl/386em_g/ftruncu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/ftruncu.s

shld.o: $(MWLIB)/mwcl/386em_g/shld.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/shld.s

shrd.o: $(MWLIB)/mwcl/386em_g/shrd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/shrd.s

zeros.o: $(MWLIB)/mwcl/386em_g/zeros.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/zeros.s

zerosll.o: $(MWLIB)/mwcl/386em_g/zerosll.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/386em_g/zerosll.s

dfltll.o: $(MWLIB)/mwcl/386em_g/dfltll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/dfltll.c

dfltull.o: $(MWLIB)/mwcl/386em_g/dfltull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/dfltull.c

dmax.o: $(MWLIB)/mwcl/386em_g/dmax.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/dmax.c

dtrunll.o: $(MWLIB)/mwcl/386em_g/dtrunll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/dtrunll.c

dtrunull.o: $(MWLIB)/mwcl/386em_g/dtrunull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/dtrunull.c

dutil.o: $(MWLIB)/mwcl/386em_g/dutil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/dutil.c

ffltll.o: $(MWLIB)/mwcl/386em_g/ffltll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/ffltll.c

ffltull.o: $(MWLIB)/mwcl/386em_g/ffltull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/ffltull.c

ftrunll.o: $(MWLIB)/mwcl/386em_g/ftrunll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/ftrunll.c

ftrunull.o: $(MWLIB)/mwcl/386em_g/ftrunull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/ftrunull.c

fmax.o: $(MWLIB)/mwcl/386em_g/fmax.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/fmax.c

futil.o: $(MWLIB)/mwcl/386em_g/futil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/386em_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/386em_g/futil.c

_rterr.o: $(MWLIB)/mwcl/misc_g/_rterr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_rterr.c

__mw_init_dsp.o: $(MWLIB)/mwcl/misc_g/__mw_init_dsp.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hfxapi -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/__mw_init_dsp.c

__mw_a32_dmpy_nf_v2q15.o: $(MWLIB)/mwcl/misc_g/__mw_a32_dmpy_nf_v2q15.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_a32_dmpy_nf_v2q15.s

__mw_a32_dmac_nf_v2q15.o: $(MWLIB)/mwcl/misc_g/__mw_a32_dmac_nf_v2q15.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_a32_dmac_nf_v2q15.s

__mw_a40_dmac_nf_v2q15.o: $(MWLIB)/mwcl/misc_g/__mw_a40_dmac_nf_v2q15.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_a40_dmac_nf_v2q15.s

__mw_add_a40.o: $(MWLIB)/mwcl/misc_g/__mw_add_a40.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_a40.s

__mw_add_a40_sd.o: $(MWLIB)/mwcl/misc_g/__mw_add_a40_sd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_a40_sd.s

__mw_add_a64.o: $(MWLIB)/mwcl/misc_g/__mw_add_a64.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_a64.s

__mw_add_a72.o: $(MWLIB)/mwcl/misc_g/__mw_add_a72.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_a72.s

__mw_add_a72_sd.o: $(MWLIB)/mwcl/misc_g/__mw_add_a72_sd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_a72_sd.s

__mw_add_v2a32.o: $(MWLIB)/mwcl/misc_g/__mw_add_v2a32.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_v2a32.s

__mw_add_v2a40.o: $(MWLIB)/mwcl/misc_g/__mw_add_v2a40.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_v2a40.s

__mw_add_v2a40_sd.o: $(MWLIB)/mwcl/misc_g/__mw_add_v2a40_sd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_add_v2a40_sd.s

__mw_asl_a64.o: $(MWLIB)/mwcl/misc_g/__mw_asl_a64.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_asl_a64.s

__mw_get_a64.o: $(MWLIB)/mwcl/misc_g/__mw_get_a64.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_get_a64.s

__mw_get_a64_limited.o: $(MWLIB)/mwcl/misc_g/__mw_get_a64_limited.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_get_a64_limited.s

__mw_norm_a64.o: $(MWLIB)/mwcl/misc_g/__mw_norm_a64.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_norm_a64.s

__mw_sub_a40.o: $(MWLIB)/mwcl/misc_g/__mw_sub_a40.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_a40.s

__mw_sub_a40_sd.o: $(MWLIB)/mwcl/misc_g/__mw_sub_a40_sd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_a40_sd.s

__mw_sub_a64.o: $(MWLIB)/mwcl/misc_g/__mw_sub_a64.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_a64.s

__mw_sub_a72.o: $(MWLIB)/mwcl/misc_g/__mw_sub_a72.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_a72.s

__mw_sub_a72_sd.o: $(MWLIB)/mwcl/misc_g/__mw_sub_a72_sd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_a72_sd.s

__mw_sub_v2a32.o: $(MWLIB)/mwcl/misc_g/__mw_sub_v2a32.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_v2a32.s

__mw_sub_v2a40.o: $(MWLIB)/mwcl/misc_g/__mw_sub_v2a40.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_v2a40.s

__mw_sub_v2a40_sd.o: $(MWLIB)/mwcl/misc_g/__mw_sub_v2a40_sd.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_sub_v2a40_sd.s

__mw_asr_rnd_up64.o: $(MWLIB)/mwcl/misc_g/__mw_asr_rnd_up64.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_asr_rnd_up64.s

__mw_asr_rnd_conv64.o: $(MWLIB)/mwcl/misc_g/__mw_asr_rnd_conv64.s
	$(AS) $(AS_OPT) -Xdsp $(aflags) -Hon=incompat_option_warn $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/__mw_asr_rnd_conv64.s

_init_ad.o: $(MWLIB)/mwcl/startup_g/_init_ad.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/_init_ad.s

_init_ei.o: $(MWLIB)/mwcl/startup_g/_init_ei.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/_init_ei.s

_init_jli.o: $(MWLIB)/mwcl/startup_g/_init_jli.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/_init_jli.s

_init_ldi.o: $(MWLIB)/mwcl/startup_g/_init_ldi.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/_init_ldi.s

_init_sjli.o: $(MWLIB)/mwcl/startup_g/_init_sjli.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/_init_sjli.s

_ac_init_spe.o: $(MWLIB)/mwcl/misc_g/_ac_init_spe.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_init_spe.c

_ac_maligned_ld16s.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16s.c

_ac_maligned_ld16sv.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16sv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16sv.c

_ac_maligned_ld16u.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16u.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16u.c

_ac_maligned_ld16uv.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16uv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld16uv.c

_ac_maligned_ld32.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld32.c

_ac_maligned_ld32v.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld32v.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld32v.c

_ac_maligned_ld64.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld64.c

_ac_maligned_ld64v.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_ld64v.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_ld64v.c

_ac_maligned_st16.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_st16.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_st16.c

_ac_maligned_st16v.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_st16v.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_st16v.c

_ac_maligned_st32.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_st32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_st32.c

_ac_maligned_st32v.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_st32v.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_st32v.c

_ac_maligned_st64.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_st64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_st64.c

_ac_maligned_st64v.o: $(MWLIB)/mwcl/misc_g/_ac_maligned_st64v.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_maligned_st64v.c

_ac_spe_handler.o: $(MWLIB)/mwcl/misc_g/_ac_spe_handler.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_ac_spe_handler.s

_ac_swap.o: $(MWLIB)/mwcl/misc_g/_ac_swap.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_swap.c

_ac_swap64.o: $(MWLIB)/mwcl/misc_g/_ac_swap64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_ac_swap64.c

_disable.o: $(MWLIB)/mwcl/misc_g/_disable.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_disable.s

_enable.o: $(MWLIB)/mwcl/misc_g/_enable.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_enable.s

_mw_dual_add.o: $(MWLIB)/mwcl/misc_g/_mw_dual_add.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mw_dual_add.s

_mw_dual_addu.o: $(MWLIB)/mwcl/misc_g/_mw_dual_addu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mw_dual_addu.s

_mw_dual_sub.o: $(MWLIB)/mwcl/misc_g/_mw_dual_sub.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mw_dual_sub.s

_mw_dual_subu.o: $(MWLIB)/mwcl/misc_g/_mw_dual_subu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mw_dual_subu.s

_mw_mpy_32x32y32.o: $(MWLIB)/mwcl/misc_g/_mw_mpy_32x32y32.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mw_mpy_32x32y32.s

_mw_mpy32_use16.o: $(MWLIB)/mwcl/misc_g/_mw_mpy32_use16.s
	$(AS) $(AS_OPT) $(aflags) -Xmpy16 $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mw_mpy32_use16.s

_mwget_ns_sp.o: $(MWLIB)/mwcl/misc_g/_mwget_ns_sp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mwget_ns_sp.s

_mwsll.o: $(MWLIB)/mwcl/misc_g/_mwsll.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mwsll.s

_mwsra.o: $(MWLIB)/mwcl/misc_g/_mwsra.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mwsra.s

_mwsrl.o: $(MWLIB)/mwcl/misc_g/_mwsrl.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_mwsrl.s

_ucdata_end.o: $(MWLIB)/mwcl/misc_g/_ucdata_end.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/_ucdata_end.s

ac_chkstk.o: $(MWLIB)/mwcl/misc_g/ac_chkstk.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/ac_chkstk.s

ac_mc_13.o: $(MWLIB)/mwcl/misc_g/ac_mc_13.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/ac_mc_13.s

ac_mc_va.o: $(MWLIB)/mwcl/misc_g/ac_mc_va.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/ac_mc_va.s

arc_sec_timer.o: $(MWLIB)/mwcl/misc_g/arc_sec_timer.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Xsec_modes $(MWLIB)/mwcl/misc_g/arc_sec_timer.c

arc_timer.o: $(MWLIB)/mwcl/misc_g/arc_timer.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/arc_timer.c

div.o: $(MWLIB)/mwcl/misc_g/div.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/div.s

div0halt.o: $(MWLIB)/mwcl/misc_g/div0halt.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/div0halt.s

div0max.o: $(MWLIB)/mwcl/misc_g/div0max.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/div0max.s

divnorm.o: $(MWLIB)/mwcl/misc_g/divnorm.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter -Xnorm $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/divnorm.s

eitab.o: $(MWLIB)/mwcl/misc_g/eitab.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/eitab.s

jlitab.o: $(MWLIB)/mwcl/misc_g/jlitab.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/jlitab.s

sjlitab.o: $(MWLIB)/mwcl/misc_g/sjlitab.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/sjlitab.s

l64x32divnorm.o: $(MWLIB)/mwcl/misc_g/l64x32divnorm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Xnorm -Xbarrel_shifter -Xmpy $(MWLIB)/mwcl/misc_g/l64x32divnorm.c

labs.o: $(MWLIB)/mwcl/misc_g/labs.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/labs.s

lditab.o: $(MWLIB)/mwcl/misc_g/lditab.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lditab.s

ldiv.o: $(MWLIB)/mwcl/misc_g/ldiv.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/ldiv.s

ldivnorm.o: $(MWLIB)/mwcl/misc_g/ldivnorm.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter -Xnorm $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/ldivnorm.s

lmax.o: $(MWLIB)/mwcl/misc_g/lmax.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lmax.s

lmaxu.o: $(MWLIB)/mwcl/misc_g/lmaxu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lmaxu.s

lmin.o: $(MWLIB)/mwcl/misc_g/lmin.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lmin.s

lminu.o: $(MWLIB)/mwcl/misc_g/lminu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lminu.s

lmpy.o: $(MWLIB)/mwcl/misc_g/lmpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lmpy.s

lmpympy.o: $(MWLIB)/mwcl/misc_g/lmpympy.s
	$(AS) $(AS_OPT) $(aflags) -Xmpy $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lmpympy.s

lsll.o: $(MWLIB)/mwcl/misc_g/lsll.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lsll.s

lsllbs.o: $(MWLIB)/mwcl/misc_g/lsllbs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lsllbs.s

lsra.o: $(MWLIB)/mwcl/misc_g/lsra.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lsra.s

lsrabs.o: $(MWLIB)/mwcl/misc_g/lsrabs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lsrabs.s

lsrl.o: $(MWLIB)/mwcl/misc_g/lsrl.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lsrl.s

lsrlbs.o: $(MWLIB)/mwcl/misc_g/lsrlbs.s
	$(AS) $(AS_OPT) $(aflags) -Xbarrel_shifter $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/lsrlbs.s

mcarg.o: $(MWLIB)/mwcl/misc_g/mcarg.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/mcarg.c

mwgetbits.o: $(MWLIB)/mwcl/misc_g/mwgetbits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwgetbits.s

mwgetbitu.o: $(MWLIB)/mwcl/misc_g/mwgetbitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwgetbitu.s

mwgetubits.o: $(MWLIB)/mwcl/misc_g/mwgetubits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwgetubits.s

mwgetubitu.o: $(MWLIB)/mwcl/misc_g/mwgetubitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwgetubitu.s

mwinsert.o: $(MWLIB)/mwcl/misc_g/mwinsert.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwinsert.s

mwmgetbits.o: $(MWLIB)/mwcl/misc_g/mwmgetbits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmgetbits.s

mwmgetbitu.o: $(MWLIB)/mwcl/misc_g/mwmgetbitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmgetbitu.s

mwmgetubits.o: $(MWLIB)/mwcl/misc_g/mwmgetubits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmgetubits.s

mwmgetubitu.o: $(MWLIB)/mwcl/misc_g/mwmgetubitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmgetubitu.s

mwmsetbits.o: $(MWLIB)/mwcl/misc_g/mwmsetbits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmsetbits.s

mwmsetbitu.o: $(MWLIB)/mwcl/misc_g/mwmsetbitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmsetbitu.s

mwmsetubits.o: $(MWLIB)/mwcl/misc_g/mwmsetubits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmsetubits.s

mwmsetubitu.o: $(MWLIB)/mwcl/misc_g/mwmsetubitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwmsetubitu.s

mwsetbits.o: $(MWLIB)/mwcl/misc_g/mwsetbits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwsetbits.s

mwsetbitu.o: $(MWLIB)/mwcl/misc_g/mwsetbitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwsetbitu.s

mwsetubits.o: $(MWLIB)/mwcl/misc_g/mwsetubits.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwsetubits.s

mwsetubitu.o: $(MWLIB)/mwcl/misc_g/mwsetubitu.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/misc_g/mwsetubitu.s

_mw_agu_bitrev.o: $(MWLIB)/mwcl/misc_g/_mw_agu_bitrev.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_mw_agu_bitrev.c

_mw_agu_circ_inc.o: $(MWLIB)/mwcl/misc_g/_mw_agu_circ_inc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_mw_agu_circ_inc.c

_mw_agu_circ_dec.o: $(MWLIB)/mwcl/misc_g/_mw_agu_circ_dec.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/_mw_agu_circ_dec.c

_mw_aslacc.o: $(MWLIB)/mwcl/misc_g/_mw_aslacc.c
	$(CCAC) $(CC_OPT) -Xdsp2 -Xdsp_accshift=limited $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/_mw_aslacc.c

_mw_aslsacc.o: $(MWLIB)/mwcl/misc_g/_mw_aslsacc.c
	$(CCAC) $(CC_OPT) -Xdsp2 -Xdsp_accshift=limited $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/_mw_aslsacc.c

_mw_getacc.o: $(MWLIB)/mwcl/misc_g/_mw_getacc.c
	$(CCAC) $(CC_OPT) -Xdsp2 -Xdsp_accshift=limited $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/_mw_getacc.c

errnog.o: $(MWLIB)/mwcl/misc_g/errnog.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/errnog.c

mutexg.o: $(MWLIB)/mwcl/misc_g/mutexg.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/mutexg.c

mwclrsb64.o: $(MWLIB)/mwcl/misc_g/mwclrsb64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/mwclrsb64.c

mwclrsb.o: $(MWLIB)/mwcl/misc_g/mwclrsb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/mwclrsb.c

mwdctor.o: $(MWLIB)/mwcl/misc_g/mwdctor.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/mwdctor.c

ashlti3.o: $(MWLIB)/mwcl/misc_g/ashlti3.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/ashlti3.c

lshrti3.o: $(MWLIB)/mwcl/misc_g/lshrti3.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/lshrti3.c

mwa40tod.o: $(MWLIB)/mwcl/misc_g/mwa40tod.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwa40tod.c

mwa72tod.o: $(MWLIB)/mwcl/misc_g/mwa72tod.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwa72tod.c

mwa40tof.o: $(MWLIB)/mwcl/misc_g/mwa40tof.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwa40tof.c

mwa72tof.o: $(MWLIB)/mwcl/misc_g/mwa72tof.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwa72tof.c

mwdtoa40.o: $(MWLIB)/mwcl/misc_g/mwdtoa40.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwdtoa40.c

mwdtoa72.o: $(MWLIB)/mwcl/misc_g/mwdtoa72.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwdtoa72.c

mwdtoq.o: $(MWLIB)/mwcl/misc_g/mwdtoq.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwdtoq.c

mwftoa40.o: $(MWLIB)/mwcl/misc_g/mwftoa40.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwftoa40.c

mwftoa72.o: $(MWLIB)/mwcl/misc_g/mwftoa72.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwftoa72.c

mwftoq.o: $(MWLIB)/mwcl/misc_g/mwftoq.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwftoq.c

mwqtod.o: $(MWLIB)/mwcl/misc_g/mwqtod.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwqtod.c

mwqtof.o: $(MWLIB)/mwcl/misc_g/mwqtof.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwqtof.c

mwmpya40.o: $(MWLIB)/mwcl/misc_g/mwmpya40.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwmpya40.c

mwmpya40pa.o: $(MWLIB)/mwcl/misc_g/mwmpya40pa.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwmpya40pa.c

mwmpya64.o: $(MWLIB)/mwcl/misc_g/mwmpya64.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwmpya64.c

mwmpya64pa.o: $(MWLIB)/mwcl/misc_g/mwmpya64pa.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwmpya64pa.c

mwmpya72.o: $(MWLIB)/mwcl/misc_g/mwmpya72.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwmpya72.c

mwmpya72pa.o: $(MWLIB)/mwcl/misc_g/mwmpya72pa.c
	$(CCAC) $(CC_OPT) -Xdsp $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) -Hon=incompat_option_warn $(MWLIB)/mwcl/misc_g/mwmpya72pa.c

profclk.o: $(MWLIB)/mwcl/misc_g/profclk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/profclk.c

tlsctorg.o: $(MWLIB)/mwcl/misc_g/tlsctorg.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/tlsctorg.c

mwhalt.o: $(MWLIB)/mwcl/startup_g/mwhalt.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/mwhalt.s

runctor.o: $(MWLIB)/mwcl/startup_g/runctor.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/runctor.s

rundtor.o: $(MWLIB)/mwcl/startup_g/rundtor.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/rundtor.s

xcheck.o: $(MWLIB)/mwcl/startup_g/xcheck.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/startup_g/xcheck.s

_strrev.o: $(MWLIB)/mwcl/std_g/_strrev.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/std_g/_strrev.c

_ultoa.o: $(MWLIB)/mwcl/std_g/_ultoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/std_g/_ultoa.c

_vdmemcpy.o: $(MWLIB)/mwcl/str_g/_vdmemcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vdmemcpy.s

_vsdmemcpy.o: $(MWLIB)/mwcl/str_g/_vsdmemcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vsdmemcpy.s

_vsmemcpy.o: $(MWLIB)/mwcl/str_g/_vsmemcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vsmemcpy.s

_vdmemcpy_qa.o: $(MWLIB)/mwcl/str_g/_vdmemcpy_qa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vdmemcpy_qa.s

_vsdmemcpy_qa.o: $(MWLIB)/mwcl/str_g/_vsdmemcpy_qa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vsdmemcpy_qa.s

_vsmemcpy_qa.o: $(MWLIB)/mwcl/str_g/_vsmemcpy_qa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vsmemcpy_qa.s

_vdmemcpy_wa.o: $(MWLIB)/mwcl/str_g/_vdmemcpy_wa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vdmemcpy_wa.s

_vsdmemcpy_wa.o: $(MWLIB)/mwcl/str_g/_vsdmemcpy_wa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vsdmemcpy_wa.s

_vsmemcpy_wa.o: $(MWLIB)/mwcl/str_g/_vsmemcpy_wa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vsmemcpy_wa.s

_vmemset_qa.o: $(MWLIB)/mwcl/str_g/_vmemset_qa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vmemset_qa.s

_vmemset_wa.o: $(MWLIB)/mwcl/str_g/_vmemset_wa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/_vmemset_wa.s

memcpy.o: $(MWLIB)/mwcl/str_g/memcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/memcpy.c

memcpy_qa.o: $(MWLIB)/mwcl/str_g/memcpy_qa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/memcpy_qa.s

memcpy_wa.o: $(MWLIB)/mwcl/str_g/memcpy_wa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/memcpy_wa.s

memset_qa.o: $(MWLIB)/mwcl/str_g/memset_qa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/memset_qa.s

memset_wa.o: $(MWLIB)/mwcl/str_g/memset_wa.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/mwcl/str_g/memset_wa.s

_memclr.o: $(MWLIB)/mwcl/str_g/_memclr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/_memclr.c

_vdmemmove.o: $(MWLIB)/mwcl/str_g/_vdmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/_vdmemmove.c

_vmemset.o: $(MWLIB)/mwcl/str_g/_vmemset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/_vmemset.c

_vsdmemmove.o: $(MWLIB)/mwcl/str_g/_vsdmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/_vsdmemmove.c

_vsmemmove.o: $(MWLIB)/mwcl/str_g/_vsmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/_vsmemmove.c

memset.o: $(MWLIB)/mwcl/str_g/memset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/memset.c

strcpy.o: $(MWLIB)/mwcl/str_g/strcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/strcpy.c

strlen.o: $(MWLIB)/mwcl/str_g/strlen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/strlen.c

strncmp.o: $(MWLIB)/mwcl/str_g/strncmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/strncmp.c

memchr.o: $(MWLIB)/mwcl/str_g/memchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/memchr.c

memcmp.o: $(MWLIB)/mwcl/str_g/memcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/memcmp.c

memmove.o: $(MWLIB)/mwcl/str_g/memmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/str_g/memmove.c

_exit.o: $(MWLIB)/mwcl/stub_g/_exit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/_exit.c

access.o: $(MWLIB)/mwcl/stub_g/access.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/access.c

argv.o: $(MWLIB)/mwcl/stub_g/argv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/argv.c

clock.o: $(MWLIB)/mwcl/stub_g/clock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/clock.c

close.o: $(MWLIB)/mwcl/stub_g/close.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/close.c

getcwd.o: $(MWLIB)/mwcl/stub_g/getcwd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/getcwd.c

getenv.o: $(MWLIB)/mwcl/stub_g/getenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/getenv.c

getpid.o: $(MWLIB)/mwcl/stub_g/getpid.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/getpid.c

isatty.o: $(MWLIB)/mwcl/stub_g/isatty.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/isatty.c

lseek.o: $(MWLIB)/mwcl/stub_g/lseek.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/lseek.c

open.o: $(MWLIB)/mwcl/stub_g/open.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/open.c

read.o: $(MWLIB)/mwcl/stub_g/read.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/read.c

environ.o: $(MWLIB)/mwcl/hl_g/environ.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/hl_g/environ.c

remove.o: $(MWLIB)/mwcl/hl_g/remove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/hl_g/remove.c

rename.o: $(MWLIB)/mwcl/stub_g/rename.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/rename.c

sbrk.o: $(MWLIB)/mwcl/hl_g/sbrk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/hl_g/sbrk.c

time.o: $(MWLIB)/mwcl/stub_g/time.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/time.c

tmpnam.o: $(MWLIB)/mwcl/stub_g/tmpnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/tmpnam.c

unlink.o: $(MWLIB)/mwcl/stub_g/unlink.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/unlink.c

write.o: $(MWLIB)/mwcl/stub_g/write.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/stub_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/stub_g/write.c

startk.o: $(MWLIB)/mwcl/mw_g/startk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/mw_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/mw_g/startk.c

__calc_iv_s1.o: $(MWLIB)/mwcl/misc_g/__calc_iv_s1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_s1.c

__calc_iv_u1.o: $(MWLIB)/mwcl/misc_g/__calc_iv_u1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_u1.c

__calc_iv_s2.o: $(MWLIB)/mwcl/misc_g/__calc_iv_s2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_s2.c

__calc_iv_u2.o: $(MWLIB)/mwcl/misc_g/__calc_iv_u2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_u2.c

__calc_iv_s4.o: $(MWLIB)/mwcl/misc_g/__calc_iv_s4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_s4.c

__calc_iv_u4.o: $(MWLIB)/mwcl/misc_g/__calc_iv_u4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_u4.c

__calc_iv_s8.o: $(MWLIB)/mwcl/misc_g/__calc_iv_s8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_s8.c

__calc_iv_u8.o: $(MWLIB)/mwcl/misc_g/__calc_iv_u8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/__calc_iv_u8.c

comparedf2.o: $(MWLIB)/mwcl/llvm_g/comparedf2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/comparedf2.c

comparesf2.o: $(MWLIB)/mwcl/llvm_g/comparesf2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/comparesf2.c

extendhfsf2.o: $(MWLIB)/mwcl/llvm_g/extendhfsf2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/extendhfsf2.c

stackchk.o: $(MWLIB)/mwcl/llvm_g/stackchk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/stackchk.c

truncdfhf2.o: $(MWLIB)/mwcl/llvm_g/truncdfhf2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/truncdfhf2.c

truncsfhf2.o: $(MWLIB)/mwcl/llvm_g/truncsfhf2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/truncsfhf2.c

init.o: $(MWLIB)/mwcl/llvm_g/init.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/init.c

fini.o: $(MWLIB)/mwcl/llvm_g/fini.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/llvm_g/fini.c

_muldc3.o: $(MWLIB)/mwcl/complex_g/_muldc3.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/complex_g/_muldc3.c

_mulsc3.o: $(MWLIB)/mwcl/complex_g/_mulsc3.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/complex_g/_mulsc3.c

_divdc3.o: $(MWLIB)/mwcl/complex_g/_divdc3.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/complex_g/_divdc3.c

_divsc3.o: $(MWLIB)/mwcl/complex_g/_divsc3.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/complex_g/_divsc3.c

sync_sync.o: $(MWLIB)/mwcl/misc_g/sync_sync.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_sync.c

sync_add_1.o: $(MWLIB)/mwcl/misc_g/sync_add_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_add_1.c

sync_add_2.o: $(MWLIB)/mwcl/misc_g/sync_add_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_add_2.c

sync_add_4.o: $(MWLIB)/mwcl/misc_g/sync_add_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_add_4.c

sync_add_8.o: $(MWLIB)/mwcl/misc_g/sync_add_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_add_8.c

sync_and_1.o: $(MWLIB)/mwcl/misc_g/sync_and_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_and_1.c

sync_and_2.o: $(MWLIB)/mwcl/misc_g/sync_and_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_and_2.c

sync_and_4.o: $(MWLIB)/mwcl/misc_g/sync_and_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_and_4.c

sync_and_8.o: $(MWLIB)/mwcl/misc_g/sync_and_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_and_8.c

sync_nand_1.o: $(MWLIB)/mwcl/misc_g/sync_nand_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_nand_1.c

sync_nand_2.o: $(MWLIB)/mwcl/misc_g/sync_nand_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_nand_2.c

sync_nand_4.o: $(MWLIB)/mwcl/misc_g/sync_nand_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_nand_4.c

sync_nand_8.o: $(MWLIB)/mwcl/misc_g/sync_nand_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_nand_8.c

sync_or_1.o: $(MWLIB)/mwcl/misc_g/sync_or_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_or_1.c

sync_or_2.o: $(MWLIB)/mwcl/misc_g/sync_or_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_or_2.c

sync_or_4.o: $(MWLIB)/mwcl/misc_g/sync_or_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_or_4.c

sync_or_8.o: $(MWLIB)/mwcl/misc_g/sync_or_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_or_8.c

sync_sub_1.o: $(MWLIB)/mwcl/misc_g/sync_sub_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_sub_1.c

sync_sub_2.o: $(MWLIB)/mwcl/misc_g/sync_sub_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_sub_2.c

sync_sub_4.o: $(MWLIB)/mwcl/misc_g/sync_sub_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_sub_4.c

sync_sub_8.o: $(MWLIB)/mwcl/misc_g/sync_sub_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_sub_8.c

sync_xor_1.o: $(MWLIB)/mwcl/misc_g/sync_xor_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_xor_1.c

sync_xor_2.o: $(MWLIB)/mwcl/misc_g/sync_xor_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_xor_2.c

sync_xor_4.o: $(MWLIB)/mwcl/misc_g/sync_xor_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_xor_4.c

sync_xor_8.o: $(MWLIB)/mwcl/misc_g/sync_xor_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_xor_8.c

sync_compare_swap.o: $(MWLIB)/mwcl/misc_g/sync_compare_swap.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_compare_swap.c

sync_compare_swap_1.o: $(MWLIB)/mwcl/misc_g/sync_compare_swap_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_compare_swap_1.c

sync_compare_swap_2.o: $(MWLIB)/mwcl/misc_g/sync_compare_swap_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/sync_compare_swap_2.c

atomic_compare_exchange_8.o: $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_8.c

atomic_compare_exchange_4.o: $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_4.c

atomic_compare_exchange_2.o: $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_2.c

atomic_compare_exchange_1.o: $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_compare_exchange_1.c

atomic_exchange_8.o: $(MWLIB)/mwcl/misc_g/atomic_exchange_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_exchange_8.c

atomic_exchange_4.o: $(MWLIB)/mwcl/misc_g/atomic_exchange_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_exchange_4.c

atomic_exchange_2.o: $(MWLIB)/mwcl/misc_g/atomic_exchange_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_exchange_2.c

atomic_exchange_1.o: $(MWLIB)/mwcl/misc_g/atomic_exchange_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_exchange_1.c

atomic_is_lock_free.o: $(MWLIB)/mwcl/misc_g/atomic_is_lock_free.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_is_lock_free.c

atomic_store_8.o: $(MWLIB)/mwcl/misc_g/atomic_store_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_store_8.c

atomic_store_4.o: $(MWLIB)/mwcl/misc_g/atomic_store_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_store_4.c

atomic_store_2.o: $(MWLIB)/mwcl/misc_g/atomic_store_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_store_2.c

atomic_store_1.o: $(MWLIB)/mwcl/misc_g/atomic_store_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_store_1.c

atomic_load_8.o: $(MWLIB)/mwcl/misc_g/atomic_load_8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_load_8.c

atomic_load_4.o: $(MWLIB)/mwcl/misc_g/atomic_load_4.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_load_4.c

atomic_load_2.o: $(MWLIB)/mwcl/misc_g/atomic_load_2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_load_2.c

atomic_load_1.o: $(MWLIB)/mwcl/misc_g/atomic_load_1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwcl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwcl/misc_g/atomic_load_1.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
