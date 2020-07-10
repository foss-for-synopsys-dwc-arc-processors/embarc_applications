# libcpp arc.av2em.custom
MWLIB=../../../..

CC=mcc

MCC=mcc
OPT_LEV=
CC_OPT=-Hnocopyr -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
CC_INC= -I$(MWLIB)/cpp/incc -I$(MWLIB)/cpp/incc/internal -I$(MWLIB)/c/inc
cflags=

AS=$(MCC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libcct.a
AR_OPT=""

LIST_complx=c_acos.o c_arg.o c_asin.o c_atan.o c_cos.o c_cosh.o c_div_cc.o\
	c_div_dc.o c_exp.o c_ext.o c_ins.o c_log.o c_log10.o c_mul_cc.o\
	c_norm.o c_pow_cc.o c_pow_cd.o c_pow_dc.o c_sin.o c_sinh.o c_sqrt.o\
	c_tan.o c_tanh.o

LIST_misc=badalloc.o cppinit.o cvtld.o eh_exception.o newhandl.o oldnewhandl.o\
	opdel.o opdela.o opnew.o opnewa.o opnewap.o opnewp.o arrays.o\
	embedded.o exc.o excret.o libdebug.o opdels.o opnewnt.o opnews.o\
	tinfo.o xymalloc.o

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

LIST_ALL=$(LIST_complx) $(LIST_misc) $(LIST_stream_fstrm) $(LIST_stream_ios) $(LIST_stream_istrm) $(LIST_stream_manip) $(LIST_stream_misc) $(LIST_stream_ostrm) $(LIST_stream_stdstm) $(LIST_stream_strmbf) $(LIST_stream_strstm)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_complx)
	$(AR) rv $(AR_NAME) $(LIST_misc)
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
stream_fstrm: $(LIST_stream_fstrm)
stream_ios: $(LIST_stream_ios)
stream_istrm: $(LIST_stream_istrm)
stream_manip: $(LIST_stream_manip)
stream_misc: $(LIST_stream_misc)
stream_ostrm: $(LIST_stream_ostrm)
stream_stdstm: $(LIST_stream_stdstm)
stream_strmbf: $(LIST_stream_strmbf)
stream_strstm: $(LIST_stream_strstm)

c_acos.o: $(MWLIB)/cpp/complx_g/c_acos.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_acos.cpp

c_arg.o: $(MWLIB)/cpp/complx_g/c_arg.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_arg.cpp

c_asin.o: $(MWLIB)/cpp/complx_g/c_asin.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_asin.cpp

c_atan.o: $(MWLIB)/cpp/complx_g/c_atan.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_atan.cpp

c_cos.o: $(MWLIB)/cpp/complx_g/c_cos.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_cos.cpp

c_cosh.o: $(MWLIB)/cpp/complx_g/c_cosh.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_cosh.cpp

c_div_cc.o: $(MWLIB)/cpp/complx_g/c_div_cc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_div_cc.cpp

c_div_dc.o: $(MWLIB)/cpp/complx_g/c_div_dc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_div_dc.cpp

c_exp.o: $(MWLIB)/cpp/complx_g/c_exp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_exp.cpp

c_ext.o: $(MWLIB)/cpp/complx_g/c_ext.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_ext.cpp

c_ins.o: $(MWLIB)/cpp/complx_g/c_ins.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_ins.cpp

c_log.o: $(MWLIB)/cpp/complx_g/c_log.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_log.cpp

c_log10.o: $(MWLIB)/cpp/complx_g/c_log10.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_log10.cpp

c_mul_cc.o: $(MWLIB)/cpp/complx_g/c_mul_cc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_mul_cc.cpp

c_norm.o: $(MWLIB)/cpp/complx_g/c_norm.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_norm.cpp

c_pow_cc.o: $(MWLIB)/cpp/complx_g/c_pow_cc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_pow_cc.cpp

c_pow_cd.o: $(MWLIB)/cpp/complx_g/c_pow_cd.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_pow_cd.cpp

c_pow_dc.o: $(MWLIB)/cpp/complx_g/c_pow_dc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_pow_dc.cpp

c_sin.o: $(MWLIB)/cpp/complx_g/c_sin.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_sin.cpp

c_sinh.o: $(MWLIB)/cpp/complx_g/c_sinh.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_sinh.cpp

c_sqrt.o: $(MWLIB)/cpp/complx_g/c_sqrt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_sqrt.cpp

c_tan.o: $(MWLIB)/cpp/complx_g/c_tan.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_tan.cpp

c_tanh.o: $(MWLIB)/cpp/complx_g/c_tanh.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/complx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/complx_g/c_tanh.cpp

badalloc.o: $(MWLIB)/cpp/misc_g/badalloc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/badalloc.cpp

cppinit.o: $(MWLIB)/cpp/misc_g/cppinit.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/cppinit.cpp

cvtld.o: $(MWLIB)/cpp/misc_g/cvtld.c
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/cvtld.c

eh_exception.o: $(MWLIB)/cpp/misc_g/eh_exception.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/eh_exception.cpp

newhandl.o: $(MWLIB)/cpp/misc_g/newhandl.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/newhandl.cpp

oldnewhandl.o: $(MWLIB)/cpp/misc_g/oldnewhandl.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/oldnewhandl.cpp

opdel.o: $(MWLIB)/cpp/misc_g/opdel.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opdel.cpp

opdela.o: $(MWLIB)/cpp/misc_g/opdela.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opdela.cpp

opnew.o: $(MWLIB)/cpp/misc_g/opnew.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opnew.cpp

opnewa.o: $(MWLIB)/cpp/misc_g/opnewa.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opnewa.cpp

opnewap.o: $(MWLIB)/cpp/misc_g/opnewap.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opnewap.cpp

opnewp.o: $(MWLIB)/cpp/misc_g/opnewp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opnewp.cpp

arrays.o: $(MWLIB)/cpp/misc_g/arrays.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/arrays.cpp

embedded.o: $(MWLIB)/cpp/misc_g/embedded.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/embedded.cpp

exc.o: $(MWLIB)/cpp/misc_g/exc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/exc.cpp

excret.o: $(MWLIB)/cpp/misc_g/excret.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/excret.cpp

libdebug.o: $(MWLIB)/cpp/misc_g/libdebug.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) -g $(MWLIB)/cpp/misc_g/libdebug.cpp

opdels.o: $(MWLIB)/cpp/misc_g/opdels.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opdels.cpp

opnewnt.o: $(MWLIB)/cpp/misc_g/opnewnt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opnewnt.cpp

opnews.o: $(MWLIB)/cpp/misc_g/opnews.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/opnews.cpp

tinfo.o: $(MWLIB)/cpp/misc_g/tinfo.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/tinfo.cpp

xymalloc.o: $(MWLIB)/cpp/misc_g/xymalloc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/misc_g/xymalloc.cpp

f_ctor.o: $(MWLIB)/cpp/stream_g/fstrm_g/f_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/f_ctor.cpp

f_dtor.o: $(MWLIB)/cpp/stream_g/fstrm_g/f_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/f_dtor.cpp

f_open.o: $(MWLIB)/cpp/stream_g/fstrm_g/f_open.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/f_open.cpp

fb_atach.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_atach.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_atach.cpp

fb_check.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_check.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_check.cpp

fb_close.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_close.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_close.cpp

fb_ctor.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_ctor.cpp

fb_dbp.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_dbp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_dbp.cpp

fb_dtor.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_dtor.cpp

fb_oflow.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_oflow.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_oflow.cpp

fb_open.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_open.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_open.cpp

fb_phyin.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_phyin.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_phyin.cpp

fb_setbf.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_setbf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_setbf.cpp

fb_skoff.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_skoff.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_skoff.cpp

fb_sync.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_sync.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_sync.cpp

fb_uflow.o: $(MWLIB)/cpp/stream_g/fstrm_g/fb_uflow.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fb_uflow.cpp

fsb_atch.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_atch.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_atch.cpp

fsb_clos.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_clos.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_clos.cpp

fsb_ctor.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_ctor.cpp

fsb_dtor.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_dtor.cpp

fsb_open.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_open.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_open.cpp

fsb_stbf.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_stbf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_stbf.cpp

fsb_vt.o: $(MWLIB)/cpp/stream_g/fstrm_g/fsb_vt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/fsb_vt.cpp

if_ctor.o: $(MWLIB)/cpp/stream_g/fstrm_g/if_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/if_ctor.cpp

if_dtor.o: $(MWLIB)/cpp/stream_g/fstrm_g/if_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/if_dtor.cpp

if_open.o: $(MWLIB)/cpp/stream_g/fstrm_g/if_open.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/if_open.cpp

of_ctor.o: $(MWLIB)/cpp/stream_g/fstrm_g/of_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/of_ctor.cpp

of_dtor.o: $(MWLIB)/cpp/stream_g/fstrm_g/of_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/of_dtor.cpp

of_open.o: $(MWLIB)/cpp/stream_g/fstrm_g/of_open.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/fstrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/fstrm_g/of_open.cpp

bitalloc.o: $(MWLIB)/cpp/stream_g/ios_g/bitalloc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/bitalloc.cpp

fill.o: $(MWLIB)/cpp/stream_g/ios_g/fill.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/fill.cpp

flags.o: $(MWLIB)/cpp/stream_g/ios_g/flags.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/flags.cpp

ios_chk.o: $(MWLIB)/cpp/stream_g/ios_g/ios_chk.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/ios_chk.cpp

ios_ctor.o: $(MWLIB)/cpp/stream_g/ios_g/ios_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/ios_ctor.cpp

ios_dtor.o: $(MWLIB)/cpp/stream_g/ios_g/ios_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/ios_dtor.cpp

ios_init.o: $(MWLIB)/cpp/stream_g/ios_g/ios_init.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/ios_init.cpp

ios_mt.o: $(MWLIB)/cpp/stream_g/ios_g/ios_mt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/ios_mt.cpp

precisn.o: $(MWLIB)/cpp/stream_g/ios_g/precisn.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/precisn.cpp

setf.o: $(MWLIB)/cpp/stream_g/ios_g/setf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/setf.cpp

syncstd.o: $(MWLIB)/cpp/stream_g/ios_g/syncstd.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/syncstd.cpp

tie.o: $(MWLIB)/cpp/stream_g/ios_g/tie.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/tie.cpp

unsetf.o: $(MWLIB)/cpp/stream_g/ios_g/unsetf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/unsetf.cpp

uresize.o: $(MWLIB)/cpp/stream_g/ios_g/uresize.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/uresize.cpp

uwords.o: $(MWLIB)/cpp/stream_g/ios_g/uwords.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/uwords.cpp

xalloc.o: $(MWLIB)/cpp/stream_g/ios_g/xalloc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ios_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ios_g/xalloc.cpp

doget.o: $(MWLIB)/cpp/stream_g/istrm_g/doget.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/doget.cpp

doipfx.o: $(MWLIB)/cpp/stream_g/istrm_g/doipfx.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/doipfx.cpp

doisfx.o: $(MWLIB)/cpp/stream_g/istrm_g/doisfx.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/doisfx.cpp

eatwhite.o: $(MWLIB)/cpp/stream_g/istrm_g/eatwhite.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/eatwhite.cpp

ext_cp.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_cp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_cp.cpp

ext_d.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_d.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_d.cpp

ext_i.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_i.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_i.cpp

ext_l.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_l.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_l.cpp

ext_ld.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_ld.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_ld.cpp

ext_man.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_man.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_man.cpp

ext_s.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_s.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_s.cpp

ext_sb.o: $(MWLIB)/cpp/stream_g/istrm_g/ext_sb.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ext_sb.cpp

get.o: $(MWLIB)/cpp/stream_g/istrm_g/get.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/get.cpp

getline.o: $(MWLIB)/cpp/stream_g/istrm_g/getline.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/getline.cpp

getlset.o: $(MWLIB)/cpp/stream_g/istrm_g/getlset.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/getlset.cpp

getsbr.o: $(MWLIB)/cpp/stream_g/istrm_g/getsbr.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/getsbr.cpp

getset.o: $(MWLIB)/cpp/stream_g/istrm_g/getset.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/getset.cpp

ignore.o: $(MWLIB)/cpp/stream_g/istrm_g/ignore.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ignore.cpp

incvt_d.o: $(MWLIB)/cpp/stream_g/istrm_g/incvt_d.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/incvt_d.cpp

incvt_ld.o: $(MWLIB)/cpp/stream_g/istrm_g/incvt_ld.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/incvt_ld.cpp

incvt_ut.o: $(MWLIB)/cpp/stream_g/istrm_g/incvt_ut.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/incvt_ut.cpp

is_ctor.o: $(MWLIB)/cpp/stream_g/istrm_g/is_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_ctor.cpp

is_dtor.o: $(MWLIB)/cpp/stream_g/istrm_g/is_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_dtor.cpp

is_extc.o: $(MWLIB)/cpp/stream_g/istrm_g/is_extc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_extc.cpp

is_extuc.o: $(MWLIB)/cpp/stream_g/istrm_g/is_extuc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_extuc.cpp

is_getc.o: $(MWLIB)/cpp/stream_g/istrm_g/is_getc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_getc.cpp

is_getuc.o: $(MWLIB)/cpp/stream_g/istrm_g/is_getuc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_getuc.cpp

is_getv.o: $(MWLIB)/cpp/stream_g/istrm_g/is_getv.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/is_getv.cpp

ischeck.o: $(MWLIB)/cpp/stream_g/istrm_g/ischeck.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/ischeck.cpp

istrm_mt.o: $(MWLIB)/cpp/stream_g/istrm_g/istrm_mt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/istrm_mt.cpp

mwdogets.o: $(MWLIB)/cpp/stream_g/istrm_g/mwdogets.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/mwdogets.cpp

putback.o: $(MWLIB)/cpp/stream_g/istrm_g/putback.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/putback.cpp

read.o: $(MWLIB)/cpp/stream_g/istrm_g/read.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/read.cpp

seekg.o: $(MWLIB)/cpp/stream_g/istrm_g/seekg.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/seekg.cpp

tellg.o: $(MWLIB)/cpp/stream_g/istrm_g/tellg.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/istrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/istrm_g/tellg.cpp

iosmanip.o: $(MWLIB)/cpp/stream_g/manip_g/iosmanip.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/iosmanip.cpp

man_fill.o: $(MWLIB)/cpp/stream_g/manip_g/man_fill.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/man_fill.cpp

man_prec.o: $(MWLIB)/cpp/stream_g/manip_g/man_prec.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/man_prec.cpp

man_res.o: $(MWLIB)/cpp/stream_g/manip_g/man_res.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/man_res.cpp

man_set.o: $(MWLIB)/cpp/stream_g/manip_g/man_set.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/man_set.cpp

man_setb.o: $(MWLIB)/cpp/stream_g/manip_g/man_setb.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/man_setb.cpp

man_setw.o: $(MWLIB)/cpp/stream_g/manip_g/man_setw.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/manip_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/manip_g/man_setw.cpp

ioinit.o: $(MWLIB)/cpp/stream_g/misc_g/ioinit.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/misc_g/ioinit.cpp

iostream.o: $(MWLIB)/cpp/stream_g/misc_g/iostream.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/misc_g/iostream.cpp

iowa_ct.o: $(MWLIB)/cpp/stream_g/misc_g/iowa_ct.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/misc_g/iowa_ct.cpp

iwa_ct.o: $(MWLIB)/cpp/stream_g/misc_g/iwa_ct.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/misc_g/iwa_ct.cpp

owa_ct.o: $(MWLIB)/cpp/stream_g/misc_g/owa_ct.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/misc_g/owa_ct.cpp

cvtbytes.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtbytes.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtbytes.cpp

cvtcharp.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtcharp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtcharp.cpp

cvtdbl.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtdbl.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtdbl.cpp

cvtint.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtint.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtint.cpp

cvtldbl.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtldbl.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtldbl.cpp

cvtlong.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtlong.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtlong.cpp

cvtptr.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtptr.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtptr.cpp

cvtshort.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtshort.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtshort.cpp

cvtuint.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtuint.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtuint.cpp

cvtulong.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtulong.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtulong.cpp

cvtushrt.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtushrt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtushrt.cpp

cvtutils.o: $(MWLIB)/cpp/stream_g/ostrm_g/cvtutils.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/cvtutils.cpp

do_opfx.o: $(MWLIB)/cpp/stream_g/ostrm_g/do_opfx.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/do_opfx.cpp

do_osfx.o: $(MWLIB)/cpp/stream_g/ostrm_g/do_osfx.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/do_osfx.cpp

flush.o: $(MWLIB)/cpp/stream_g/ostrm_g/flush.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/flush.cpp

ins_ccp.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_ccp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_ccp.cpp

ins_d.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_d.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_d.cpp

ins_f.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_f.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_f.cpp

ins_i.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_i.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_i.cpp

ins_l.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_l.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_l.cpp

ins_ld.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_ld.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_ld.cpp

ins_man.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_man.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_man.cpp

ins_sb.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_sb.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_sb.cpp

ins_si.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_si.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_si.cpp

ins_vp.o: $(MWLIB)/cpp/stream_g/ostrm_g/ins_vp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ins_vp.cpp

os_ctor.o: $(MWLIB)/cpp/stream_g/ostrm_g/os_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/os_ctor.cpp

os_dtor.o: $(MWLIB)/cpp/stream_g/ostrm_g/os_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/os_dtor.cpp

os_putc.o: $(MWLIB)/cpp/stream_g/ostrm_g/os_putc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/os_putc.cpp

os_write.o: $(MWLIB)/cpp/stream_g/ostrm_g/os_write.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/os_write.cpp

ostrm_mt.o: $(MWLIB)/cpp/stream_g/ostrm_g/ostrm_mt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/ostrm_mt.cpp

seekp.o: $(MWLIB)/cpp/stream_g/ostrm_g/seekp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/seekp.cpp

tellp.o: $(MWLIB)/cpp/stream_g/ostrm_g/tellp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/ostrm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/ostrm_g/tellp.cpp

std_ctor.o: $(MWLIB)/cpp/stream_g/stdstm_g/std_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/stdstm_g/std_ctor.cpp

std_oflo.o: $(MWLIB)/cpp/stream_g/stdstm_g/std_oflo.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/stdstm_g/std_oflo.cpp

std_sk.o: $(MWLIB)/cpp/stream_g/stdstm_g/std_sk.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/stdstm_g/std_sk.cpp

std_sync.o: $(MWLIB)/cpp/stream_g/stdstm_g/std_sync.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/stdstm_g/std_sync.cpp

std_uflo.o: $(MWLIB)/cpp/stream_g/stdstm_g/std_uflo.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/stdstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/stdstm_g/std_uflo.cpp

checks.o: $(MWLIB)/cpp/stream_g/strmbf_g/checks.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/checks.cpp

dbp.o: $(MWLIB)/cpp/stream_g/strmbf_g/dbp.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/dbp.cpp

doalloc.o: $(MWLIB)/cpp/stream_g/strmbf_g/doalloc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/doalloc.cpp

overflow.o: $(MWLIB)/cpp/stream_g/strmbf_g/overflow.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/overflow.cpp

pbackfl.o: $(MWLIB)/cpp/stream_g/strmbf_g/pbackfl.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/pbackfl.cpp

psnextc.o: $(MWLIB)/cpp/stream_g/strmbf_g/psnextc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/psnextc.cpp

sbf_ctor.o: $(MWLIB)/cpp/stream_g/strmbf_g/sbf_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/sbf_ctor.cpp

sbf_dtor.o: $(MWLIB)/cpp/stream_g/strmbf_g/sbf_dtor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/sbf_dtor.cpp

sbf_mt.o: $(MWLIB)/cpp/stream_g/strmbf_g/sbf_mt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/sbf_mt.cpp

seekoff.o: $(MWLIB)/cpp/stream_g/strmbf_g/seekoff.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/seekoff.cpp

seekpos.o: $(MWLIB)/cpp/stream_g/strmbf_g/seekpos.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/seekpos.cpp

setbuf.o: $(MWLIB)/cpp/stream_g/strmbf_g/setbuf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/setbuf.cpp

sync.o: $(MWLIB)/cpp/stream_g/strmbf_g/sync.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/sync.cpp

undrflow.o: $(MWLIB)/cpp/stream_g/strmbf_g/undrflow.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/undrflow.cpp

xsgetn.o: $(MWLIB)/cpp/stream_g/strmbf_g/xsgetn.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/xsgetn.cpp

xsputn.o: $(MWLIB)/cpp/stream_g/strmbf_g/xsputn.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strmbf_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strmbf_g/xsputn.cpp

ist_ctor.o: $(MWLIB)/cpp/stream_g/strstm_g/ist_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/ist_ctor.cpp

ost_ctor.o: $(MWLIB)/cpp/stream_g/strstm_g/ost_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/ost_ctor.cpp

ost_pcnt.o: $(MWLIB)/cpp/stream_g/strstm_g/ost_pcnt.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/ost_pcnt.cpp

ost_str.o: $(MWLIB)/cpp/stream_g/strstm_g/ost_str.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/ost_str.cpp

sba_ctor.o: $(MWLIB)/cpp/stream_g/strstm_g/sba_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/sba_ctor.cpp

sba_rdbf.o: $(MWLIB)/cpp/stream_g/strstm_g/sba_rdbf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/sba_rdbf.cpp

stb_aloc.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_aloc.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_aloc.cpp

stb_buf.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_buf.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_buf.cpp

stb_frz.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_frz.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_frz.cpp

stb_init.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_init.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_init.cpp

stb_oflo.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_oflo.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_oflo.cpp

stb_seek.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_seek.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_seek.cpp

stb_str.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_str.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_str.cpp

stb_uflo.o: $(MWLIB)/cpp/stream_g/strstm_g/stb_uflo.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/stb_uflo.cpp

str_ctor.o: $(MWLIB)/cpp/stream_g/strstm_g/str_ctor.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/str_ctor.cpp

str_str.o: $(MWLIB)/cpp/stream_g/strstm_g/str_str.cpp
	$(MCC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cpp/stream_g/strstm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cpp/stream_g/strstm_g/str_str.cpp

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
