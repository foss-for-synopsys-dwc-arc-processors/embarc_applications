# libcl arc.av2em.custom
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

AR_NAME=libcl.a
AR_OPT=""

LIST_fio=_bprintf.o _dofopen.o _fclosal.o _fclosfp.o _fd_shrd.o _fgetchr.o\
	_filbuf.o _filelen.o _fileno.o _flushal.o _fputchr.o _freefp.o _iob.o\
	_mktemp.o _tell.o _tempnam.o _ubprntf.o _udofopn.o _ufdopen.o _ufgetc.o\
	_ufgets.o _ufgtchr.o _ufopen.o _ufprintf.o _ufputc.o _ufputch.o\
	_ufputs.o _ufreopn.o _ugetc.o _ugetchr.o _ugets.o _umktemp.o _uopen.o\
	_uperror.o _uprintf.o _uputc.o _uputchr.o _uputs.o _uremove.o _uscanf.o\
	_usprntf.o _usscanf.o _utmpnam.o _uungetc.o _uvfprintf.o _uvprintf.o\
	asprintf.o clearerr.o fclose.o fdopen.o feof.o ferror.o fflush.o\
	fgetc.o fgetpos.o fgets.o filelock.o fopen.o fprintf.o fputc.o fputs.o\
	fread.o freopen.o fseek.o fseek64.o fsetpos.o ftell.o ftell64.o\
	fwrite.o getc.o getchar.o gets.o locks.o perror.o printf.o putc.o\
	putchar.o puts.o rewind.o scanf.o setbuf.o setvbuf.o snprintf.o\
	sprintf.o sscanf.o tmpnam.o tmpfile.o ungetc.o vfprintf.o vprintf.o

LIST_fp=acoshf.o asinhf.o atanhf.o _acosh.o _asinh.o _atan.o _atanf.o _atanh.o\
	_fpclassifyd.o _fpclassifyf.o _hypot.o _isfinited.o _isfinitef.o\
	_isinfd.o _isinff.o _isnand.o _isnanf.o _isnormd.o _isnormf.o _j0.o\
	_j1.o _jn.o _log2.o _pandq0.o _pandq1.o _pneval.o _signbitd.o\
	_signbitf.o _y0.o _y1.o _yn.o asin.o asinf.o atan.o atanf.o atan2.o\
	atan2f.o cbrt.o cbrtf.o ceil.o ceilf.o copysign.o copysignf.o\
	copysignl.o coshf.o erf.o exp.o exp2.o exp10.o expf.o exp2f.o exp10f.o\
	expm1.o expm1f.o fabs.o fabsf.o fdim.o fdimf.o floor.o floorf.o fma.o\
	fmaf.o fmax.o fmaxf.o fmin.o fminf.o fmod.o fmodf.o fp.o hypot.o\
	hypotf.o ilogb.o ilogbf.o isinf.o isinff.o isinfl.o isnan.o isnanf.o\
	isnanl.o lgamma.o lgammaf.o log.o logf.o logb.o logbf.o log1p.o\
	log1pf.o log2.o log2f.o log10.o log10f.o llrint.o llrintf.o lrint.o\
	lrintf.o lround.o lroundf.o llround.o llroundf.o modf.o modff.o nan.o\
	nanf.o nextafter.o nextafterf.o nexttowardf.o pow.o powf.o remainder.o\
	remainderf.o remquo.o remquof.o rint.o rintf.o round.o roundf.o\
	scalbn.o scalbnf.o sin.o sinf.o sinh.o sinhf.o sqrt.o sqrtf.o tan.o\
	tanf.o tanh.o tanhf.o trunc.o truncf.o

LIST_ieee=float.o fpcon.o fpcon2.o fpcon2f.o frexp.o frexpf.o ldexp.o ldexpf.o\
	ufpecvt.o

LIST_misc=_bprt.o crc.o _crotl.o _crotr.o _lfind.o _lrotl.o _lrotr.o _lsearch.o\
	_makepa.o _matherr.o _rotl.o _rotr.o _rterr.o _searche.o _searchs.o\
	_set_mat.o _splitpa.o _srotl.o _srotr.o _swab.o _sys_err.o _arc_lr.o\
	_disable.o _enable.o _mcount.o _mask_vecti.o _setvect.o _sec_setvect.o\
	dcache.o dcache_fblk.o dcache_fl.o dcache_iblk.o dcache_inv.o\
	dcache_lblk.o icache.o icache_iblk.o icache_inv.o icache_lblk.o\
	invcache.o invslcache.o slcache.o slcache_fblk.o slcache_fl.o\
	slcache_iblk.o slcache_inv.o slcache_lblk.o slcache_pblk.o bputil.o\
	gprof_ct.o iprof_ct.o iprofclk.o iprofile.o lzcompress.o lzdecompress.o\
	matherrx.o mwlock.o pictable.o prof_ct.o profrt.o trace.o fenv.o

LIST_extra_objs=_rterr_stub.o crt1cl.o crt1clpt.o crt1cl_ri.o crt1cl_rib.o\
	crt1clpt_ri.o crt1clpt_rib.o crt_argv.o pfmin.o pfnoflt.o sfnoflt.o

LIST_startup=crt_initbss.o initbss.o crt_callmain.o crt_initcopy.o\
	crt_inittimer.o crt_invcache.o mwcall_main.o mwrtl_init.o initpic.o\
	vectors.o xcheck.o

LIST_std=_assert.o _atold.o _compare.o _doatof.o _doatofd.o _doecvt.o\
	_doecvtd.o _doprint.o _doscan.o _fillchr.o _find.o _itoa.o _itos.o\
	_ltoa.o _move.o _pow10.o _pow10d.o _round.o _roundd.o _smalloc.o\
	_stoa.o _stoi.o _strrev.o _u_ltoa.o _uassert.o _uatof.o _uatoi.o\
	_uatol.o _uctypef.o _ucvt.o _ucvtld.o _udoatfd.o _udoatof.o _udoecvd.o\
	_udoecvt.o _udoprnt.o _udoscan.o _ugcvt.o _ugetenv.o _uitoa.o _uitos.o\
	_ultoa.o _ustoi.o _ustrrev.o _ustrtod.o _ustrtof.o _ustrtol.o\
	_ustrtul.o _uultoa.o abort.o setjmp.o arith.o atexit.o atof.o atoi.o\
	atol.o bsearch.o calloc.o cfree.o ctype.o ctypef.o ctype_l.o cvt.o\
	cvt2.o cvtld.o exit.o findchr.o imaxabs.o imaxdiv.o initcopy.o\
	initcopyp.o isascii.o locale.o localeco.o malloc2.o free.o realloc.o\
	mblen.o mbstowcs.o mbtowc.o mkstemp.o nofloatp.o nofloats.o onexit.o\
	qsort.o rand.o setlocal.o signal.o skipchr.o strtod.o strtof.o strtol.o\
	strtoul.o toascii.o wcstombs.o wctomb.o _lltoa.o _lltos.o _stoll.o\
	_u_lltoa.o _u_ulltoa.o _uatoll.o _ulltoa.o _ulltos.o _ustoll.o\
	_ustrtoll.o _ustrtoull.o atoll.o strtoll.o strtoull.o ulltod.o

LIST_str=_memccpy.o _memclr.o _memicmp.o _rmemcpy.o _rstrcpy.o _rstrncp.o\
	_serr_nl.o _strcats.o _strcmpi.o _strdup.o _stricmp.o _strlwr.o\
	_strncat.o _strnicm.o _strnset.o _strset.o _strtokr.o _strupr.o\
	_uchrccp.o _uchrchr.o _uchrcmp.o _uchrcpy.o _uchricm.o _uchrset.o\
	_ustrcat.o _ustrchr.o _ustrcmi.o _ustrcmp.o _ustrcpy.o _ustrcsp.o\
	_ustrdup.o _ustrerr.o _ustricm.o _ustrlen.o _ustrlwr.o _ustrnca.o\
	_ustrncm.o _ustrncp.o _ustrnic.o _ustrnst.o _ustrpbr.o _ustrrch.o\
	_ustrset.o _ustrspn.o _ustrstr.o _ustrtkr.o _vdmemmove.o _vmemset.o\
	_vsdmemmove.o _vsmemmove.o memcpy.o memset.o memchr.o memcmp.o\
	memmove.o strcasecmp.o strcat.o strchr.o strcmp.o strcoll.o strcpy.o\
	strcspn.o strerror.o strlen.o strerror_r.o strerror_s.o strncasecmp.o\
	strncat.o strncmp.o strncpy.o strpbrk.o strrchr.o strspn.o strstr.o\
	strtok.o strxfrm.o

LIST_time=_asctime.o _ctime.o _get_tz.o _gmtime.o _localtm.o _mktime.o\
	_strdate.o _strtime.o _tzset.o asctime.o ctime.o difftime.o gmtime.o\
	localtm.o mktime.o strftime.o

LIST_complex=cmplx.o cmplxf.o cabs.o cabsf.o carg.o cargf.o cacos.o cacosf.o\
	cacosh.o cacoshf.o casin.o casinf.o casinh.o casinhf.o catan.o catanf.o\
	catanh.o catanhf.o ccos.o ccosf.o ccosh.o ccoshf.o cexp.o cexpf.o\
	clog.o clogf.o cimag.o cimagf.o cpow.o cpowf.o cproj.o cprojf.o conj.o\
	conjf.o creal.o crealf.o csin.o csinf.o csinh.o csinhf.o csqrt.o\
	csqrtf.o ctan.o ctanf.o ctanh.o ctanhf.o

LIST_unix=system.o

LIST_wfio=swprintf.o swscanf.o wscanf.o

LIST_wstr=btowc.o mbrlen.o mbrtowc.o mbsinit.o mbsrtowcs.o mbsnrtowcs.o utf8.o\
	wcrtomb.o wcscat.o wcschr.o wcscmp.o wcscoll.o wcscpy.o wcscspn.o\
	wcslen.o wcsncat.o wcsncmp.o wcsncpy.o wcspbrk.o wcsrchr.o wcsnrtombs.o\
	wcsrtombs.o wcsspn.o wcsstr.o wcstod.o wcstof.o wcstok.o wcstol.o\
	wcstoll.o wcstoul.o wcstoull.o wcsxfrm.o wctob.o wctrans.o wctype.o\
	wctype_l.o wmemchr.o wmemcmp.o wmemcpy.o wmemmove.o wmemset.o

LIST_llvm=cxa_atexit.o

LIST_ALL=$(LIST_fio) $(LIST_fp) $(LIST_ieee) $(LIST_misc) $(LIST_startup) $(LIST_std) $(LIST_str) $(LIST_time) $(LIST_complex) $(LIST_unix) $(LIST_wfio) $(LIST_wstr) $(LIST_llvm)

all:  extra_objs $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_fio)
	$(AR) rv $(AR_NAME) $(LIST_fp)
	$(AR) rv $(AR_NAME) $(LIST_ieee)
	$(AR) rv $(AR_NAME) $(LIST_misc)
	$(AR) rv $(AR_NAME) $(LIST_startup)
	$(AR) rv $(AR_NAME) $(LIST_std)
	$(AR) rv $(AR_NAME) $(LIST_str)
	$(AR) rv $(AR_NAME) $(LIST_time)
	$(AR) rv $(AR_NAME) $(LIST_complex)
	$(AR) rv $(AR_NAME) $(LIST_unix)
	$(AR) rv $(AR_NAME) $(LIST_wfio)
	$(AR) rv $(AR_NAME) $(LIST_wstr)
	$(AR) rv $(AR_NAME) $(LIST_llvm)

fio: $(LIST_fio)
fp: $(LIST_fp)
ieee: $(LIST_ieee)
misc: $(LIST_misc)
extra_objs: $(LIST_extra_objs)
startup: $(LIST_startup)
std: $(LIST_std)
str: $(LIST_str)
time: $(LIST_time)
complex: $(LIST_complex)
unix: $(LIST_unix)
wfio: $(LIST_wfio)
wstr: $(LIST_wstr)
llvm: $(LIST_llvm)

_bprintf.o: $(MWLIB)/cl/fio_g/_bprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_bprintf.c

_dofopen.o: $(MWLIB)/cl/fio_g/_dofopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_dofopen.c

_fclosal.o: $(MWLIB)/cl/fio_g/_fclosal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_fclosal.c

_fclosfp.o: $(MWLIB)/cl/fio_g/_fclosfp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_fclosfp.c

_fd_shrd.o: $(MWLIB)/cl/fio_g/_fd_shrd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_fd_shrd.c

_fgetchr.o: $(MWLIB)/cl/fio_g/_fgetchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_fgetchr.c

_filbuf.o: $(MWLIB)/cl/fio_g/_filbuf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_filbuf.c

_filelen.o: $(MWLIB)/cl/fio_g/_filelen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_filelen.c

_fileno.o: $(MWLIB)/cl/fio_g/_fileno.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_fileno.c

_flushal.o: $(MWLIB)/cl/fio_g/_flushal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_flushal.c

_fputchr.o: $(MWLIB)/cl/fio_g/_fputchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_fputchr.c

_freefp.o: $(MWLIB)/cl/fio_g/_freefp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_freefp.c

_iob.o: $(MWLIB)/cl/fio_g/_iob.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_iob.c

_mktemp.o: $(MWLIB)/cl/fio_g/_mktemp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_mktemp.c

_tell.o: $(MWLIB)/cl/fio_g/_tell.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_tell.c

_tempnam.o: $(MWLIB)/cl/fio_g/_tempnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_tempnam.c

_ubprntf.o: $(MWLIB)/cl/fio_g/_ubprntf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ubprntf.c

_udofopn.o: $(MWLIB)/cl/fio_g/_udofopn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_udofopn.c

_ufdopen.o: $(MWLIB)/cl/fio_g/_ufdopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufdopen.c

_ufgetc.o: $(MWLIB)/cl/fio_g/_ufgetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufgetc.c

_ufgets.o: $(MWLIB)/cl/fio_g/_ufgets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufgets.c

_ufgtchr.o: $(MWLIB)/cl/fio_g/_ufgtchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufgtchr.c

_ufopen.o: $(MWLIB)/cl/fio_g/_ufopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufopen.c

_ufprintf.o: $(MWLIB)/cl/fio_g/_ufprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufprintf.c

_ufputc.o: $(MWLIB)/cl/fio_g/_ufputc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufputc.c

_ufputch.o: $(MWLIB)/cl/fio_g/_ufputch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufputch.c

_ufputs.o: $(MWLIB)/cl/fio_g/_ufputs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufputs.c

_ufreopn.o: $(MWLIB)/cl/fio_g/_ufreopn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ufreopn.c

_ugetc.o: $(MWLIB)/cl/fio_g/_ugetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ugetc.c

_ugetchr.o: $(MWLIB)/cl/fio_g/_ugetchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ugetchr.c

_ugets.o: $(MWLIB)/cl/fio_g/_ugets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_ugets.c

_umktemp.o: $(MWLIB)/cl/fio_g/_umktemp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_umktemp.c

_uopen.o: $(MWLIB)/cl/fio_g/_uopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uopen.c

_uperror.o: $(MWLIB)/cl/fio_g/_uperror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uperror.c

_uprintf.o: $(MWLIB)/cl/fio_g/_uprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uprintf.c

_uputc.o: $(MWLIB)/cl/fio_g/_uputc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uputc.c

_uputchr.o: $(MWLIB)/cl/fio_g/_uputchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uputchr.c

_uputs.o: $(MWLIB)/cl/fio_g/_uputs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uputs.c

_uremove.o: $(MWLIB)/cl/fio_g/_uremove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uremove.c

_uscanf.o: $(MWLIB)/cl/fio_g/_uscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uscanf.c

_usprntf.o: $(MWLIB)/cl/fio_g/_usprntf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_usprntf.c

_usscanf.o: $(MWLIB)/cl/fio_g/_usscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_usscanf.c

_utmpnam.o: $(MWLIB)/cl/fio_g/_utmpnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_utmpnam.c

_uungetc.o: $(MWLIB)/cl/fio_g/_uungetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uungetc.c

_uvfprintf.o: $(MWLIB)/cl/fio_g/_uvfprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uvfprintf.c

_uvprintf.o: $(MWLIB)/cl/fio_g/_uvprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/_uvprintf.c

asprintf.o: $(MWLIB)/cl/fio_g/asprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/asprintf.c

clearerr.o: $(MWLIB)/cl/fio_g/clearerr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/clearerr.c

fclose.o: $(MWLIB)/cl/fio_g/fclose.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fclose.c

fdopen.o: $(MWLIB)/cl/fio_g/fdopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fdopen.c

feof.o: $(MWLIB)/cl/fio_g/feof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/feof.c

ferror.o: $(MWLIB)/cl/fio_g/ferror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/ferror.c

fflush.o: $(MWLIB)/cl/fio_g/fflush.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fflush.c

fgetc.o: $(MWLIB)/cl/fio_g/fgetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fgetc.c

fgetpos.o: $(MWLIB)/cl/fio_g/fgetpos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fgetpos.c

fgets.o: $(MWLIB)/cl/fio_g/fgets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fgets.c

filelock.o: $(MWLIB)/cl/fio_g/filelock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/filelock.c

fopen.o: $(MWLIB)/cl/fio_g/fopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fopen.c

fprintf.o: $(MWLIB)/cl/fio_g/fprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fprintf.c

fputc.o: $(MWLIB)/cl/fio_g/fputc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fputc.c

fputs.o: $(MWLIB)/cl/fio_g/fputs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fputs.c

fread.o: $(MWLIB)/cl/fio_g/fread.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fread.c

freopen.o: $(MWLIB)/cl/fio_g/freopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/freopen.c

fseek.o: $(MWLIB)/cl/fio_g/fseek.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fseek.c

fseek64.o: $(MWLIB)/cl/fio_g/fseek64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fseek64.c

fsetpos.o: $(MWLIB)/cl/fio_g/fsetpos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fsetpos.c

ftell.o: $(MWLIB)/cl/fio_g/ftell.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/ftell.c

ftell64.o: $(MWLIB)/cl/fio_g/ftell64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/ftell64.c

fwrite.o: $(MWLIB)/cl/fio_g/fwrite.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/fwrite.c

getc.o: $(MWLIB)/cl/fio_g/getc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/getc.c

getchar.o: $(MWLIB)/cl/fio_g/getchar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/getchar.c

gets.o: $(MWLIB)/cl/fio_g/gets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/gets.c

locks.o: $(MWLIB)/cl/fio_g/locks.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/locks.c

perror.o: $(MWLIB)/cl/fio_g/perror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/perror.c

printf.o: $(MWLIB)/cl/fio_g/printf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/printf.c

putc.o: $(MWLIB)/cl/fio_g/putc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/putc.c

putchar.o: $(MWLIB)/cl/fio_g/putchar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/putchar.c

puts.o: $(MWLIB)/cl/fio_g/puts.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/puts.c

rewind.o: $(MWLIB)/cl/fio_g/rewind.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/rewind.c

scanf.o: $(MWLIB)/cl/fio_g/scanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/scanf.c

setbuf.o: $(MWLIB)/cl/fio_g/setbuf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/setbuf.c

setvbuf.o: $(MWLIB)/cl/fio_g/setvbuf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/setvbuf.c

snprintf.o: $(MWLIB)/cl/fio_g/snprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/snprintf.c

sprintf.o: $(MWLIB)/cl/fio_g/sprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/sprintf.c

sscanf.o: $(MWLIB)/cl/fio_g/sscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/sscanf.c

tmpnam.o: $(MWLIB)/cl/fio_g/tmpnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/tmpnam.c

tmpfile.o: $(MWLIB)/cl/fio_g/tmpfile.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/tmpfile.c

ungetc.o: $(MWLIB)/cl/fio_g/ungetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/ungetc.c

vfprintf.o: $(MWLIB)/cl/fio_g/vfprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/vfprintf.c

vprintf.o: $(MWLIB)/cl/fio_g/vprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fio_g/vprintf.c

acoshf.o: $(MWLIB)/cl/fp_g/acoshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/acoshf.c

asinhf.o: $(MWLIB)/cl/fp_g/asinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/asinhf.c

atanhf.o: $(MWLIB)/cl/fp_g/atanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/atanhf.c

_acosh.o: $(MWLIB)/cl/fp_g/_acosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_acosh.c

_asinh.o: $(MWLIB)/cl/fp_g/_asinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_asinh.c

_atan.o: $(MWLIB)/cl/fp_g/_atan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_atan.c

_atanf.o: $(MWLIB)/cl/fp_g/_atanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_atanf.c

_atanh.o: $(MWLIB)/cl/fp_g/_atanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_atanh.c

_fpclassifyd.o: $(MWLIB)/cl/fp_g/_fpclassifyd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_fpclassifyd.c

_fpclassifyf.o: $(MWLIB)/cl/fp_g/_fpclassifyf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_fpclassifyf.c

_hypot.o: $(MWLIB)/cl/fp_g/_hypot.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_hypot.c

_isfinited.o: $(MWLIB)/cl/fp_g/_isfinited.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isfinited.c

_isfinitef.o: $(MWLIB)/cl/fp_g/_isfinitef.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isfinitef.c

_isinfd.o: $(MWLIB)/cl/fp_g/_isinfd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isinfd.c

_isinff.o: $(MWLIB)/cl/fp_g/_isinff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isinff.c

_isnand.o: $(MWLIB)/cl/fp_g/_isnand.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isnand.c

_isnanf.o: $(MWLIB)/cl/fp_g/_isnanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isnanf.c

_isnormd.o: $(MWLIB)/cl/fp_g/_isnormd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isnormd.c

_isnormf.o: $(MWLIB)/cl/fp_g/_isnormf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_isnormf.c

_j0.o: $(MWLIB)/cl/fp_g/_j0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_j0.c

_j1.o: $(MWLIB)/cl/fp_g/_j1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_j1.c

_jn.o: $(MWLIB)/cl/fp_g/_jn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_jn.c

_log2.o: $(MWLIB)/cl/fp_g/_log2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_log2.c

_pandq0.o: $(MWLIB)/cl/fp_g/_pandq0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_pandq0.c

_pandq1.o: $(MWLIB)/cl/fp_g/_pandq1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_pandq1.c

_pneval.o: $(MWLIB)/cl/fp_g/_pneval.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_pneval.c

_signbitd.o: $(MWLIB)/cl/fp_g/_signbitd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_signbitd.c

_signbitf.o: $(MWLIB)/cl/fp_g/_signbitf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_signbitf.c

_y0.o: $(MWLIB)/cl/fp_g/_y0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_y0.c

_y1.o: $(MWLIB)/cl/fp_g/_y1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_y1.c

_yn.o: $(MWLIB)/cl/fp_g/_yn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/_yn.c

asin.o: $(MWLIB)/cl/fp_g/asin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/asin.c

asinf.o: $(MWLIB)/cl/fp_g/asinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/asinf.c

atan.o: $(MWLIB)/cl/fp_g/atan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/atan.c

atanf.o: $(MWLIB)/cl/fp_g/atanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/atanf.c

atan2.o: $(MWLIB)/cl/fp_g/atan2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/atan2.c

atan2f.o: $(MWLIB)/cl/fp_g/atan2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/atan2f.c

cbrt.o: $(MWLIB)/cl/fp_g/cbrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/cbrt.c

cbrtf.o: $(MWLIB)/cl/fp_g/cbrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/cbrtf.c

ceil.o: $(MWLIB)/cl/fp_g/ceil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/ceil.c

ceilf.o: $(MWLIB)/cl/fp_g/ceilf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/ceilf.c

copysign.o: $(MWLIB)/cl/fp_g/copysign.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/copysign.c

copysignf.o: $(MWLIB)/cl/fp_g/copysignf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/copysignf.c

copysignl.o: $(MWLIB)/cl/fp_g/copysignl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/copysignl.c

coshf.o: $(MWLIB)/cl/fp_g/coshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/coshf.c

erf.o: $(MWLIB)/cl/fp_g/erf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/erf.c

exp.o: $(MWLIB)/cl/fp_g/exp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/exp.c

exp2.o: $(MWLIB)/cl/fp_g/exp2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/exp2.c

exp10.o: $(MWLIB)/cl/fp_g/exp10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/exp10.c

expf.o: $(MWLIB)/cl/fp_g/expf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/expf.c

exp2f.o: $(MWLIB)/cl/fp_g/exp2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/exp2f.c

exp10f.o: $(MWLIB)/cl/fp_g/exp10f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/exp10f.c

expm1.o: $(MWLIB)/cl/fp_g/expm1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/expm1.c

expm1f.o: $(MWLIB)/cl/fp_g/expm1f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/expm1f.c

fabs.o: $(MWLIB)/cl/fp_g/fabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fabs.c

fabsf.o: $(MWLIB)/cl/fp_g/fabsf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fabsf.c

fdim.o: $(MWLIB)/cl/fp_g/fdim.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fdim.c

fdimf.o: $(MWLIB)/cl/fp_g/fdimf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fdimf.c

floor.o: $(MWLIB)/cl/fp_g/floor.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/floor.c

floorf.o: $(MWLIB)/cl/fp_g/floorf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/floorf.c

fma.o: $(MWLIB)/cl/fp_g/fma.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fma.c

fmaf.o: $(MWLIB)/cl/fp_g/fmaf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fmaf.c

fmax.o: $(MWLIB)/cl/fp_g/fmax.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fmax.c

fmaxf.o: $(MWLIB)/cl/fp_g/fmaxf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fmaxf.c

fmin.o: $(MWLIB)/cl/fp_g/fmin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fmin.c

fminf.o: $(MWLIB)/cl/fp_g/fminf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fminf.c

fmod.o: $(MWLIB)/cl/fp_g/fmod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fmod.c

fmodf.o: $(MWLIB)/cl/fp_g/fmodf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fmodf.c

fp.o: $(MWLIB)/cl/fp_g/fp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/fp.c

hypot.o: $(MWLIB)/cl/fp_g/hypot.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/hypot.c

hypotf.o: $(MWLIB)/cl/fp_g/hypotf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/hypotf.c

ilogb.o: $(MWLIB)/cl/fp_g/ilogb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/ilogb.c

ilogbf.o: $(MWLIB)/cl/fp_g/ilogbf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/ilogbf.c

isinf.o: $(MWLIB)/cl/fp_g/isinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/isinf.c

isinff.o: $(MWLIB)/cl/fp_g/isinff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/isinff.c

isinfl.o: $(MWLIB)/cl/fp_g/isinfl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/isinfl.c

isnan.o: $(MWLIB)/cl/fp_g/isnan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/isnan.c

isnanf.o: $(MWLIB)/cl/fp_g/isnanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/isnanf.c

isnanl.o: $(MWLIB)/cl/fp_g/isnanl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/isnanl.c

lgamma.o: $(MWLIB)/cl/fp_g/lgamma.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/lgamma.c

lgammaf.o: $(MWLIB)/cl/fp_g/lgammaf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/lgammaf.c

log.o: $(MWLIB)/cl/fp_g/log.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log.c

logf.o: $(MWLIB)/cl/fp_g/logf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/logf.c

logb.o: $(MWLIB)/cl/fp_g/logb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/logb.c

logbf.o: $(MWLIB)/cl/fp_g/logbf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/logbf.c

log1p.o: $(MWLIB)/cl/fp_g/log1p.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log1p.c

log1pf.o: $(MWLIB)/cl/fp_g/log1pf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log1pf.c

log2.o: $(MWLIB)/cl/fp_g/log2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log2.c

log2f.o: $(MWLIB)/cl/fp_g/log2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log2f.c

log10.o: $(MWLIB)/cl/fp_g/log10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log10.c

log10f.o: $(MWLIB)/cl/fp_g/log10f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/log10f.c

llrint.o: $(MWLIB)/cl/fp_g/llrint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/llrint.c

llrintf.o: $(MWLIB)/cl/fp_g/llrintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/llrintf.c

lrint.o: $(MWLIB)/cl/fp_g/lrint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/lrint.c

lrintf.o: $(MWLIB)/cl/fp_g/lrintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/lrintf.c

lround.o: $(MWLIB)/cl/fp_g/lround.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/lround.c

lroundf.o: $(MWLIB)/cl/fp_g/lroundf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/lroundf.c

llround.o: $(MWLIB)/cl/fp_g/llround.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/llround.c

llroundf.o: $(MWLIB)/cl/fp_g/llroundf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/llroundf.c

modf.o: $(MWLIB)/cl/fp_g/modf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/modf.c

modff.o: $(MWLIB)/cl/fp_g/modff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/modff.c

nan.o: $(MWLIB)/cl/fp_g/nan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/nan.c

nanf.o: $(MWLIB)/cl/fp_g/nanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/nanf.c

nextafter.o: $(MWLIB)/cl/fp_g/nextafter.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/nextafter.c

nextafterf.o: $(MWLIB)/cl/fp_g/nextafterf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/nextafterf.c

nexttowardf.o: $(MWLIB)/cl/fp_g/nexttowardf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/nexttowardf.c

pow.o: $(MWLIB)/cl/fp_g/pow.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/pow.c

powf.o: $(MWLIB)/cl/fp_g/powf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/powf.c

remainder.o: $(MWLIB)/cl/fp_g/remainder.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/remainder.c

remainderf.o: $(MWLIB)/cl/fp_g/remainderf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/remainderf.c

remquo.o: $(MWLIB)/cl/fp_g/remquo.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/remquo.c

remquof.o: $(MWLIB)/cl/fp_g/remquof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/remquof.c

rint.o: $(MWLIB)/cl/fp_g/rint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/rint.c

rintf.o: $(MWLIB)/cl/fp_g/rintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/rintf.c

round.o: $(MWLIB)/cl/fp_g/round.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/round.c

roundf.o: $(MWLIB)/cl/fp_g/roundf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/roundf.c

scalbn.o: $(MWLIB)/cl/fp_g/scalbn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/scalbn.c

scalbnf.o: $(MWLIB)/cl/fp_g/scalbnf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/scalbnf.c

sin.o: $(MWLIB)/cl/fp_g/sin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/sin.c

sinf.o: $(MWLIB)/cl/fp_g/sinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/sinf.c

sinh.o: $(MWLIB)/cl/fp_g/sinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/sinh.c

sinhf.o: $(MWLIB)/cl/fp_g/sinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/sinhf.c

sqrt.o: $(MWLIB)/cl/fp_g/sqrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/sqrt.c

sqrtf.o: $(MWLIB)/cl/fp_g/sqrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/sqrtf.c

tan.o: $(MWLIB)/cl/fp_g/tan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/tan.c

tanf.o: $(MWLIB)/cl/fp_g/tanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/tanf.c

tanh.o: $(MWLIB)/cl/fp_g/tanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/tanh.c

tanhf.o: $(MWLIB)/cl/fp_g/tanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/tanhf.c

trunc.o: $(MWLIB)/cl/fp_g/trunc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/trunc.c

truncf.o: $(MWLIB)/cl/fp_g/truncf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/fp_g/truncf.c

float.o: $(MWLIB)/cl/ieee_g/float.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/float.c

fpcon.o: $(MWLIB)/cl/ieee_g/fpcon.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/fpcon.c

fpcon2.o: $(MWLIB)/cl/ieee_g/fpcon2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/fpcon2.c

fpcon2f.o: $(MWLIB)/cl/ieee_g/fpcon2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/fpcon2f.c

frexp.o: $(MWLIB)/cl/ieee_g/frexp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/frexp.c

frexpf.o: $(MWLIB)/cl/ieee_g/frexpf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/frexpf.c

ldexp.o: $(MWLIB)/cl/ieee_g/ldexp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/ldexp.c

ldexpf.o: $(MWLIB)/cl/ieee_g/ldexpf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/ldexpf.c

ufpecvt.o: $(MWLIB)/cl/ieee_g/ufpecvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/ieee_g/ufpecvt.c

_bprt.o: $(MWLIB)/cl/misc_g/_bprt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_bprt.c

crc.o: $(MWLIB)/cl/misc_g/crc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/crc.c

_crotl.o: $(MWLIB)/cl/misc_g/_crotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_crotl.c

_crotr.o: $(MWLIB)/cl/misc_g/_crotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_crotr.c

_lfind.o: $(MWLIB)/cl/misc_g/_lfind.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_lfind.c

_lrotl.o: $(MWLIB)/cl/misc_g/_lrotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_lrotl.c

_lrotr.o: $(MWLIB)/cl/misc_g/_lrotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_lrotr.c

_lsearch.o: $(MWLIB)/cl/misc_g/_lsearch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_lsearch.c

_makepa.o: $(MWLIB)/cl/misc_g/_makepa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_makepa.c

_matherr.o: $(MWLIB)/cl/misc_g/_matherr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_matherr.c

_rotl.o: $(MWLIB)/cl/misc_g/_rotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_rotl.c

_rotr.o: $(MWLIB)/cl/misc_g/_rotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_rotr.c

_rterr.o: $(MWLIB)/cl/misc_g/_rterr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_rterr.c

_rterr_stub.o: $(MWLIB)/cl/misc_g/_rterr_stub.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_rterr_stub.c

_searche.o: $(MWLIB)/cl/misc_g/_searche.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_searche.c

_searchs.o: $(MWLIB)/cl/misc_g/_searchs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_searchs.c

_set_mat.o: $(MWLIB)/cl/misc_g/_set_mat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_set_mat.c

_splitpa.o: $(MWLIB)/cl/misc_g/_splitpa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_splitpa.c

_srotl.o: $(MWLIB)/cl/misc_g/_srotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_srotl.c

_srotr.o: $(MWLIB)/cl/misc_g/_srotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_srotr.c

_swab.o: $(MWLIB)/cl/misc_g/_swab.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_swab.c

_sys_err.o: $(MWLIB)/cl/misc_g/_sys_err.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_sys_err.c

_arc_lr.o: $(MWLIB)/cl/misc_g/_arc_lr.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/misc_g/_arc_lr.s

_disable.o: $(MWLIB)/cl/misc_g/_disable.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/misc_g/_disable.s

_enable.o: $(MWLIB)/cl/misc_g/_enable.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/misc_g/_enable.s

_mcount.o: $(MWLIB)/cl/misc_g/_mcount.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/misc_g/_mcount.s

_mask_vecti.o: $(MWLIB)/cl/misc_g/_mask_vecti.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_mask_vecti.c

_setvect.o: $(MWLIB)/cl/misc_g/_setvect.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/_setvect.c

_sec_setvect.o: $(MWLIB)/cl/misc_g/_sec_setvect.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) -Xsec_modes $(MWLIB)/cl/misc_g/_sec_setvect.c

dcache.o: $(MWLIB)/cl/misc_g/dcache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/dcache.c

dcache_fblk.o: $(MWLIB)/cl/misc_g/dcache_fblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/dcache_fblk.c

dcache_fl.o: $(MWLIB)/cl/misc_g/dcache_fl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/dcache_fl.c

dcache_iblk.o: $(MWLIB)/cl/misc_g/dcache_iblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/dcache_iblk.c

dcache_inv.o: $(MWLIB)/cl/misc_g/dcache_inv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/dcache_inv.c

dcache_lblk.o: $(MWLIB)/cl/misc_g/dcache_lblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/dcache_lblk.c

icache.o: $(MWLIB)/cl/misc_g/icache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/icache.c

icache_iblk.o: $(MWLIB)/cl/misc_g/icache_iblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/icache_iblk.c

icache_inv.o: $(MWLIB)/cl/misc_g/icache_inv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/icache_inv.c

icache_lblk.o: $(MWLIB)/cl/misc_g/icache_lblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/icache_lblk.c

invcache.o: $(MWLIB)/cl/misc_g/invcache.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/misc_g/invcache.s

invslcache.o: $(MWLIB)/cl/misc_g/invslcache.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/misc_g/invslcache.s

slcache.o: $(MWLIB)/cl/misc_g/slcache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache.c

slcache_fblk.o: $(MWLIB)/cl/misc_g/slcache_fblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache_fblk.c

slcache_fl.o: $(MWLIB)/cl/misc_g/slcache_fl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache_fl.c

slcache_iblk.o: $(MWLIB)/cl/misc_g/slcache_iblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache_iblk.c

slcache_inv.o: $(MWLIB)/cl/misc_g/slcache_inv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache_inv.c

slcache_lblk.o: $(MWLIB)/cl/misc_g/slcache_lblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache_lblk.c

slcache_pblk.o: $(MWLIB)/cl/misc_g/slcache_pblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/slcache_pblk.c

bputil.o: $(MWLIB)/cl/misc_g/bputil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/bputil.c

gprof_ct.o: $(MWLIB)/cl/misc_g/gprof_ct.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/gprof_ct.c

iprof_ct.o: $(MWLIB)/cl/misc_g/iprof_ct.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/iprof_ct.c

iprofclk.o: $(MWLIB)/cl/misc_g/iprofclk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/iprofclk.c

iprofile.o: $(MWLIB)/cl/misc_g/iprofile.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/iprofile.c

lzcompress.o: $(MWLIB)/cl/misc_g/lzcompress.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/lzcompress.c

lzdecompress.o: $(MWLIB)/cl/misc_g/lzdecompress.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/lzdecompress.c

matherrx.o: $(MWLIB)/cl/misc_g/matherrx.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/matherrx.c

mwlock.o: $(MWLIB)/cl/misc_g/mwlock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/mwlock.c

pictable.o: $(MWLIB)/cl/misc_g/pictable.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/pictable.c

prof_ct.o: $(MWLIB)/cl/misc_g/prof_ct.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/prof_ct.c

profrt.o: $(MWLIB)/cl/misc_g/profrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/profrt.c

trace.o: $(MWLIB)/cl/misc_g/trace.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/trace.c

crt1cl.o: $(MWLIB)/cl/startup_g/crt1cl.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt1cl.s

crt1clpt.o: $(MWLIB)/cl/startup_g/crt1clpt.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt1clpt.s

crt1cl_ri.o: $(MWLIB)/cl/startup_g/crt1cl_ri.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt1cl_ri.s

crt1cl_rib.o: $(MWLIB)/cl/startup_g/crt1cl_rib.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt1cl_rib.s

crt1clpt_ri.o: $(MWLIB)/cl/startup_g/crt1clpt_ri.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt1clpt_ri.s

crt1clpt_rib.o: $(MWLIB)/cl/startup_g/crt1clpt_rib.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt1clpt_rib.s

crt_initbss.o: $(MWLIB)/cl/startup_g/crt_initbss.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt_initbss.s

initbss.o: $(MWLIB)/cl/startup_g/initbss.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/startup_g/initbss.c

crt_callmain.o: $(MWLIB)/cl/startup_g/crt_callmain.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt_callmain.s

crt_initcopy.o: $(MWLIB)/cl/startup_g/crt_initcopy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt_initcopy.s

crt_inittimer.o: $(MWLIB)/cl/startup_g/crt_inittimer.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt_inittimer.s

crt_invcache.o: $(MWLIB)/cl/startup_g/crt_invcache.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt_invcache.s

crt_argv.o: $(MWLIB)/cl/startup_g/crt_argv.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/crt_argv.s

mwcall_main.o: $(MWLIB)/cl/startup_g/mwcall_main.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/startup_g/mwcall_main.c

mwrtl_init.o: $(MWLIB)/cl/startup_g/mwrtl_init.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/startup_g/mwrtl_init.c

initpic.o: $(MWLIB)/cl/startup_g/initpic.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/initpic.s

vectors.o: $(MWLIB)/cl/startup_g/vectors.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/vectors.s

xcheck.o: $(MWLIB)/cl/startup_g/xcheck.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/startup_g/xcheck.s

_assert.o: $(MWLIB)/cl/std_g/_assert.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_assert.c

_atold.o: $(MWLIB)/cl/std_g/_atold.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_atold.c

_compare.o: $(MWLIB)/cl/std_g/_compare.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_compare.c

_doatof.o: $(MWLIB)/cl/std_g/_doatof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_doatof.c

_doatofd.o: $(MWLIB)/cl/std_g/_doatofd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_doatofd.c

_doecvt.o: $(MWLIB)/cl/std_g/_doecvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_doecvt.c

_doecvtd.o: $(MWLIB)/cl/std_g/_doecvtd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_doecvtd.c

_doprint.o: $(MWLIB)/cl/std_g/_doprint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) -U_DSPC $(MWLIB)/cl/std_g/_doprint.c

_doscan.o: $(MWLIB)/cl/std_g/_doscan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) -U_DSPC $(MWLIB)/cl/std_g/_doscan.c

_fillchr.o: $(MWLIB)/cl/std_g/_fillchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_fillchr.c

_find.o: $(MWLIB)/cl/std_g/_find.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_find.c

_itoa.o: $(MWLIB)/cl/std_g/_itoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_itoa.c

_itos.o: $(MWLIB)/cl/std_g/_itos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_itos.c

_ltoa.o: $(MWLIB)/cl/std_g/_ltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ltoa.c

_move.o: $(MWLIB)/cl/std_g/_move.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_move.c

_pow10.o: $(MWLIB)/cl/std_g/_pow10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_pow10.c

_pow10d.o: $(MWLIB)/cl/std_g/_pow10d.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_pow10d.c

_round.o: $(MWLIB)/cl/std_g/_round.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_round.c

_roundd.o: $(MWLIB)/cl/std_g/_roundd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_roundd.c

_smalloc.o: $(MWLIB)/cl/std_g/_smalloc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_smalloc.c

_stoa.o: $(MWLIB)/cl/std_g/_stoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_stoa.c

_stoi.o: $(MWLIB)/cl/std_g/_stoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_stoi.c

_strrev.o: $(MWLIB)/cl/std_g/_strrev.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_strrev.c

_u_ltoa.o: $(MWLIB)/cl/std_g/_u_ltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_u_ltoa.c

_uassert.o: $(MWLIB)/cl/std_g/_uassert.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uassert.c

_uatof.o: $(MWLIB)/cl/std_g/_uatof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uatof.c

_uatoi.o: $(MWLIB)/cl/std_g/_uatoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uatoi.c

_uatol.o: $(MWLIB)/cl/std_g/_uatol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uatol.c

_uctypef.o: $(MWLIB)/cl/std_g/_uctypef.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uctypef.c

_ucvt.o: $(MWLIB)/cl/std_g/_ucvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ucvt.c

_ucvtld.o: $(MWLIB)/cl/std_g/_ucvtld.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ucvtld.c

_udoatfd.o: $(MWLIB)/cl/std_g/_udoatfd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_udoatfd.c

_udoatof.o: $(MWLIB)/cl/std_g/_udoatof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_udoatof.c

_udoecvd.o: $(MWLIB)/cl/std_g/_udoecvd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_udoecvd.c

_udoecvt.o: $(MWLIB)/cl/std_g/_udoecvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_udoecvt.c

_udoprnt.o: $(MWLIB)/cl/std_g/_udoprnt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_udoprnt.c

_udoscan.o: $(MWLIB)/cl/std_g/_udoscan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_udoscan.c

_ugcvt.o: $(MWLIB)/cl/std_g/_ugcvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ugcvt.c

_ugetenv.o: $(MWLIB)/cl/std_g/_ugetenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ugetenv.c

_uitoa.o: $(MWLIB)/cl/std_g/_uitoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uitoa.c

_uitos.o: $(MWLIB)/cl/std_g/_uitos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uitos.c

_ultoa.o: $(MWLIB)/cl/std_g/_ultoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ultoa.c

_ustoi.o: $(MWLIB)/cl/std_g/_ustoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustoi.c

_ustrrev.o: $(MWLIB)/cl/std_g/_ustrrev.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrrev.c

_ustrtod.o: $(MWLIB)/cl/std_g/_ustrtod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrtod.c

_ustrtof.o: $(MWLIB)/cl/std_g/_ustrtof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrtof.c

_ustrtol.o: $(MWLIB)/cl/std_g/_ustrtol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrtol.c

_ustrtul.o: $(MWLIB)/cl/std_g/_ustrtul.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrtul.c

_uultoa.o: $(MWLIB)/cl/std_g/_uultoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uultoa.c

abort.o: $(MWLIB)/cl/std_g/abort.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/abort.c

setjmp.o: $(MWLIB)/cl/std_g/setjmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/cl/std_g/setjmp.s

arith.o: $(MWLIB)/cl/std_g/arith.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/arith.c

atexit.o: $(MWLIB)/cl/std_g/atexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/atexit.c

atof.o: $(MWLIB)/cl/std_g/atof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/atof.c

atoi.o: $(MWLIB)/cl/std_g/atoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/atoi.c

atol.o: $(MWLIB)/cl/std_g/atol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/atol.c

bsearch.o: $(MWLIB)/cl/std_g/bsearch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/bsearch.c

calloc.o: $(MWLIB)/cl/std_g/calloc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/calloc.c

cfree.o: $(MWLIB)/cl/std_g/cfree.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/cfree.c

ctype.o: $(MWLIB)/cl/std_g/ctype.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/ctype.c

ctypef.o: $(MWLIB)/cl/std_g/ctypef.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/ctypef.c

ctype_l.o: $(MWLIB)/cl/std_g/ctype_l.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/ctype_l.c

cvt.o: $(MWLIB)/cl/std_g/cvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/cvt.c

cvt2.o: $(MWLIB)/cl/std_g/cvt2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/cvt2.c

cvtld.o: $(MWLIB)/cl/std_g/cvtld.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/cvtld.c

exit.o: $(MWLIB)/cl/std_g/exit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/exit.c

findchr.o: $(MWLIB)/cl/std_g/findchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/findchr.c

imaxabs.o: $(MWLIB)/cl/std_g/imaxabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/imaxabs.c

imaxdiv.o: $(MWLIB)/cl/std_g/imaxdiv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/imaxdiv.c

initcopy.o: $(MWLIB)/cl/std_g/initcopy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/initcopy.c

initcopyp.o: $(MWLIB)/cl/std_g/initcopyp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/initcopyp.c

isascii.o: $(MWLIB)/cl/std_g/isascii.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/isascii.c

locale.o: $(MWLIB)/cl/std_g/locale.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/locale.c

localeco.o: $(MWLIB)/cl/std_g/localeco.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/localeco.c

malloc2.o: $(MWLIB)/cl/std_g/malloc2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/malloc2.c

free.o: $(MWLIB)/cl/std_g/free.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/free.c

realloc.o: $(MWLIB)/cl/std_g/realloc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/realloc.c

mblen.o: $(MWLIB)/cl/std_g/mblen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/mblen.c

mbstowcs.o: $(MWLIB)/cl/std_g/mbstowcs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/mbstowcs.c

mbtowc.o: $(MWLIB)/cl/std_g/mbtowc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/mbtowc.c

mkstemp.o: $(MWLIB)/cl/std_g/mkstemp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/mkstemp.c

nofloatp.o: $(MWLIB)/cl/std_g/nofloatp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/nofloatp.c

nofloats.o: $(MWLIB)/cl/std_g/nofloats.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/nofloats.c

onexit.o: $(MWLIB)/cl/std_g/onexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/onexit.c

pfmin.o: $(MWLIB)/cl/std_g/pfmin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/pfmin.c

pfnoflt.o: $(MWLIB)/cl/std_g/pfnoflt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/pfnoflt.c

qsort.o: $(MWLIB)/cl/std_g/qsort.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/qsort.c

rand.o: $(MWLIB)/cl/std_g/rand.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/rand.c

setlocal.o: $(MWLIB)/cl/std_g/setlocal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/setlocal.c

sfnoflt.o: $(MWLIB)/cl/std_g/sfnoflt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/sfnoflt.c

signal.o: $(MWLIB)/cl/std_g/signal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/signal.c

skipchr.o: $(MWLIB)/cl/std_g/skipchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/skipchr.c

strtod.o: $(MWLIB)/cl/std_g/strtod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/strtod.c

strtof.o: $(MWLIB)/cl/std_g/strtof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/strtof.c

strtol.o: $(MWLIB)/cl/std_g/strtol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/strtol.c

strtoul.o: $(MWLIB)/cl/std_g/strtoul.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/strtoul.c

toascii.o: $(MWLIB)/cl/std_g/toascii.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/toascii.c

wcstombs.o: $(MWLIB)/cl/std_g/wcstombs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/wcstombs.c

wctomb.o: $(MWLIB)/cl/std_g/wctomb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/wctomb.c

_lltoa.o: $(MWLIB)/cl/std_g/_lltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_lltoa.c

_lltos.o: $(MWLIB)/cl/std_g/_lltos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_lltos.c

_stoll.o: $(MWLIB)/cl/std_g/_stoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_stoll.c

_u_lltoa.o: $(MWLIB)/cl/std_g/_u_lltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_u_lltoa.c

_u_ulltoa.o: $(MWLIB)/cl/std_g/_u_ulltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_u_ulltoa.c

_uatoll.o: $(MWLIB)/cl/std_g/_uatoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_uatoll.c

_ulltoa.o: $(MWLIB)/cl/std_g/_ulltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ulltoa.c

_ulltos.o: $(MWLIB)/cl/std_g/_ulltos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ulltos.c

_ustoll.o: $(MWLIB)/cl/std_g/_ustoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustoll.c

_ustrtoll.o: $(MWLIB)/cl/std_g/_ustrtoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrtoll.c

_ustrtoull.o: $(MWLIB)/cl/std_g/_ustrtoull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/_ustrtoull.c

atoll.o: $(MWLIB)/cl/std_g/atoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/atoll.c

strtoll.o: $(MWLIB)/cl/std_g/strtoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/strtoll.c

strtoull.o: $(MWLIB)/cl/std_g/strtoull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/strtoull.c

ulltod.o: $(MWLIB)/cl/std_g/ulltod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/std_g/ulltod.c

_memccpy.o: $(MWLIB)/cl/str_g/_memccpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_memccpy.c

_memclr.o: $(MWLIB)/cl/str_g/_memclr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_memclr.c

_memicmp.o: $(MWLIB)/cl/str_g/_memicmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_memicmp.c

_rmemcpy.o: $(MWLIB)/cl/str_g/_rmemcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_rmemcpy.c

_rstrcpy.o: $(MWLIB)/cl/str_g/_rstrcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_rstrcpy.c

_rstrncp.o: $(MWLIB)/cl/str_g/_rstrncp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_rstrncp.c

_serr_nl.o: $(MWLIB)/cl/str_g/_serr_nl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_serr_nl.c

_strcats.o: $(MWLIB)/cl/str_g/_strcats.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strcats.c

_strcmpi.o: $(MWLIB)/cl/str_g/_strcmpi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strcmpi.c

_strdup.o: $(MWLIB)/cl/str_g/_strdup.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strdup.c

_stricmp.o: $(MWLIB)/cl/str_g/_stricmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_stricmp.c

_strlwr.o: $(MWLIB)/cl/str_g/_strlwr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strlwr.c

_strncat.o: $(MWLIB)/cl/str_g/_strncat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strncat.c

_strnicm.o: $(MWLIB)/cl/str_g/_strnicm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strnicm.c

_strnset.o: $(MWLIB)/cl/str_g/_strnset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strnset.c

_strset.o: $(MWLIB)/cl/str_g/_strset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strset.c

_strtokr.o: $(MWLIB)/cl/str_g/_strtokr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strtokr.c

_strupr.o: $(MWLIB)/cl/str_g/_strupr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_strupr.c

_uchrccp.o: $(MWLIB)/cl/str_g/_uchrccp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_uchrccp.c

_uchrchr.o: $(MWLIB)/cl/str_g/_uchrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_uchrchr.c

_uchrcmp.o: $(MWLIB)/cl/str_g/_uchrcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_uchrcmp.c

_uchrcpy.o: $(MWLIB)/cl/str_g/_uchrcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_uchrcpy.c

_uchricm.o: $(MWLIB)/cl/str_g/_uchricm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_uchricm.c

_uchrset.o: $(MWLIB)/cl/str_g/_uchrset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_uchrset.c

_ustrcat.o: $(MWLIB)/cl/str_g/_ustrcat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrcat.c

_ustrchr.o: $(MWLIB)/cl/str_g/_ustrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrchr.c

_ustrcmi.o: $(MWLIB)/cl/str_g/_ustrcmi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrcmi.c

_ustrcmp.o: $(MWLIB)/cl/str_g/_ustrcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrcmp.c

_ustrcpy.o: $(MWLIB)/cl/str_g/_ustrcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrcpy.c

_ustrcsp.o: $(MWLIB)/cl/str_g/_ustrcsp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrcsp.c

_ustrdup.o: $(MWLIB)/cl/str_g/_ustrdup.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrdup.c

_ustrerr.o: $(MWLIB)/cl/str_g/_ustrerr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrerr.c

_ustricm.o: $(MWLIB)/cl/str_g/_ustricm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustricm.c

_ustrlen.o: $(MWLIB)/cl/str_g/_ustrlen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrlen.c

_ustrlwr.o: $(MWLIB)/cl/str_g/_ustrlwr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrlwr.c

_ustrnca.o: $(MWLIB)/cl/str_g/_ustrnca.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrnca.c

_ustrncm.o: $(MWLIB)/cl/str_g/_ustrncm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrncm.c

_ustrncp.o: $(MWLIB)/cl/str_g/_ustrncp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrncp.c

_ustrnic.o: $(MWLIB)/cl/str_g/_ustrnic.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrnic.c

_ustrnst.o: $(MWLIB)/cl/str_g/_ustrnst.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrnst.c

_ustrpbr.o: $(MWLIB)/cl/str_g/_ustrpbr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrpbr.c

_ustrrch.o: $(MWLIB)/cl/str_g/_ustrrch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrrch.c

_ustrset.o: $(MWLIB)/cl/str_g/_ustrset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrset.c

_ustrspn.o: $(MWLIB)/cl/str_g/_ustrspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrspn.c

_ustrstr.o: $(MWLIB)/cl/str_g/_ustrstr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrstr.c

_ustrtkr.o: $(MWLIB)/cl/str_g/_ustrtkr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_ustrtkr.c

_vdmemmove.o: $(MWLIB)/cl/str_g/_vdmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_vdmemmove.c

_vmemset.o: $(MWLIB)/cl/str_g/_vmemset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_vmemset.c

_vsdmemmove.o: $(MWLIB)/cl/str_g/_vsdmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_vsdmemmove.c

_vsmemmove.o: $(MWLIB)/cl/str_g/_vsmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/_vsmemmove.c

memcpy.o: $(MWLIB)/cl/str_g/memcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/memcpy.c

memset.o: $(MWLIB)/cl/str_g/memset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/memset.c

memchr.o: $(MWLIB)/cl/str_g/memchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/memchr.c

memcmp.o: $(MWLIB)/cl/str_g/memcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/memcmp.c

memmove.o: $(MWLIB)/cl/str_g/memmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/memmove.c

strcasecmp.o: $(MWLIB)/cl/str_g/strcasecmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strcasecmp.c

strcat.o: $(MWLIB)/cl/str_g/strcat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strcat.c

strchr.o: $(MWLIB)/cl/str_g/strchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strchr.c

strcmp.o: $(MWLIB)/cl/str_g/strcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strcmp.c

strcoll.o: $(MWLIB)/cl/str_g/strcoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strcoll.c

strcpy.o: $(MWLIB)/cl/str_g/strcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strcpy.c

strcspn.o: $(MWLIB)/cl/str_g/strcspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strcspn.c

strerror.o: $(MWLIB)/cl/str_g/strerror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strerror.c

strlen.o: $(MWLIB)/cl/str_g/strlen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strlen.c

strerror_r.o: $(MWLIB)/cl/str_g/strerror_r.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strerror_r.c

strerror_s.o: $(MWLIB)/cl/str_g/strerror_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strerror_s.c

strncasecmp.o: $(MWLIB)/cl/str_g/strncasecmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strncasecmp.c

strncat.o: $(MWLIB)/cl/str_g/strncat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strncat.c

strncmp.o: $(MWLIB)/cl/str_g/strncmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strncmp.c

strncpy.o: $(MWLIB)/cl/str_g/strncpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strncpy.c

strpbrk.o: $(MWLIB)/cl/str_g/strpbrk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strpbrk.c

strrchr.o: $(MWLIB)/cl/str_g/strrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strrchr.c

strspn.o: $(MWLIB)/cl/str_g/strspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strspn.c

strstr.o: $(MWLIB)/cl/str_g/strstr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strstr.c

strtok.o: $(MWLIB)/cl/str_g/strtok.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strtok.c

strxfrm.o: $(MWLIB)/cl/str_g/strxfrm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/str_g/strxfrm.c

_asctime.o: $(MWLIB)/cl/time_g/_asctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_asctime.c

_ctime.o: $(MWLIB)/cl/time_g/_ctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_ctime.c

_get_tz.o: $(MWLIB)/cl/time_g/_get_tz.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_get_tz.c

_gmtime.o: $(MWLIB)/cl/time_g/_gmtime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_gmtime.c

_localtm.o: $(MWLIB)/cl/time_g/_localtm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_localtm.c

_mktime.o: $(MWLIB)/cl/time_g/_mktime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_mktime.c

_strdate.o: $(MWLIB)/cl/time_g/_strdate.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_strdate.c

_strtime.o: $(MWLIB)/cl/time_g/_strtime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_strtime.c

_tzset.o: $(MWLIB)/cl/time_g/_tzset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/_tzset.c

asctime.o: $(MWLIB)/cl/time_g/asctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/asctime.c

ctime.o: $(MWLIB)/cl/time_g/ctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/ctime.c

difftime.o: $(MWLIB)/cl/time_g/difftime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/difftime.c

gmtime.o: $(MWLIB)/cl/time_g/gmtime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/gmtime.c

localtm.o: $(MWLIB)/cl/time_g/localtm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/localtm.c

mktime.o: $(MWLIB)/cl/time_g/mktime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/mktime.c

strftime.o: $(MWLIB)/cl/time_g/strftime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/time_g/strftime.c

fenv.o: $(MWLIB)/cl/misc_g/fenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/misc_g/fenv.c

cmplx.o: $(MWLIB)/cl/complex_g/cmplx.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cmplx.c

cmplxf.o: $(MWLIB)/cl/complex_g/cmplxf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cmplxf.c

cabs.o: $(MWLIB)/cl/complex_g/cabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/cabs.c

cabsf.o: $(MWLIB)/cl/complex_g/cabsf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/cabsf.c

carg.o: $(MWLIB)/cl/complex_g/carg.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/carg.c

cargf.o: $(MWLIB)/cl/complex_g/cargf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/cargf.c

cacos.o: $(MWLIB)/cl/complex_g/cacos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cacos.c

cacosf.o: $(MWLIB)/cl/complex_g/cacosf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cacosf.c

cacosh.o: $(MWLIB)/cl/complex_g/cacosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cacosh.c

cacoshf.o: $(MWLIB)/cl/complex_g/cacoshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cacoshf.c

casin.o: $(MWLIB)/cl/complex_g/casin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/casin.c

casinf.o: $(MWLIB)/cl/complex_g/casinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/casinf.c

casinh.o: $(MWLIB)/cl/complex_g/casinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/casinh.c

casinhf.o: $(MWLIB)/cl/complex_g/casinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/casinhf.c

catan.o: $(MWLIB)/cl/complex_g/catan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/catan.c

catanf.o: $(MWLIB)/cl/complex_g/catanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/catanf.c

catanh.o: $(MWLIB)/cl/complex_g/catanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/catanh.c

catanhf.o: $(MWLIB)/cl/complex_g/catanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/catanhf.c

ccos.o: $(MWLIB)/cl/complex_g/ccos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ccos.c

ccosf.o: $(MWLIB)/cl/complex_g/ccosf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ccosf.c

ccosh.o: $(MWLIB)/cl/complex_g/ccosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ccosh.c

ccoshf.o: $(MWLIB)/cl/complex_g/ccoshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ccoshf.c

cexp.o: $(MWLIB)/cl/complex_g/cexp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cexp.c

cexpf.o: $(MWLIB)/cl/complex_g/cexpf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cexpf.c

clog.o: $(MWLIB)/cl/complex_g/clog.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/clog.c

clogf.o: $(MWLIB)/cl/complex_g/clogf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/clogf.c

cimag.o: $(MWLIB)/cl/complex_g/cimag.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cimag.c

cimagf.o: $(MWLIB)/cl/complex_g/cimagf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/cimagf.c

cpow.o: $(MWLIB)/cl/complex_g/cpow.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cpow.c

cpowf.o: $(MWLIB)/cl/complex_g/cpowf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cpowf.c

cproj.o: $(MWLIB)/cl/complex_g/cproj.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cproj.c

cprojf.o: $(MWLIB)/cl/complex_g/cprojf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/cprojf.c

conj.o: $(MWLIB)/cl/complex_g/conj.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/conj.c

conjf.o: $(MWLIB)/cl/complex_g/conjf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/conjf.c

creal.o: $(MWLIB)/cl/complex_g/creal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/creal.c

crealf.o: $(MWLIB)/cl/complex_g/crealf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/cl/complex_g/crealf.c

csin.o: $(MWLIB)/cl/complex_g/csin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/csin.c

csinf.o: $(MWLIB)/cl/complex_g/csinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/csinf.c

csinh.o: $(MWLIB)/cl/complex_g/csinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/csinh.c

csinhf.o: $(MWLIB)/cl/complex_g/csinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/csinhf.c

csqrt.o: $(MWLIB)/cl/complex_g/csqrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/csqrt.c

csqrtf.o: $(MWLIB)/cl/complex_g/csqrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/csqrtf.c

ctan.o: $(MWLIB)/cl/complex_g/ctan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ctan.c

ctanf.o: $(MWLIB)/cl/complex_g/ctanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ctanf.c

ctanh.o: $(MWLIB)/cl/complex_g/ctanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ctanh.c

ctanhf.o: $(MWLIB)/cl/complex_g/ctanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/complex_g/ctanhf.c

system.o: $(MWLIB)/cl/unix_g/system.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/unix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/unix_g/system.c

swprintf.o: $(MWLIB)/cl/wfio_g/swprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wfio_g/swprintf.c

swscanf.o: $(MWLIB)/cl/wfio_g/swscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wfio_g/swscanf.c

wscanf.o: $(MWLIB)/cl/wfio_g/wscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wfio_g/wscanf.c

btowc.o: $(MWLIB)/cl/wstr_g/btowc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/btowc.c

mbrlen.o: $(MWLIB)/cl/wstr_g/mbrlen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/mbrlen.c

mbrtowc.o: $(MWLIB)/cl/wstr_g/mbrtowc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/mbrtowc.c

mbsinit.o: $(MWLIB)/cl/wstr_g/mbsinit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/mbsinit.c

mbsrtowcs.o: $(MWLIB)/cl/wstr_g/mbsrtowcs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/mbsrtowcs.c

mbsnrtowcs.o: $(MWLIB)/cl/wstr_g/mbsnrtowcs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/mbsnrtowcs.c

utf8.o: $(MWLIB)/cl/wstr_g/utf8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/utf8.c

wcrtomb.o: $(MWLIB)/cl/wstr_g/wcrtomb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcrtomb.c

wcscat.o: $(MWLIB)/cl/wstr_g/wcscat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcscat.c

wcschr.o: $(MWLIB)/cl/wstr_g/wcschr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcschr.c

wcscmp.o: $(MWLIB)/cl/wstr_g/wcscmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcscmp.c

wcscoll.o: $(MWLIB)/cl/wstr_g/wcscoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcscoll.c

wcscpy.o: $(MWLIB)/cl/wstr_g/wcscpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcscpy.c

wcscspn.o: $(MWLIB)/cl/wstr_g/wcscspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcscspn.c

wcslen.o: $(MWLIB)/cl/wstr_g/wcslen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcslen.c

wcsncat.o: $(MWLIB)/cl/wstr_g/wcsncat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsncat.c

wcsncmp.o: $(MWLIB)/cl/wstr_g/wcsncmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsncmp.c

wcsncpy.o: $(MWLIB)/cl/wstr_g/wcsncpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsncpy.c

wcspbrk.o: $(MWLIB)/cl/wstr_g/wcspbrk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcspbrk.c

wcsrchr.o: $(MWLIB)/cl/wstr_g/wcsrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsrchr.c

wcsnrtombs.o: $(MWLIB)/cl/wstr_g/wcsnrtombs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsnrtombs.c

wcsrtombs.o: $(MWLIB)/cl/wstr_g/wcsrtombs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsrtombs.c

wcsspn.o: $(MWLIB)/cl/wstr_g/wcsspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsspn.c

wcsstr.o: $(MWLIB)/cl/wstr_g/wcsstr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsstr.c

wcstod.o: $(MWLIB)/cl/wstr_g/wcstod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstod.c

wcstof.o: $(MWLIB)/cl/wstr_g/wcstof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstof.c

wcstok.o: $(MWLIB)/cl/wstr_g/wcstok.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstok.c

wcstol.o: $(MWLIB)/cl/wstr_g/wcstol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstol.c

wcstoll.o: $(MWLIB)/cl/wstr_g/wcstoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstoll.c

wcstoul.o: $(MWLIB)/cl/wstr_g/wcstoul.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstoul.c

wcstoull.o: $(MWLIB)/cl/wstr_g/wcstoull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcstoull.c

wcsxfrm.o: $(MWLIB)/cl/wstr_g/wcsxfrm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wcsxfrm.c

wctob.o: $(MWLIB)/cl/wstr_g/wctob.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wctob.c

wctrans.o: $(MWLIB)/cl/wstr_g/wctrans.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wctrans.c

wctype.o: $(MWLIB)/cl/wstr_g/wctype.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wctype.c

wctype_l.o: $(MWLIB)/cl/wstr_g/wctype_l.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wctype_l.c

wmemchr.o: $(MWLIB)/cl/wstr_g/wmemchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wmemchr.c

wmemcmp.o: $(MWLIB)/cl/wstr_g/wmemcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wmemcmp.c

wmemcpy.o: $(MWLIB)/cl/wstr_g/wmemcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wmemcpy.c

wmemmove.o: $(MWLIB)/cl/wstr_g/wmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wmemmove.c

wmemset.o: $(MWLIB)/cl/wstr_g/wmemset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/wstr_g/wmemset.c

cxa_atexit.o: $(MWLIB)/cl/llvm_g/cxa_atexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/cl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/cl/llvm_g/cxa_atexit.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
