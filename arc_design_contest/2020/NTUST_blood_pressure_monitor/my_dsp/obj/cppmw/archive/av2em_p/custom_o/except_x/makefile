# libcppmw arc.av2em.custom.except
MWLIB=../../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30 -Hexcept
CC_INC= -I$(MWLIB)/cppmw/incc -I$(MWLIB)/cppmw/incc/internal -I$(MWLIB)/c/inc -I$(MWLIB)/stl/stlport
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libcppmw.a
AR_OPT=""

LIST_complx=c_acos.o c_arg.o c_asin.o c_atan.o c_cos.o c_cosh.o c_div_cc.o\
	c_div_dc.o c_exp.o c_ext.o c_ins.o c_log.o c_log10.o c_mul_cc.o\
	c_norm.o c_pow_cc.o c_pow_cd.o c_pow_dc.o c_sin.o c_sinh.o c_sqrt.o\
	c_tan.o c_tanh.o

LIST_misc=badalloc.o cppinit.o cvtld.o eh_exception.o newhandl.o oldnewhandl.o\
	opdel.o opdela.o opnew.o opnewa.o opnewap.o opnewp.o xymalloc.o

LIST_llvm=eh_alloc.o eh_aux_runtime.o eh_call.o eh_catch.o eh_globals.o\
	eh_personality.o eh_term_handler.o eh_terminate.o eh_throw.o eh_type.o\
	eh_unex_handler.o guard.o pure.o tinfo1.o tinfo2.o unwind-dw2.o\
	unwind-dw2-fde.o vec.o

LIST_stream_fstrm=f_ctor.o f_dtor.o f_open.o fb_atach.o fb_check.o fb_close.o\
	fb_ctor.o fb_dbp.o fb_dtor.o fb_oflow.o fb_open.o fb_phyin.o fb_setbf.o\
	fb_skoff.o fb_sync.o fb_uflow.o fsb_atch.o fsb_clos.o fsb_ctor.o\
	fsb_dtor.o fsb_open.o fsb_stbf.o fsb_vt.o if_ctor.o if_dtor.o if_open.o\
	of_ctor.o of_dtor.o of_open.o

LIST_stream_ios=bitalloc.o fill.o flags.o ios_chk.o ios_ctor.o ios_dtor.o\
	ios_init.o ios_mt.o precisn.o setf.o syncstd.o tie.o unsetf.o uresize.o\
	uwords.o xalloc.o

LIST_stream_istrm=doget.o doipfx.o doisfx.o eatwhite.o ext_cp.o ext_d.o ext_i.o\
	ext_l.o ext_ld.o ext_man.o ext_s.o ext_sb.o get.o getline.o getlset.o\
	getsbr.o getset.o ignore.o incvt_d.o incvt_ld.o incvt_ut.o is_ctor.o\
	is_dtor.o is_extc.o is_extuc.o is_getc.o is_getuc.o is_getv.o ischeck.o\
	istrm_mt.o mwdogets.o putback.o read.o seekg.o tellg.o

LIST_stream_manip=iosmanip.o man_fill.o man_prec.o man_res.o man_set.o\
	man_setb.o man_setw.o

LIST_stream_misc=ioinit.o iostream.o iowa_ct.o iwa_ct.o owa_ct.o

LIST_stream_ostrm=cvtbytes.o cvtcharp.o cvtdbl.o cvtint.o cvtldbl.o cvtlong.o\
	cvtptr.o cvtshort.o cvtuint.o cvtulong.o cvtushrt.o cvtutils.o\
	do_opfx.o do_osfx.o flush.o ins_ccp.o ins_d.o ins_f.o ins_i.o ins_l.o\
	ins_ld.o ins_man.o ins_sb.o ins_si.o ins_vp.o os_ctor.o os_dtor.o\
	os_putc.o os_write.o ostrm_mt.o seekp.o tellp.o

LIST_stream_stdstm=std_ctor.o std_oflo.o std_sk.o std_sync.o std_uflo.o

LIST_stream_strmbf=checks.o dbp.o doalloc.o overflow.o pbackfl.o psnextc.o\
	sbf_ctor.o sbf_dtor.o sbf_mt.o seekoff.o seekpos.o setbuf.o sync.o\
	undrflow.o xsgetn.o xsputn.o

LIST_stream_strstm=ist_ctor.o ost_ctor.o ost_pcnt.o ost_str.o sba_ctor.o\
	sba_rdbf.o stb_aloc.o stb_buf.o stb_frz.o stb_init.o stb_oflo.o\
	stb_seek.o stb_str.o stb_uflo.o str_ctor.o str_str.o

LIST_ALL=$(LIST_complx) $(LIST_misc) $(LIST_llvm) $(LIST_stream_fstrm) $(LIST_stream_ios) $(LIST_stream_istrm) $(LIST_stream_manip) $(LIST_stream_misc) $(LIST_stream_ostrm) $(LIST_stream_stdstm) $(LIST_stream_strmbf) $(LIST_stream_strstm)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_complx)
	$(AR) rv $(AR_NAME) $(LIST_misc)
	$(AR) rv $(AR_NAME) $(LIST_llvm)
	$(AR) rv $(AR_NAME) $(LIST_stream_fstrm)
	$(AR) rv $(AR_NAME) $(LIST_stream_ios)
	$(AR) rv $(AR_NAME) $(LIST_stream_istrm)
	$(AR) rv $(AR_NAME) $(LIST_stream_manip)
	$(AR) rv $(AR_NAME) $(LIST_stream_misc)
	$(AR) rv $(AR_NAME) $(LIST_stream_ostrm)
	$(AR) rv $(AR_NAME) $(LIST_stream_stdstm)
	$(AR) rv $(AR_NAME) $(LIST_stream_strmbf)
	$(AR) rv $(AR_NAME) $(LIST_stream_strstm)

complx: $(LIST_complx)
misc: $(LIST_misc)
llvm: $(LIST_llvm)
stream_fstrm: $(LIST_stream_fstrm)
stream_ios: $(LIST_stream_ios)
stream_istrm: $(LIST_stream_istrm)
stream_manip: $(LIST_stream_manip)
stream_misc: $(LIST_stream_misc)
stream_ostrm: $(LIST_stream_ostrm)
stream_stdstm: $(LIST_stream_stdstm)
stream_strmbf: $(LIST_stream_strmbf)
stream_strstm: $(LIST_stream_strstm)

c_acos.o: $(MWLIB)/cppmw/complx_g/c_acos.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_acos.cpp

c_arg.o: $(MWLIB)/cppmw/complx_g/c_arg.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_arg.cpp

c_asin.o: $(MWLIB)/cppmw/complx_g/c_asin.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_asin.cpp

c_atan.o: $(MWLIB)/cppmw/complx_g/c_atan.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_atan.cpp

c_cos.o: $(MWLIB)/cppmw/complx_g/c_cos.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_cos.cpp

c_cosh.o: $(MWLIB)/cppmw/complx_g/c_cosh.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_cosh.cpp

c_div_cc.o: $(MWLIB)/cppmw/complx_g/c_div_cc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_div_cc.cpp

c_div_dc.o: $(MWLIB)/cppmw/complx_g/c_div_dc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_div_dc.cpp

c_exp.o: $(MWLIB)/cppmw/complx_g/c_exp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_exp.cpp

c_ext.o: $(MWLIB)/cppmw/complx_g/c_ext.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_ext.cpp

c_ins.o: $(MWLIB)/cppmw/complx_g/c_ins.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_ins.cpp

c_log.o: $(MWLIB)/cppmw/complx_g/c_log.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_log.cpp

c_log10.o: $(MWLIB)/cppmw/complx_g/c_log10.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_log10.cpp

c_mul_cc.o: $(MWLIB)/cppmw/complx_g/c_mul_cc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_mul_cc.cpp

c_norm.o: $(MWLIB)/cppmw/complx_g/c_norm.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_norm.cpp

c_pow_cc.o: $(MWLIB)/cppmw/complx_g/c_pow_cc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_pow_cc.cpp

c_pow_cd.o: $(MWLIB)/cppmw/complx_g/c_pow_cd.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_pow_cd.cpp

c_pow_dc.o: $(MWLIB)/cppmw/complx_g/c_pow_dc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_pow_dc.cpp

c_sin.o: $(MWLIB)/cppmw/complx_g/c_sin.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_sin.cpp

c_sinh.o: $(MWLIB)/cppmw/complx_g/c_sinh.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_sinh.cpp

c_sqrt.o: $(MWLIB)/cppmw/complx_g/c_sqrt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_sqrt.cpp

c_tan.o: $(MWLIB)/cppmw/complx_g/c_tan.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_tan.cpp

c_tanh.o: $(MWLIB)/cppmw/complx_g/c_tanh.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/complx_g/c_tanh.cpp

badalloc.o: $(MWLIB)/cppmw/misc_g/badalloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/badalloc.cpp

cppinit.o: $(MWLIB)/cppmw/misc_g/cppinit.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/cppinit.cpp

cvtld.o: $(MWLIB)/cppmw/misc_g/cvtld.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/cvtld.c

eh_exception.o: $(MWLIB)/cppmw/misc_g/eh_exception.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/eh_exception.cpp

newhandl.o: $(MWLIB)/cppmw/misc_g/newhandl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/newhandl.cpp

oldnewhandl.o: $(MWLIB)/cppmw/misc_g/oldnewhandl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/oldnewhandl.cpp

opdel.o: $(MWLIB)/cppmw/misc_g/opdel.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/opdel.cpp

opdela.o: $(MWLIB)/cppmw/misc_g/opdela.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/opdela.cpp

opnew.o: $(MWLIB)/cppmw/misc_g/opnew.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/opnew.cpp

opnewa.o: $(MWLIB)/cppmw/misc_g/opnewa.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/opnewa.cpp

opnewap.o: $(MWLIB)/cppmw/misc_g/opnewap.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/opnewap.cpp

opnewp.o: $(MWLIB)/cppmw/misc_g/opnewp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/opnewp.cpp

eh_alloc.o: $(MWLIB)/cppmw/llvm_g/eh_alloc.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_alloc.cc

eh_aux_runtime.o: $(MWLIB)/cppmw/llvm_g/eh_aux_runtime.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_aux_runtime.cc

eh_call.o: $(MWLIB)/cppmw/llvm_g/eh_call.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_call.cc

eh_catch.o: $(MWLIB)/cppmw/llvm_g/eh_catch.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_catch.cc

eh_globals.o: $(MWLIB)/cppmw/llvm_g/eh_globals.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -D__METAWARE__ -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_globals.cc

eh_personality.o: $(MWLIB)/cppmw/llvm_g/eh_personality.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_personality.cc

eh_term_handler.o: $(MWLIB)/cppmw/llvm_g/eh_term_handler.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_term_handler.cc

eh_terminate.o: $(MWLIB)/cppmw/llvm_g/eh_terminate.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_terminate.cc

eh_throw.o: $(MWLIB)/cppmw/llvm_g/eh_throw.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_throw.cc

eh_type.o: $(MWLIB)/cppmw/llvm_g/eh_type.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_type.cc

eh_unex_handler.o: $(MWLIB)/cppmw/llvm_g/eh_unex_handler.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/eh_unex_handler.cc

guard.o: $(MWLIB)/cppmw/llvm_g/guard.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/llvm_g/guard.cc

pure.o: $(MWLIB)/cppmw/llvm_g/pure.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/pure.cc

tinfo1.o: $(MWLIB)/cppmw/llvm_g/tinfo1.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/llvm_g/tinfo1.cc

tinfo2.o: $(MWLIB)/cppmw/llvm_g/tinfo2.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/llvm_g/tinfo2.cc

unwind-dw2.o: $(MWLIB)/cppmw/llvm_g/unwind-dw2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -D__METAWARE__ -Hoff=stackcheck_alloca -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/unwind-dw2.c

unwind-dw2-fde.o: $(MWLIB)/cppmw/llvm_g/unwind-dw2-fde.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -D__METAWARE__ -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/unwind-dw2-fde.c

vec.o: $(MWLIB)/cppmw/llvm_g/vec.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/cppmw/llvm_g/vec.cc

xymalloc.o: $(MWLIB)/cppmw/misc_g/xymalloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/misc_g/xymalloc.cpp

f_ctor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/f_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/f_ctor.cpp

f_dtor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/f_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/f_dtor.cpp

f_open.o: $(MWLIB)/cppmw/stream_g/fstrm_g/f_open.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/f_open.cpp

fb_atach.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_atach.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_atach.cpp

fb_check.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_check.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_check.cpp

fb_close.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_close.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_close.cpp

fb_ctor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_ctor.cpp

fb_dbp.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_dbp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_dbp.cpp

fb_dtor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_dtor.cpp

fb_oflow.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_oflow.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_oflow.cpp

fb_open.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_open.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_open.cpp

fb_phyin.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_phyin.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_phyin.cpp

fb_setbf.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_setbf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_setbf.cpp

fb_skoff.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_skoff.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_skoff.cpp

fb_sync.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_sync.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_sync.cpp

fb_uflow.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fb_uflow.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fb_uflow.cpp

fsb_atch.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_atch.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_atch.cpp

fsb_clos.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_clos.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_clos.cpp

fsb_ctor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_ctor.cpp

fsb_dtor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_dtor.cpp

fsb_open.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_open.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_open.cpp

fsb_stbf.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_stbf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_stbf.cpp

fsb_vt.o: $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_vt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/fsb_vt.cpp

if_ctor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/if_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/if_ctor.cpp

if_dtor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/if_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/if_dtor.cpp

if_open.o: $(MWLIB)/cppmw/stream_g/fstrm_g/if_open.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/if_open.cpp

of_ctor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/of_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/of_ctor.cpp

of_dtor.o: $(MWLIB)/cppmw/stream_g/fstrm_g/of_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/of_dtor.cpp

of_open.o: $(MWLIB)/cppmw/stream_g/fstrm_g/of_open.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/fstrm_g/of_open.cpp

bitalloc.o: $(MWLIB)/cppmw/stream_g/ios_g/bitalloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/bitalloc.cpp

fill.o: $(MWLIB)/cppmw/stream_g/ios_g/fill.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/fill.cpp

flags.o: $(MWLIB)/cppmw/stream_g/ios_g/flags.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/flags.cpp

ios_chk.o: $(MWLIB)/cppmw/stream_g/ios_g/ios_chk.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/ios_chk.cpp

ios_ctor.o: $(MWLIB)/cppmw/stream_g/ios_g/ios_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/ios_ctor.cpp

ios_dtor.o: $(MWLIB)/cppmw/stream_g/ios_g/ios_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/ios_dtor.cpp

ios_init.o: $(MWLIB)/cppmw/stream_g/ios_g/ios_init.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/ios_init.cpp

ios_mt.o: $(MWLIB)/cppmw/stream_g/ios_g/ios_mt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/ios_mt.cpp

precisn.o: $(MWLIB)/cppmw/stream_g/ios_g/precisn.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/precisn.cpp

setf.o: $(MWLIB)/cppmw/stream_g/ios_g/setf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/setf.cpp

syncstd.o: $(MWLIB)/cppmw/stream_g/ios_g/syncstd.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/syncstd.cpp

tie.o: $(MWLIB)/cppmw/stream_g/ios_g/tie.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/tie.cpp

unsetf.o: $(MWLIB)/cppmw/stream_g/ios_g/unsetf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/unsetf.cpp

uresize.o: $(MWLIB)/cppmw/stream_g/ios_g/uresize.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/uresize.cpp

uwords.o: $(MWLIB)/cppmw/stream_g/ios_g/uwords.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/uwords.cpp

xalloc.o: $(MWLIB)/cppmw/stream_g/ios_g/xalloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ios_g/xalloc.cpp

doget.o: $(MWLIB)/cppmw/stream_g/istrm_g/doget.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/doget.cpp

doipfx.o: $(MWLIB)/cppmw/stream_g/istrm_g/doipfx.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/doipfx.cpp

doisfx.o: $(MWLIB)/cppmw/stream_g/istrm_g/doisfx.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/doisfx.cpp

eatwhite.o: $(MWLIB)/cppmw/stream_g/istrm_g/eatwhite.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/eatwhite.cpp

ext_cp.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_cp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_cp.cpp

ext_d.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_d.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_d.cpp

ext_i.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_i.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_i.cpp

ext_l.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_l.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_l.cpp

ext_ld.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_ld.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_ld.cpp

ext_man.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_man.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_man.cpp

ext_s.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_s.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_s.cpp

ext_sb.o: $(MWLIB)/cppmw/stream_g/istrm_g/ext_sb.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ext_sb.cpp

get.o: $(MWLIB)/cppmw/stream_g/istrm_g/get.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/get.cpp

getline.o: $(MWLIB)/cppmw/stream_g/istrm_g/getline.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/getline.cpp

getlset.o: $(MWLIB)/cppmw/stream_g/istrm_g/getlset.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/getlset.cpp

getsbr.o: $(MWLIB)/cppmw/stream_g/istrm_g/getsbr.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/getsbr.cpp

getset.o: $(MWLIB)/cppmw/stream_g/istrm_g/getset.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/getset.cpp

ignore.o: $(MWLIB)/cppmw/stream_g/istrm_g/ignore.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ignore.cpp

incvt_d.o: $(MWLIB)/cppmw/stream_g/istrm_g/incvt_d.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/incvt_d.cpp

incvt_ld.o: $(MWLIB)/cppmw/stream_g/istrm_g/incvt_ld.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/incvt_ld.cpp

incvt_ut.o: $(MWLIB)/cppmw/stream_g/istrm_g/incvt_ut.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/incvt_ut.cpp

is_ctor.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_ctor.cpp

is_dtor.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_dtor.cpp

is_extc.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_extc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_extc.cpp

is_extuc.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_extuc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_extuc.cpp

is_getc.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_getc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_getc.cpp

is_getuc.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_getuc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_getuc.cpp

is_getv.o: $(MWLIB)/cppmw/stream_g/istrm_g/is_getv.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/is_getv.cpp

ischeck.o: $(MWLIB)/cppmw/stream_g/istrm_g/ischeck.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/ischeck.cpp

istrm_mt.o: $(MWLIB)/cppmw/stream_g/istrm_g/istrm_mt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/istrm_mt.cpp

mwdogets.o: $(MWLIB)/cppmw/stream_g/istrm_g/mwdogets.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/mwdogets.cpp

putback.o: $(MWLIB)/cppmw/stream_g/istrm_g/putback.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/putback.cpp

read.o: $(MWLIB)/cppmw/stream_g/istrm_g/read.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/read.cpp

seekg.o: $(MWLIB)/cppmw/stream_g/istrm_g/seekg.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/seekg.cpp

tellg.o: $(MWLIB)/cppmw/stream_g/istrm_g/tellg.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/istrm_g/tellg.cpp

iosmanip.o: $(MWLIB)/cppmw/stream_g/manip_g/iosmanip.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/iosmanip.cpp

man_fill.o: $(MWLIB)/cppmw/stream_g/manip_g/man_fill.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/man_fill.cpp

man_prec.o: $(MWLIB)/cppmw/stream_g/manip_g/man_prec.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/man_prec.cpp

man_res.o: $(MWLIB)/cppmw/stream_g/manip_g/man_res.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/man_res.cpp

man_set.o: $(MWLIB)/cppmw/stream_g/manip_g/man_set.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/man_set.cpp

man_setb.o: $(MWLIB)/cppmw/stream_g/manip_g/man_setb.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/man_setb.cpp

man_setw.o: $(MWLIB)/cppmw/stream_g/manip_g/man_setw.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/manip_g/man_setw.cpp

ioinit.o: $(MWLIB)/cppmw/stream_g/misc_g/ioinit.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/misc_g/ioinit.cpp

iostream.o: $(MWLIB)/cppmw/stream_g/misc_g/iostream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/misc_g/iostream.cpp

iowa_ct.o: $(MWLIB)/cppmw/stream_g/misc_g/iowa_ct.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/misc_g/iowa_ct.cpp

iwa_ct.o: $(MWLIB)/cppmw/stream_g/misc_g/iwa_ct.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/misc_g/iwa_ct.cpp

owa_ct.o: $(MWLIB)/cppmw/stream_g/misc_g/owa_ct.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/misc_g/owa_ct.cpp

cvtbytes.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtbytes.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtbytes.cpp

cvtcharp.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtcharp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtcharp.cpp

cvtdbl.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtdbl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtdbl.cpp

cvtint.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtint.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtint.cpp

cvtldbl.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtldbl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtldbl.cpp

cvtlong.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtlong.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtlong.cpp

cvtptr.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtptr.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtptr.cpp

cvtshort.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtshort.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtshort.cpp

cvtuint.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtuint.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtuint.cpp

cvtulong.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtulong.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtulong.cpp

cvtushrt.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtushrt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtushrt.cpp

cvtutils.o: $(MWLIB)/cppmw/stream_g/ostrm_g/cvtutils.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/cvtutils.cpp

do_opfx.o: $(MWLIB)/cppmw/stream_g/ostrm_g/do_opfx.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/do_opfx.cpp

do_osfx.o: $(MWLIB)/cppmw/stream_g/ostrm_g/do_osfx.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/do_osfx.cpp

flush.o: $(MWLIB)/cppmw/stream_g/ostrm_g/flush.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/flush.cpp

ins_ccp.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_ccp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_ccp.cpp

ins_d.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_d.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_d.cpp

ins_f.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_f.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_f.cpp

ins_i.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_i.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_i.cpp

ins_l.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_l.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_l.cpp

ins_ld.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_ld.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_ld.cpp

ins_man.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_man.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_man.cpp

ins_sb.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_sb.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_sb.cpp

ins_si.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_si.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_si.cpp

ins_vp.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ins_vp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ins_vp.cpp

os_ctor.o: $(MWLIB)/cppmw/stream_g/ostrm_g/os_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/os_ctor.cpp

os_dtor.o: $(MWLIB)/cppmw/stream_g/ostrm_g/os_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/os_dtor.cpp

os_putc.o: $(MWLIB)/cppmw/stream_g/ostrm_g/os_putc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/os_putc.cpp

os_write.o: $(MWLIB)/cppmw/stream_g/ostrm_g/os_write.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/os_write.cpp

ostrm_mt.o: $(MWLIB)/cppmw/stream_g/ostrm_g/ostrm_mt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/ostrm_mt.cpp

seekp.o: $(MWLIB)/cppmw/stream_g/ostrm_g/seekp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/seekp.cpp

tellp.o: $(MWLIB)/cppmw/stream_g/ostrm_g/tellp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/ostrm_g/tellp.cpp

std_ctor.o: $(MWLIB)/cppmw/stream_g/stdstm_g/std_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/stdstm_g/std_ctor.cpp

std_oflo.o: $(MWLIB)/cppmw/stream_g/stdstm_g/std_oflo.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/stdstm_g/std_oflo.cpp

std_sk.o: $(MWLIB)/cppmw/stream_g/stdstm_g/std_sk.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/stdstm_g/std_sk.cpp

std_sync.o: $(MWLIB)/cppmw/stream_g/stdstm_g/std_sync.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/stdstm_g/std_sync.cpp

std_uflo.o: $(MWLIB)/cppmw/stream_g/stdstm_g/std_uflo.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/stdstm_g/std_uflo.cpp

checks.o: $(MWLIB)/cppmw/stream_g/strmbf_g/checks.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/checks.cpp

dbp.o: $(MWLIB)/cppmw/stream_g/strmbf_g/dbp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/dbp.cpp

doalloc.o: $(MWLIB)/cppmw/stream_g/strmbf_g/doalloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/doalloc.cpp

overflow.o: $(MWLIB)/cppmw/stream_g/strmbf_g/overflow.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/overflow.cpp

pbackfl.o: $(MWLIB)/cppmw/stream_g/strmbf_g/pbackfl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/pbackfl.cpp

psnextc.o: $(MWLIB)/cppmw/stream_g/strmbf_g/psnextc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/psnextc.cpp

sbf_ctor.o: $(MWLIB)/cppmw/stream_g/strmbf_g/sbf_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/sbf_ctor.cpp

sbf_dtor.o: $(MWLIB)/cppmw/stream_g/strmbf_g/sbf_dtor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/sbf_dtor.cpp

sbf_mt.o: $(MWLIB)/cppmw/stream_g/strmbf_g/sbf_mt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/sbf_mt.cpp

seekoff.o: $(MWLIB)/cppmw/stream_g/strmbf_g/seekoff.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/seekoff.cpp

seekpos.o: $(MWLIB)/cppmw/stream_g/strmbf_g/seekpos.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/seekpos.cpp

setbuf.o: $(MWLIB)/cppmw/stream_g/strmbf_g/setbuf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/setbuf.cpp

sync.o: $(MWLIB)/cppmw/stream_g/strmbf_g/sync.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/sync.cpp

undrflow.o: $(MWLIB)/cppmw/stream_g/strmbf_g/undrflow.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/undrflow.cpp

xsgetn.o: $(MWLIB)/cppmw/stream_g/strmbf_g/xsgetn.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/xsgetn.cpp

xsputn.o: $(MWLIB)/cppmw/stream_g/strmbf_g/xsputn.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strmbf_g/xsputn.cpp

ist_ctor.o: $(MWLIB)/cppmw/stream_g/strstm_g/ist_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/ist_ctor.cpp

ost_ctor.o: $(MWLIB)/cppmw/stream_g/strstm_g/ost_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/ost_ctor.cpp

ost_pcnt.o: $(MWLIB)/cppmw/stream_g/strstm_g/ost_pcnt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/ost_pcnt.cpp

ost_str.o: $(MWLIB)/cppmw/stream_g/strstm_g/ost_str.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/ost_str.cpp

sba_ctor.o: $(MWLIB)/cppmw/stream_g/strstm_g/sba_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/sba_ctor.cpp

sba_rdbf.o: $(MWLIB)/cppmw/stream_g/strstm_g/sba_rdbf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/sba_rdbf.cpp

stb_aloc.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_aloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_aloc.cpp

stb_buf.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_buf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_buf.cpp

stb_frz.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_frz.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_frz.cpp

stb_init.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_init.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_init.cpp

stb_oflo.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_oflo.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_oflo.cpp

stb_seek.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_seek.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_seek.cpp

stb_str.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_str.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_str.cpp

stb_uflo.o: $(MWLIB)/cppmw/stream_g/strstm_g/stb_uflo.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/stb_uflo.cpp

str_ctor.o: $(MWLIB)/cppmw/stream_g/strstm_g/str_ctor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/str_ctor.cpp

str_str.o: $(MWLIB)/cppmw/stream_g/strstm_g/str_str.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cppmw/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cppmw/stream_g/strstm_g/str_str.cpp

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
