# libc arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
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

AR_NAME=libct.a
AR_OPT=""

LIST_fio=_bprintf.o _dofopen.o _fclosal.o _fclosfp.o _fd_shrd.o _fgetchr.o\
	_filbuf.o _filelen.o _fileno.o _fill.o _finit.o _flushal.o _fputchr.o\
	_iob.o _mktemp.o _stdiolk.o _tell.o _tempnam.o _ubprntf.o _udofopn.o\
	_ufdopen.o _ufgetc.o _ufgets.o _ufgtchr.o _ufopen.o _ufprintf.o\
	_ufputc.o _ufputch.o _ufputs.o _ufreopn.o _ugetc.o _ugetchr.o _ugets.o\
	_umktemp.o _uopen.o _uperror.o _uprintf.o _uputc.o _uputchr.o _uputs.o\
	_uremove.o _uscanf.o _usprntf.o _usscanf.o _utmpnam.o _uungetc.o\
	_uvfprintf.o _uvprintf.o _write_data.o asprintf.o clearerr.o fclose.o\
	fdopen.o feof.o ferror.o fflush.o fgetc.o fgetpos.o fgets.o filelock.o\
	fopen.o fprintf.o fputc.o fputs.o fread.o freopen.o fseek.o fseek64.o\
	fsetpos.o ftell.o ftell64.o fwrite.o getc.o getchar.o gets.o handlock.o\
	locks.o perror.o printf.o printf_s.o putc.o putchar.o puts.o rewind.o\
	scanf.o setbuf.o setvbuf.o snprintf.o sprintf.o sprintf_s.o sscanf.o\
	tmpfile.o ungetc.o vfprintf.o vprintf.o

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

LIST_misc=_bprt.o crc.o cyg.o _crotl.o _crotr.o _lfind.o _lrotl.o _lrotr.o\
	_lsearch.o _makepa.o _matherr.o _rotl.o _rotr.o _rterr.o _searche.o\
	_searchs.o _set_mat.o _splitpa.o _srotl.o _srotr.o _swab.o _sys_err.o\
	_arc_lr.o _disable.o _enable.o _mcount.o _mask_vecti.o _setvect.o\
	_sec_setvect.o dcache.o dcache_fblk.o dcache_fl.o dcache_iblk.o\
	dcache_inv.o dcache_lblk.o icache.o icache_iblk.o icache_inv.o\
	icache_lblk.o invcache.o invslcache.o slcache.o slcache_fblk.o\
	slcache_fl.o slcache_iblk.o slcache_inv.o slcache_lblk.o slcache_pblk.o\
	bputil.o gprof_ct.o iprof_ct.o iprofclk.o iprofile.o lzcompress.o\
	lzdecompress.o matherrx.o mwlock.o pictable.o prof_ct.o profrt.o\
	trace.o fenv.o

LIST_extra_objs=_rterr_stub.o angeltim.o angeltim1.o angeltim_sec0.o\
	angeltim_sec1.o breakp.o crt1.o crt1pt.o crt1_rib.o crt1pt_rib.o crti.o\
	crtn.o vxcrt1.o xcheckno.o pfnoflt.o sfnoflt.o s_memcpy.o s_memset.o

LIST_startup=arc_main.o arc_main_lin.o crt_initbss.o crt_initcopy.o\
	crt_inittimer.o crt_invcache.o initpic.o vectors.o xcheck.o

LIST_std=_assert.o _atold.o _atouni.o _cleanup.o _compare.o _doatof.o\
	_doatofd.o _doecvt.o _doecvtd.o _doprint.o _doscan.o _fillchr.o _find.o\
	_itoa.o _itos.o _ltoa.o _move.o _pow10.o _pow10d.o _round.o _roundd.o\
	_smalloc.o _stoa.o _stoi.o _strrev.o _u_ltoa.o _uassert.o _uatof.o\
	_uatoi.o _uatol.o _uctypef.o _ucvt.o _ucvtld.o _udoatfd.o _udoatof.o\
	_udoecvd.o _udoecvt.o _udoprnt.o _udoscan.o _ugcvt.o _ugetenv.o\
	_uitoa.o _uitos.o _ultoa.o _ustoi.o _ustrrev.o _ustrtod.o _ustrtof.o\
	_ustrtol.o _ustrtul.o _uultoa.o abort.o abort_handler.o setjmp.o\
	arith.o atexit.o atof.o atoi.o atol.o bsearch.o calloc.o cfree.o\
	constraint.o ctype.o ctypef.o ctype_l.o cvt.o cvt2.o cvtld.o exit.o\
	findchr.o imaxabs.o imaxdiv.o initcopy.o initcopyp.o isascii.o locale.o\
	localeco.o malloc2.o malloc2x.o mblen.o mbstowcs.o mbtowc.o mkstemp.o\
	nofloatp.o nofloats.o onexit.o qsort.o rand.o rsizet.o setlocal.o\
	signal.o skipchr.o strtod.o strtof.o strtol.o strtoul.o toascii.o\
	wcstombs.o wctomb.o _lltoa.o _lltos.o _stoll.o _u_lltoa.o _u_ulltoa.o\
	_uatoll.o _ulltoa.o _ulltos.o _ustoll.o _ustrtoll.o _ustrtoull.o\
	atoll.o strtoll.o strtoull.o ulltod.o

LIST_str=_memccpy.o _memicmp.o _rmemcpy.o _rstrcpy.o _rstrncp.o _serr_nl.o\
	_strcats.o _strcmpi.o _strdup.o _stricmp.o _strlwr.o _strncat.o\
	_strnicm.o _strnset.o _strset.o _strtokr.o _strupr.o _uchrccp.o\
	_uchrchr.o _uchrcmp.o _uchrcpy.o _uchricm.o _uchrset.o _ustrcat.o\
	_ustrchr.o _ustrcmi.o _ustrcmp.o _ustrcpy.o _ustrcsp.o _ustrdup.o\
	_ustrerr.o _ustricm.o _ustrlen.o _ustrlwr.o _ustrnca.o _ustrncm.o\
	_ustrncp.o _ustrnic.o _ustrnst.o _ustrpbr.o _ustrrch.o _ustrset.o\
	_ustrspn.o _ustrstr.o _ustrtkr.o _ustrtok.o _ustrupr.o _vdmemmove.o\
	_vmemset.o _vsdmemmove.o _vsmemmove.o _vdmemcpy.o _vsdmemcpy.o\
	_vsmemcpy.o memcpy.o memcpy_s.o _a5_strcmp.o _a5_strlen.o f_memset.o\
	memchr.o memcmp.o memmove.o memmove_s.o memset_s.o strcasecmp.o\
	strcat.o strcat_s.o strchr.o strcoll.o strcpy.o strcpy_s.o strcspn.o\
	strerror.o strerror_r.o strerror_s.o strnlen_s.o strncasecmp.o\
	strncat.o strncat_s.o strncmp.o strncpy.o strncpy_s.o strpbrk.o\
	strrchr.o strspn.o strstr.o strtok.o strxfrm.o

LIST_time=clkgettm.o _asctime.o _ctime.o _get_tz.o _gmtime.o _localtm.o\
	_mktime.o _strdate.o _strtime.o _tzdata.o _tzset.o _u_ascti.o\
	_u_ctime.o _uasctim.o _uctime.o _ustrfti.o _utzdata.o _utzset.o\
	asctime.o ctime.o difftime.o gmtime.o localtm.o mktime.o strftime.o

LIST_unix=system.o

LIST_complex=cmplx.o cmplxf.o cabs.o cabsf.o carg.o cargf.o cacos.o cacosf.o\
	cacosh.o cacoshf.o casin.o casinf.o casinh.o casinhf.o catan.o catanf.o\
	catanh.o catanhf.o ccos.o ccosf.o ccosh.o ccoshf.o cexp.o cexpf.o\
	clog.o clogf.o cimag.o cimagf.o cpow.o cpowf.o cproj.o cprojf.o conj.o\
	conjf.o creal.o crealf.o csin.o csinf.o csinh.o csinhf.o csqrt.o\
	csqrtf.o ctan.o ctanf.o ctanh.o ctanhf.o

LIST_wfio=fgetwc.o fgetws.o fputwc.o fputws.o fwide.o getwchar.o putwchar.o\
	swprintf.o swscanf.o ungetwc.o wfio.o wfprintf.o wfread.o wfwrite.o\
	wscanf.o

LIST_wstr=btowc.o mbrlen.o mbrtowc.o mbsinit.o mbsrtowcs.o mbsnrtowcs.o utf8.o\
	wcrtomb.o wcscat.o wcschr.o wcscmp.o wcscoll.o wcscpy.o wcscspn.o\
	wcslen.o wcsncat.o wcsncmp.o wcsncpy.o wcspbrk.o wcsrchr.o wcsnrtombs.o\
	wcsrtombs.o wcsspn.o wcsstr.o wcstod.o wcstof.o wcstok.o wcstol.o\
	wcstoll.o wcstoul.o wcstoull.o wcsxfrm.o wctob.o wctype.o wctrans.o\
	wctype_l.o wmemchr.o wmemcmp.o wmemcpy.o wmemmove.o wmemset.o

LIST_llvm=cxa_atexit.o cxa_thread_atexit.o

LIST_ALL=$(LIST_fio) $(LIST_fp) $(LIST_ieee) $(LIST_misc) $(LIST_startup) $(LIST_std) $(LIST_str) $(LIST_time) $(LIST_unix) $(LIST_complex) $(LIST_wfio) $(LIST_wstr) $(LIST_llvm)

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
	$(AR) rv $(AR_NAME) $(LIST_unix)
	$(AR) rv $(AR_NAME) $(LIST_complex)
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
unix: $(LIST_unix)
complex: $(LIST_complex)
wfio: $(LIST_wfio)
wstr: $(LIST_wstr)
llvm: $(LIST_llvm)

_bprintf.o: $(MWLIB)/c/fio_g/_bprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_bprintf.c

_dofopen.o: $(MWLIB)/c/fio_g/_dofopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_dofopen.c

_fclosal.o: $(MWLIB)/c/fio_g/_fclosal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fclosal.c

_fclosfp.o: $(MWLIB)/c/fio_g/_fclosfp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fclosfp.c

_fd_shrd.o: $(MWLIB)/c/fio_g/_fd_shrd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fd_shrd.c

_fgetchr.o: $(MWLIB)/c/fio_g/_fgetchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fgetchr.c

_filbuf.o: $(MWLIB)/c/fio_g/_filbuf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_filbuf.c

_filelen.o: $(MWLIB)/c/fio_g/_filelen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_filelen.c

_fileno.o: $(MWLIB)/c/fio_g/_fileno.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fileno.c

_fill.o: $(MWLIB)/c/fio_g/_fill.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fill.c

_finit.o: $(MWLIB)/c/fio_g/_finit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_finit.c

_flushal.o: $(MWLIB)/c/fio_g/_flushal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_flushal.c

_fputchr.o: $(MWLIB)/c/fio_g/_fputchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_fputchr.c

_iob.o: $(MWLIB)/c/fio_g/_iob.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_iob.c

_mktemp.o: $(MWLIB)/c/fio_g/_mktemp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_mktemp.c

_stdiolk.o: $(MWLIB)/c/fio_g/_stdiolk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_stdiolk.c

_tell.o: $(MWLIB)/c/fio_g/_tell.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_tell.c

_tempnam.o: $(MWLIB)/c/fio_g/_tempnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_tempnam.c

_ubprntf.o: $(MWLIB)/c/fio_g/_ubprntf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ubprntf.c

_udofopn.o: $(MWLIB)/c/fio_g/_udofopn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_udofopn.c

_ufdopen.o: $(MWLIB)/c/fio_g/_ufdopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufdopen.c

_ufgetc.o: $(MWLIB)/c/fio_g/_ufgetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufgetc.c

_ufgets.o: $(MWLIB)/c/fio_g/_ufgets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufgets.c

_ufgtchr.o: $(MWLIB)/c/fio_g/_ufgtchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufgtchr.c

_ufopen.o: $(MWLIB)/c/fio_g/_ufopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufopen.c

_ufprintf.o: $(MWLIB)/c/fio_g/_ufprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufprintf.c

_ufputc.o: $(MWLIB)/c/fio_g/_ufputc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufputc.c

_ufputch.o: $(MWLIB)/c/fio_g/_ufputch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufputch.c

_ufputs.o: $(MWLIB)/c/fio_g/_ufputs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufputs.c

_ufreopn.o: $(MWLIB)/c/fio_g/_ufreopn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ufreopn.c

_ugetc.o: $(MWLIB)/c/fio_g/_ugetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ugetc.c

_ugetchr.o: $(MWLIB)/c/fio_g/_ugetchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ugetchr.c

_ugets.o: $(MWLIB)/c/fio_g/_ugets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_ugets.c

_umktemp.o: $(MWLIB)/c/fio_g/_umktemp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_umktemp.c

_uopen.o: $(MWLIB)/c/fio_g/_uopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uopen.c

_uperror.o: $(MWLIB)/c/fio_g/_uperror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uperror.c

_uprintf.o: $(MWLIB)/c/fio_g/_uprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uprintf.c

_uputc.o: $(MWLIB)/c/fio_g/_uputc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uputc.c

_uputchr.o: $(MWLIB)/c/fio_g/_uputchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uputchr.c

_uputs.o: $(MWLIB)/c/fio_g/_uputs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uputs.c

_uremove.o: $(MWLIB)/c/fio_g/_uremove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uremove.c

_uscanf.o: $(MWLIB)/c/fio_g/_uscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uscanf.c

_usprntf.o: $(MWLIB)/c/fio_g/_usprntf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_usprntf.c

_usscanf.o: $(MWLIB)/c/fio_g/_usscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_usscanf.c

_utmpnam.o: $(MWLIB)/c/fio_g/_utmpnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_utmpnam.c

_uungetc.o: $(MWLIB)/c/fio_g/_uungetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uungetc.c

_uvfprintf.o: $(MWLIB)/c/fio_g/_uvfprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uvfprintf.c

_uvprintf.o: $(MWLIB)/c/fio_g/_uvprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/_uvprintf.c

_write_data.o: $(MWLIB)/c/fio_g/_write_data.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) -Os1 $(MWLIB)/c/fio_g/_write_data.c

asprintf.o: $(MWLIB)/c/fio_g/asprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/asprintf.c

clearerr.o: $(MWLIB)/c/fio_g/clearerr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/clearerr.c

fclose.o: $(MWLIB)/c/fio_g/fclose.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fclose.c

fdopen.o: $(MWLIB)/c/fio_g/fdopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fdopen.c

feof.o: $(MWLIB)/c/fio_g/feof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/feof.c

ferror.o: $(MWLIB)/c/fio_g/ferror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/ferror.c

fflush.o: $(MWLIB)/c/fio_g/fflush.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fflush.c

fgetc.o: $(MWLIB)/c/fio_g/fgetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fgetc.c

fgetpos.o: $(MWLIB)/c/fio_g/fgetpos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fgetpos.c

fgets.o: $(MWLIB)/c/fio_g/fgets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fgets.c

filelock.o: $(MWLIB)/c/fio_g/filelock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/filelock.c

fopen.o: $(MWLIB)/c/fio_g/fopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fopen.c

fprintf.o: $(MWLIB)/c/fio_g/fprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fprintf.c

fputc.o: $(MWLIB)/c/fio_g/fputc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fputc.c

fputs.o: $(MWLIB)/c/fio_g/fputs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fputs.c

fread.o: $(MWLIB)/c/fio_g/fread.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fread.c

freopen.o: $(MWLIB)/c/fio_g/freopen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/freopen.c

fseek.o: $(MWLIB)/c/fio_g/fseek.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fseek.c

fseek64.o: $(MWLIB)/c/fio_g/fseek64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fseek64.c

fsetpos.o: $(MWLIB)/c/fio_g/fsetpos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fsetpos.c

ftell.o: $(MWLIB)/c/fio_g/ftell.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/ftell.c

ftell64.o: $(MWLIB)/c/fio_g/ftell64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/ftell64.c

fwrite.o: $(MWLIB)/c/fio_g/fwrite.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/fwrite.c

getc.o: $(MWLIB)/c/fio_g/getc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/getc.c

getchar.o: $(MWLIB)/c/fio_g/getchar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/getchar.c

gets.o: $(MWLIB)/c/fio_g/gets.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/gets.c

handlock.o: $(MWLIB)/c/fio_g/handlock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/handlock.c

locks.o: $(MWLIB)/c/fio_g/locks.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/locks.c

perror.o: $(MWLIB)/c/fio_g/perror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/perror.c

printf.o: $(MWLIB)/c/fio_g/printf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/printf.c

printf_s.o: $(MWLIB)/c/fio_g/printf_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/printf_s.c

putc.o: $(MWLIB)/c/fio_g/putc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/putc.c

putchar.o: $(MWLIB)/c/fio_g/putchar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/putchar.c

puts.o: $(MWLIB)/c/fio_g/puts.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/puts.c

rewind.o: $(MWLIB)/c/fio_g/rewind.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/rewind.c

scanf.o: $(MWLIB)/c/fio_g/scanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/scanf.c

setbuf.o: $(MWLIB)/c/fio_g/setbuf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/setbuf.c

setvbuf.o: $(MWLIB)/c/fio_g/setvbuf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/setvbuf.c

snprintf.o: $(MWLIB)/c/fio_g/snprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/snprintf.c

sprintf.o: $(MWLIB)/c/fio_g/sprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/sprintf.c

sprintf_s.o: $(MWLIB)/c/fio_g/sprintf_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/sprintf_s.c

sscanf.o: $(MWLIB)/c/fio_g/sscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/sscanf.c

tmpfile.o: $(MWLIB)/c/fio_g/tmpfile.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/tmpfile.c

ungetc.o: $(MWLIB)/c/fio_g/ungetc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/ungetc.c

vfprintf.o: $(MWLIB)/c/fio_g/vfprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/vfprintf.c

vprintf.o: $(MWLIB)/c/fio_g/vprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fio_g/vprintf.c

acoshf.o: $(MWLIB)/c/fp_g/acoshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/acoshf.c

asinhf.o: $(MWLIB)/c/fp_g/asinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/asinhf.c

atanhf.o: $(MWLIB)/c/fp_g/atanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/atanhf.c

_acosh.o: $(MWLIB)/c/fp_g/_acosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_acosh.c

_asinh.o: $(MWLIB)/c/fp_g/_asinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_asinh.c

_atan.o: $(MWLIB)/c/fp_g/_atan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_atan.c

_atanf.o: $(MWLIB)/c/fp_g/_atanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_atanf.c

_atanh.o: $(MWLIB)/c/fp_g/_atanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_atanh.c

_fpclassifyd.o: $(MWLIB)/c/fp_g/_fpclassifyd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_fpclassifyd.c

_fpclassifyf.o: $(MWLIB)/c/fp_g/_fpclassifyf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_fpclassifyf.c

_hypot.o: $(MWLIB)/c/fp_g/_hypot.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_hypot.c

_isfinited.o: $(MWLIB)/c/fp_g/_isfinited.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isfinited.c

_isfinitef.o: $(MWLIB)/c/fp_g/_isfinitef.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isfinitef.c

_isinfd.o: $(MWLIB)/c/fp_g/_isinfd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isinfd.c

_isinff.o: $(MWLIB)/c/fp_g/_isinff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isinff.c

_isnand.o: $(MWLIB)/c/fp_g/_isnand.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isnand.c

_isnanf.o: $(MWLIB)/c/fp_g/_isnanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isnanf.c

_isnormd.o: $(MWLIB)/c/fp_g/_isnormd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isnormd.c

_isnormf.o: $(MWLIB)/c/fp_g/_isnormf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_isnormf.c

_j0.o: $(MWLIB)/c/fp_g/_j0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_j0.c

_j1.o: $(MWLIB)/c/fp_g/_j1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_j1.c

_jn.o: $(MWLIB)/c/fp_g/_jn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_jn.c

_log2.o: $(MWLIB)/c/fp_g/_log2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_log2.c

_pandq0.o: $(MWLIB)/c/fp_g/_pandq0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_pandq0.c

_pandq1.o: $(MWLIB)/c/fp_g/_pandq1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_pandq1.c

_pneval.o: $(MWLIB)/c/fp_g/_pneval.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_pneval.c

_signbitd.o: $(MWLIB)/c/fp_g/_signbitd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_signbitd.c

_signbitf.o: $(MWLIB)/c/fp_g/_signbitf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_signbitf.c

_y0.o: $(MWLIB)/c/fp_g/_y0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_y0.c

_y1.o: $(MWLIB)/c/fp_g/_y1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_y1.c

_yn.o: $(MWLIB)/c/fp_g/_yn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/_yn.c

asin.o: $(MWLIB)/c/fp_g/asin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/asin.c

asinf.o: $(MWLIB)/c/fp_g/asinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/asinf.c

atan.o: $(MWLIB)/c/fp_g/atan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/atan.c

atanf.o: $(MWLIB)/c/fp_g/atanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/atanf.c

atan2.o: $(MWLIB)/c/fp_g/atan2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/atan2.c

atan2f.o: $(MWLIB)/c/fp_g/atan2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/atan2f.c

cbrt.o: $(MWLIB)/c/fp_g/cbrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/cbrt.c

cbrtf.o: $(MWLIB)/c/fp_g/cbrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/cbrtf.c

ceil.o: $(MWLIB)/c/fp_g/ceil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/ceil.c

ceilf.o: $(MWLIB)/c/fp_g/ceilf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/ceilf.c

copysign.o: $(MWLIB)/c/fp_g/copysign.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/copysign.c

copysignf.o: $(MWLIB)/c/fp_g/copysignf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/copysignf.c

copysignl.o: $(MWLIB)/c/fp_g/copysignl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/copysignl.c

coshf.o: $(MWLIB)/c/fp_g/coshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/coshf.c

erf.o: $(MWLIB)/c/fp_g/erf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/erf.c

exp.o: $(MWLIB)/c/fp_g/exp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/exp.c

exp2.o: $(MWLIB)/c/fp_g/exp2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/exp2.c

exp10.o: $(MWLIB)/c/fp_g/exp10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/exp10.c

expf.o: $(MWLIB)/c/fp_g/expf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/expf.c

exp2f.o: $(MWLIB)/c/fp_g/exp2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/exp2f.c

exp10f.o: $(MWLIB)/c/fp_g/exp10f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/exp10f.c

expm1.o: $(MWLIB)/c/fp_g/expm1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/expm1.c

expm1f.o: $(MWLIB)/c/fp_g/expm1f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/expm1f.c

fabs.o: $(MWLIB)/c/fp_g/fabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fabs.c

fabsf.o: $(MWLIB)/c/fp_g/fabsf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fabsf.c

fdim.o: $(MWLIB)/c/fp_g/fdim.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fdim.c

fdimf.o: $(MWLIB)/c/fp_g/fdimf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fdimf.c

floor.o: $(MWLIB)/c/fp_g/floor.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/floor.c

floorf.o: $(MWLIB)/c/fp_g/floorf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/floorf.c

fma.o: $(MWLIB)/c/fp_g/fma.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fma.c

fmaf.o: $(MWLIB)/c/fp_g/fmaf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fmaf.c

fmax.o: $(MWLIB)/c/fp_g/fmax.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fmax.c

fmaxf.o: $(MWLIB)/c/fp_g/fmaxf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fmaxf.c

fmin.o: $(MWLIB)/c/fp_g/fmin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fmin.c

fminf.o: $(MWLIB)/c/fp_g/fminf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fminf.c

fmod.o: $(MWLIB)/c/fp_g/fmod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fmod.c

fmodf.o: $(MWLIB)/c/fp_g/fmodf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fmodf.c

fp.o: $(MWLIB)/c/fp_g/fp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/fp.c

hypot.o: $(MWLIB)/c/fp_g/hypot.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/hypot.c

hypotf.o: $(MWLIB)/c/fp_g/hypotf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/hypotf.c

ilogb.o: $(MWLIB)/c/fp_g/ilogb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/ilogb.c

ilogbf.o: $(MWLIB)/c/fp_g/ilogbf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/ilogbf.c

isinf.o: $(MWLIB)/c/fp_g/isinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/isinf.c

isinff.o: $(MWLIB)/c/fp_g/isinff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/isinff.c

isinfl.o: $(MWLIB)/c/fp_g/isinfl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/isinfl.c

isnan.o: $(MWLIB)/c/fp_g/isnan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/isnan.c

isnanf.o: $(MWLIB)/c/fp_g/isnanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/isnanf.c

isnanl.o: $(MWLIB)/c/fp_g/isnanl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/isnanl.c

lgamma.o: $(MWLIB)/c/fp_g/lgamma.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/lgamma.c

lgammaf.o: $(MWLIB)/c/fp_g/lgammaf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/lgammaf.c

log.o: $(MWLIB)/c/fp_g/log.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log.c

logf.o: $(MWLIB)/c/fp_g/logf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/logf.c

logb.o: $(MWLIB)/c/fp_g/logb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/logb.c

logbf.o: $(MWLIB)/c/fp_g/logbf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/logbf.c

log1p.o: $(MWLIB)/c/fp_g/log1p.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log1p.c

log1pf.o: $(MWLIB)/c/fp_g/log1pf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log1pf.c

log2.o: $(MWLIB)/c/fp_g/log2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log2.c

log2f.o: $(MWLIB)/c/fp_g/log2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log2f.c

log10.o: $(MWLIB)/c/fp_g/log10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log10.c

log10f.o: $(MWLIB)/c/fp_g/log10f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/log10f.c

llrint.o: $(MWLIB)/c/fp_g/llrint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/llrint.c

llrintf.o: $(MWLIB)/c/fp_g/llrintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/llrintf.c

lrint.o: $(MWLIB)/c/fp_g/lrint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/lrint.c

lrintf.o: $(MWLIB)/c/fp_g/lrintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/lrintf.c

lround.o: $(MWLIB)/c/fp_g/lround.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/lround.c

lroundf.o: $(MWLIB)/c/fp_g/lroundf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/lroundf.c

llround.o: $(MWLIB)/c/fp_g/llround.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/llround.c

llroundf.o: $(MWLIB)/c/fp_g/llroundf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/llroundf.c

modf.o: $(MWLIB)/c/fp_g/modf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/modf.c

modff.o: $(MWLIB)/c/fp_g/modff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/modff.c

nan.o: $(MWLIB)/c/fp_g/nan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/nan.c

nanf.o: $(MWLIB)/c/fp_g/nanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/nanf.c

nextafter.o: $(MWLIB)/c/fp_g/nextafter.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/nextafter.c

nextafterf.o: $(MWLIB)/c/fp_g/nextafterf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/nextafterf.c

nexttowardf.o: $(MWLIB)/c/fp_g/nexttowardf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/nexttowardf.c

pow.o: $(MWLIB)/c/fp_g/pow.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/pow.c

powf.o: $(MWLIB)/c/fp_g/powf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/powf.c

remainder.o: $(MWLIB)/c/fp_g/remainder.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/remainder.c

remainderf.o: $(MWLIB)/c/fp_g/remainderf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/remainderf.c

remquo.o: $(MWLIB)/c/fp_g/remquo.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/remquo.c

remquof.o: $(MWLIB)/c/fp_g/remquof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/remquof.c

rint.o: $(MWLIB)/c/fp_g/rint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/rint.c

rintf.o: $(MWLIB)/c/fp_g/rintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/rintf.c

round.o: $(MWLIB)/c/fp_g/round.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/round.c

roundf.o: $(MWLIB)/c/fp_g/roundf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/roundf.c

scalbn.o: $(MWLIB)/c/fp_g/scalbn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/scalbn.c

scalbnf.o: $(MWLIB)/c/fp_g/scalbnf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/scalbnf.c

sin.o: $(MWLIB)/c/fp_g/sin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/sin.c

sinf.o: $(MWLIB)/c/fp_g/sinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/sinf.c

sinh.o: $(MWLIB)/c/fp_g/sinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/sinh.c

sinhf.o: $(MWLIB)/c/fp_g/sinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/sinhf.c

sqrt.o: $(MWLIB)/c/fp_g/sqrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/sqrt.c

sqrtf.o: $(MWLIB)/c/fp_g/sqrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/sqrtf.c

tan.o: $(MWLIB)/c/fp_g/tan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/tan.c

tanf.o: $(MWLIB)/c/fp_g/tanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/tanf.c

tanh.o: $(MWLIB)/c/fp_g/tanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/tanh.c

tanhf.o: $(MWLIB)/c/fp_g/tanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/tanhf.c

trunc.o: $(MWLIB)/c/fp_g/trunc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/trunc.c

truncf.o: $(MWLIB)/c/fp_g/truncf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/fp_g/truncf.c

float.o: $(MWLIB)/c/ieee_g/float.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/float.c

fpcon.o: $(MWLIB)/c/ieee_g/fpcon.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/fpcon.c

fpcon2.o: $(MWLIB)/c/ieee_g/fpcon2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/fpcon2.c

fpcon2f.o: $(MWLIB)/c/ieee_g/fpcon2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/fpcon2f.c

frexp.o: $(MWLIB)/c/ieee_g/frexp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/frexp.c

frexpf.o: $(MWLIB)/c/ieee_g/frexpf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/frexpf.c

ldexp.o: $(MWLIB)/c/ieee_g/ldexp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/ldexp.c

ldexpf.o: $(MWLIB)/c/ieee_g/ldexpf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/ldexpf.c

ufpecvt.o: $(MWLIB)/c/ieee_g/ufpecvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/ieee_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/ieee_g/ufpecvt.c

_bprt.o: $(MWLIB)/c/misc_g/_bprt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_bprt.c

crc.o: $(MWLIB)/c/misc_g/crc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/crc.c

cyg.o: $(MWLIB)/c/misc_g/cyg.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/cyg.c

_crotl.o: $(MWLIB)/c/misc_g/_crotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_crotl.c

_crotr.o: $(MWLIB)/c/misc_g/_crotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_crotr.c

_lfind.o: $(MWLIB)/c/misc_g/_lfind.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_lfind.c

_lrotl.o: $(MWLIB)/c/misc_g/_lrotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_lrotl.c

_lrotr.o: $(MWLIB)/c/misc_g/_lrotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_lrotr.c

_lsearch.o: $(MWLIB)/c/misc_g/_lsearch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_lsearch.c

_makepa.o: $(MWLIB)/c/misc_g/_makepa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_makepa.c

_matherr.o: $(MWLIB)/c/misc_g/_matherr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_matherr.c

_rotl.o: $(MWLIB)/c/misc_g/_rotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_rotl.c

_rotr.o: $(MWLIB)/c/misc_g/_rotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_rotr.c

_rterr.o: $(MWLIB)/c/misc_g/_rterr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_rterr.c

_rterr_stub.o: $(MWLIB)/c/misc_g/_rterr_stub.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_rterr_stub.c

_searche.o: $(MWLIB)/c/misc_g/_searche.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_searche.c

_searchs.o: $(MWLIB)/c/misc_g/_searchs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_searchs.c

_set_mat.o: $(MWLIB)/c/misc_g/_set_mat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_set_mat.c

_splitpa.o: $(MWLIB)/c/misc_g/_splitpa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_splitpa.c

_srotl.o: $(MWLIB)/c/misc_g/_srotl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_srotl.c

_srotr.o: $(MWLIB)/c/misc_g/_srotr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_srotr.c

_swab.o: $(MWLIB)/c/misc_g/_swab.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_swab.c

_sys_err.o: $(MWLIB)/c/misc_g/_sys_err.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_sys_err.c

_arc_lr.o: $(MWLIB)/c/misc_g/_arc_lr.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/misc_g/_arc_lr.s

_disable.o: $(MWLIB)/c/misc_g/_disable.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/misc_g/_disable.s

_enable.o: $(MWLIB)/c/misc_g/_enable.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/misc_g/_enable.s

_mcount.o: $(MWLIB)/c/misc_g/_mcount.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/misc_g/_mcount.s

_mask_vecti.o: $(MWLIB)/c/misc_g/_mask_vecti.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_mask_vecti.c

_setvect.o: $(MWLIB)/c/misc_g/_setvect.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/_setvect.c

_sec_setvect.o: $(MWLIB)/c/misc_g/_sec_setvect.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) -Xsec_modes $(MWLIB)/c/misc_g/_sec_setvect.c

angeltim.o: $(MWLIB)/c/misc_g/angeltim.s
	$(AS) $(AS_OPT) $(aflags) -Xtimer0 $(AS_INC) -o $@ $(MWLIB)/c/misc_g/angeltim.s

angeltim1.o: $(MWLIB)/c/misc_g/angeltim1.s
	$(AS) $(AS_OPT) $(aflags) -Xtimer1 $(AS_INC) -o $@ $(MWLIB)/c/misc_g/angeltim1.s

angeltim_sec0.o: $(MWLIB)/c/misc_g/angeltim_sec0.s
	$(AS) $(AS_OPT) $(aflags) -Xsec_modes -Xsec_timer0 $(AS_INC) -o $@ $(MWLIB)/c/misc_g/angeltim_sec0.s

angeltim_sec1.o: $(MWLIB)/c/misc_g/angeltim_sec1.s
	$(AS) $(AS_OPT) $(aflags) -Xsec_modes -Xsec_timer1 $(AS_INC) -o $@ $(MWLIB)/c/misc_g/angeltim_sec1.s

dcache.o: $(MWLIB)/c/misc_g/dcache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/dcache.c

dcache_fblk.o: $(MWLIB)/c/misc_g/dcache_fblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/dcache_fblk.c

dcache_fl.o: $(MWLIB)/c/misc_g/dcache_fl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/dcache_fl.c

dcache_iblk.o: $(MWLIB)/c/misc_g/dcache_iblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/dcache_iblk.c

dcache_inv.o: $(MWLIB)/c/misc_g/dcache_inv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/dcache_inv.c

dcache_lblk.o: $(MWLIB)/c/misc_g/dcache_lblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/dcache_lblk.c

icache.o: $(MWLIB)/c/misc_g/icache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/icache.c

icache_iblk.o: $(MWLIB)/c/misc_g/icache_iblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/icache_iblk.c

icache_inv.o: $(MWLIB)/c/misc_g/icache_inv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/icache_inv.c

icache_lblk.o: $(MWLIB)/c/misc_g/icache_lblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/icache_lblk.c

invcache.o: $(MWLIB)/c/misc_g/invcache.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/misc_g/invcache.s

invslcache.o: $(MWLIB)/c/misc_g/invslcache.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/misc_g/invslcache.s

slcache.o: $(MWLIB)/c/misc_g/slcache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache.c

slcache_fblk.o: $(MWLIB)/c/misc_g/slcache_fblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache_fblk.c

slcache_fl.o: $(MWLIB)/c/misc_g/slcache_fl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache_fl.c

slcache_iblk.o: $(MWLIB)/c/misc_g/slcache_iblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache_iblk.c

slcache_inv.o: $(MWLIB)/c/misc_g/slcache_inv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache_inv.c

slcache_lblk.o: $(MWLIB)/c/misc_g/slcache_lblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache_lblk.c

slcache_pblk.o: $(MWLIB)/c/misc_g/slcache_pblk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/slcache_pblk.c

bputil.o: $(MWLIB)/c/misc_g/bputil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/bputil.c

gprof_ct.o: $(MWLIB)/c/misc_g/gprof_ct.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/gprof_ct.c

iprof_ct.o: $(MWLIB)/c/misc_g/iprof_ct.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/iprof_ct.c

iprofclk.o: $(MWLIB)/c/misc_g/iprofclk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/iprofclk.c

iprofile.o: $(MWLIB)/c/misc_g/iprofile.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/iprofile.c

lzcompress.o: $(MWLIB)/c/misc_g/lzcompress.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/lzcompress.c

lzdecompress.o: $(MWLIB)/c/misc_g/lzdecompress.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/lzdecompress.c

matherrx.o: $(MWLIB)/c/misc_g/matherrx.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/matherrx.c

mwlock.o: $(MWLIB)/c/misc_g/mwlock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/mwlock.c

pictable.o: $(MWLIB)/c/misc_g/pictable.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/pictable.c

prof_ct.o: $(MWLIB)/c/misc_g/prof_ct.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/prof_ct.c

profrt.o: $(MWLIB)/c/misc_g/profrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/profrt.c

trace.o: $(MWLIB)/c/misc_g/trace.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/trace.c

arc_main.o: $(MWLIB)/c/startup_g/arc_main.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) -U_DSPC $(MWLIB)/c/startup_g/arc_main.c

arc_main_lin.o: $(MWLIB)/c/startup_g/arc_main_lin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/startup_g/arc_main_lin.c

breakp.o: $(MWLIB)/c/startup_g/breakp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/breakp.s

crt1.o: $(MWLIB)/c/startup_g/crt1.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/crt1.s

crt1pt.o: $(MWLIB)/c/startup_g/crt1pt.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/crt1pt.s

crt1_rib.o: $(MWLIB)/c/startup_g/crt1_rib.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/crt1_rib.s

crt1pt_rib.o: $(MWLIB)/c/startup_g/crt1pt_rib.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/crt1pt_rib.s

crti.o: $(MWLIB)/c/startup_g/crti.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/crti.s

crtn.o: $(MWLIB)/c/startup_g/crtn.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/crtn.s

crt_initbss.o: $(MWLIB)/c/startup_g/crt_initbss.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/startup_g/crt_initbss.c

crt_initcopy.o: $(MWLIB)/c/startup_g/crt_initcopy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/startup_g/crt_initcopy.c

crt_inittimer.o: $(MWLIB)/c/startup_g/crt_inittimer.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/startup_g/crt_inittimer.c

crt_invcache.o: $(MWLIB)/c/startup_g/crt_invcache.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/startup_g/crt_invcache.c

initpic.o: $(MWLIB)/c/startup_g/initpic.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/initpic.s

vectors.o: $(MWLIB)/c/startup_g/vectors.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/vectors.s

vxcrt1.o: $(MWLIB)/c/startup_g/vxcrt1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/startup_g/vxcrt1.c

xcheck.o: $(MWLIB)/c/startup_g/xcheck.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/xcheck.s

xcheckno.o: $(MWLIB)/c/startup_g/xcheckno.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/startup_g/xcheckno.s

_assert.o: $(MWLIB)/c/std_g/_assert.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_assert.c

_atold.o: $(MWLIB)/c/std_g/_atold.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_atold.c

_atouni.o: $(MWLIB)/c/std_g/_atouni.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_atouni.c

_cleanup.o: $(MWLIB)/c/std_g/_cleanup.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_cleanup.c

_compare.o: $(MWLIB)/c/std_g/_compare.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_compare.c

_doatof.o: $(MWLIB)/c/std_g/_doatof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_doatof.c

_doatofd.o: $(MWLIB)/c/std_g/_doatofd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_doatofd.c

_doecvt.o: $(MWLIB)/c/std_g/_doecvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_doecvt.c

_doecvtd.o: $(MWLIB)/c/std_g/_doecvtd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_doecvtd.c

_doprint.o: $(MWLIB)/c/std_g/_doprint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) -U_DSPC $(MWLIB)/c/std_g/_doprint.c

_doscan.o: $(MWLIB)/c/std_g/_doscan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) -U_DSPC $(MWLIB)/c/std_g/_doscan.c

_fillchr.o: $(MWLIB)/c/std_g/_fillchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_fillchr.c

_find.o: $(MWLIB)/c/std_g/_find.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_find.c

_itoa.o: $(MWLIB)/c/std_g/_itoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_itoa.c

_itos.o: $(MWLIB)/c/std_g/_itos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_itos.c

_ltoa.o: $(MWLIB)/c/std_g/_ltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ltoa.c

_move.o: $(MWLIB)/c/std_g/_move.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_move.c

_pow10.o: $(MWLIB)/c/std_g/_pow10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_pow10.c

_pow10d.o: $(MWLIB)/c/std_g/_pow10d.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_pow10d.c

_round.o: $(MWLIB)/c/std_g/_round.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_round.c

_roundd.o: $(MWLIB)/c/std_g/_roundd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_roundd.c

_smalloc.o: $(MWLIB)/c/std_g/_smalloc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_smalloc.c

_stoa.o: $(MWLIB)/c/std_g/_stoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_stoa.c

_stoi.o: $(MWLIB)/c/std_g/_stoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_stoi.c

_strrev.o: $(MWLIB)/c/std_g/_strrev.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_strrev.c

_u_ltoa.o: $(MWLIB)/c/std_g/_u_ltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_u_ltoa.c

_uassert.o: $(MWLIB)/c/std_g/_uassert.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uassert.c

_uatof.o: $(MWLIB)/c/std_g/_uatof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uatof.c

_uatoi.o: $(MWLIB)/c/std_g/_uatoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uatoi.c

_uatol.o: $(MWLIB)/c/std_g/_uatol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uatol.c

_uctypef.o: $(MWLIB)/c/std_g/_uctypef.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uctypef.c

_ucvt.o: $(MWLIB)/c/std_g/_ucvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ucvt.c

_ucvtld.o: $(MWLIB)/c/std_g/_ucvtld.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ucvtld.c

_udoatfd.o: $(MWLIB)/c/std_g/_udoatfd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_udoatfd.c

_udoatof.o: $(MWLIB)/c/std_g/_udoatof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_udoatof.c

_udoecvd.o: $(MWLIB)/c/std_g/_udoecvd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_udoecvd.c

_udoecvt.o: $(MWLIB)/c/std_g/_udoecvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_udoecvt.c

_udoprnt.o: $(MWLIB)/c/std_g/_udoprnt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_udoprnt.c

_udoscan.o: $(MWLIB)/c/std_g/_udoscan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_udoscan.c

_ugcvt.o: $(MWLIB)/c/std_g/_ugcvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ugcvt.c

_ugetenv.o: $(MWLIB)/c/std_g/_ugetenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ugetenv.c

_uitoa.o: $(MWLIB)/c/std_g/_uitoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uitoa.c

_uitos.o: $(MWLIB)/c/std_g/_uitos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uitos.c

_ultoa.o: $(MWLIB)/c/std_g/_ultoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ultoa.c

_ustoi.o: $(MWLIB)/c/std_g/_ustoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustoi.c

_ustrrev.o: $(MWLIB)/c/std_g/_ustrrev.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrrev.c

_ustrtod.o: $(MWLIB)/c/std_g/_ustrtod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrtod.c

_ustrtof.o: $(MWLIB)/c/std_g/_ustrtof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrtof.c

_ustrtol.o: $(MWLIB)/c/std_g/_ustrtol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrtol.c

_ustrtul.o: $(MWLIB)/c/std_g/_ustrtul.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrtul.c

_uultoa.o: $(MWLIB)/c/std_g/_uultoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uultoa.c

abort.o: $(MWLIB)/c/std_g/abort.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/abort.c

abort_handler.o: $(MWLIB)/c/std_g/abort_handler.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/abort_handler.c

setjmp.o: $(MWLIB)/c/std_g/setjmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/std_g/setjmp.s

arith.o: $(MWLIB)/c/std_g/arith.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/arith.c

atexit.o: $(MWLIB)/c/std_g/atexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/atexit.c

atof.o: $(MWLIB)/c/std_g/atof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/atof.c

atoi.o: $(MWLIB)/c/std_g/atoi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/atoi.c

atol.o: $(MWLIB)/c/std_g/atol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/atol.c

bsearch.o: $(MWLIB)/c/std_g/bsearch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/bsearch.c

calloc.o: $(MWLIB)/c/std_g/calloc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/calloc.c

cfree.o: $(MWLIB)/c/std_g/cfree.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/cfree.c

constraint.o: $(MWLIB)/c/std_g/constraint.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/constraint.c

ctype.o: $(MWLIB)/c/std_g/ctype.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/ctype.c

ctypef.o: $(MWLIB)/c/std_g/ctypef.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/ctypef.c

ctype_l.o: $(MWLIB)/c/std_g/ctype_l.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/ctype_l.c

cvt.o: $(MWLIB)/c/std_g/cvt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/cvt.c

cvt2.o: $(MWLIB)/c/std_g/cvt2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/cvt2.c

cvtld.o: $(MWLIB)/c/std_g/cvtld.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/cvtld.c

exit.o: $(MWLIB)/c/std_g/exit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/exit.c

findchr.o: $(MWLIB)/c/std_g/findchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/findchr.c

imaxabs.o: $(MWLIB)/c/std_g/imaxabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/imaxabs.c

imaxdiv.o: $(MWLIB)/c/std_g/imaxdiv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/imaxdiv.c

initcopy.o: $(MWLIB)/c/std_g/initcopy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/initcopy.c

initcopyp.o: $(MWLIB)/c/std_g/initcopyp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/initcopyp.c

isascii.o: $(MWLIB)/c/std_g/isascii.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/isascii.c

locale.o: $(MWLIB)/c/std_g/locale.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/locale.c

localeco.o: $(MWLIB)/c/std_g/localeco.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/localeco.c

malloc2.o: $(MWLIB)/c/std_g/malloc2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/malloc2.c

malloc2x.o: $(MWLIB)/c/std_g/malloc2x.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/malloc2x.c

mblen.o: $(MWLIB)/c/std_g/mblen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/mblen.c

mbstowcs.o: $(MWLIB)/c/std_g/mbstowcs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/mbstowcs.c

mbtowc.o: $(MWLIB)/c/std_g/mbtowc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/mbtowc.c

mkstemp.o: $(MWLIB)/c/std_g/mkstemp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/mkstemp.c

nofloatp.o: $(MWLIB)/c/std_g/nofloatp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/nofloatp.c

nofloats.o: $(MWLIB)/c/std_g/nofloats.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/nofloats.c

onexit.o: $(MWLIB)/c/std_g/onexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/onexit.c

pfnoflt.o: $(MWLIB)/c/std_g/pfnoflt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/pfnoflt.c

qsort.o: $(MWLIB)/c/std_g/qsort.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/qsort.c

rand.o: $(MWLIB)/c/std_g/rand.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/rand.c

rsizet.o: $(MWLIB)/c/std_g/rsizet.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/rsizet.c

setlocal.o: $(MWLIB)/c/std_g/setlocal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/setlocal.c

sfnoflt.o: $(MWLIB)/c/std_g/sfnoflt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/sfnoflt.c

signal.o: $(MWLIB)/c/std_g/signal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/signal.c

skipchr.o: $(MWLIB)/c/std_g/skipchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/skipchr.c

strtod.o: $(MWLIB)/c/std_g/strtod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/strtod.c

strtof.o: $(MWLIB)/c/std_g/strtof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/strtof.c

strtol.o: $(MWLIB)/c/std_g/strtol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/strtol.c

strtoul.o: $(MWLIB)/c/std_g/strtoul.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/strtoul.c

toascii.o: $(MWLIB)/c/std_g/toascii.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/toascii.c

wcstombs.o: $(MWLIB)/c/std_g/wcstombs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/wcstombs.c

wctomb.o: $(MWLIB)/c/std_g/wctomb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/wctomb.c

_lltoa.o: $(MWLIB)/c/std_g/_lltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_lltoa.c

_lltos.o: $(MWLIB)/c/std_g/_lltos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_lltos.c

_stoll.o: $(MWLIB)/c/std_g/_stoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_stoll.c

_u_lltoa.o: $(MWLIB)/c/std_g/_u_lltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_u_lltoa.c

_u_ulltoa.o: $(MWLIB)/c/std_g/_u_ulltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_u_ulltoa.c

_uatoll.o: $(MWLIB)/c/std_g/_uatoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_uatoll.c

_ulltoa.o: $(MWLIB)/c/std_g/_ulltoa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ulltoa.c

_ulltos.o: $(MWLIB)/c/std_g/_ulltos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ulltos.c

_ustoll.o: $(MWLIB)/c/std_g/_ustoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustoll.c

_ustrtoll.o: $(MWLIB)/c/std_g/_ustrtoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrtoll.c

_ustrtoull.o: $(MWLIB)/c/std_g/_ustrtoull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/_ustrtoull.c

atoll.o: $(MWLIB)/c/std_g/atoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/atoll.c

strtoll.o: $(MWLIB)/c/std_g/strtoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/strtoll.c

strtoull.o: $(MWLIB)/c/std_g/strtoull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/strtoull.c

ulltod.o: $(MWLIB)/c/std_g/ulltod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/std_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/std_g/ulltod.c

_memccpy.o: $(MWLIB)/c/str_g/_memccpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_memccpy.c

_memicmp.o: $(MWLIB)/c/str_g/_memicmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_memicmp.c

_rmemcpy.o: $(MWLIB)/c/str_g/_rmemcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_rmemcpy.c

_rstrcpy.o: $(MWLIB)/c/str_g/_rstrcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_rstrcpy.c

_rstrncp.o: $(MWLIB)/c/str_g/_rstrncp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_rstrncp.c

_serr_nl.o: $(MWLIB)/c/str_g/_serr_nl.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_serr_nl.c

_strcats.o: $(MWLIB)/c/str_g/_strcats.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strcats.c

_strcmpi.o: $(MWLIB)/c/str_g/_strcmpi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strcmpi.c

_strdup.o: $(MWLIB)/c/str_g/_strdup.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strdup.c

_stricmp.o: $(MWLIB)/c/str_g/_stricmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_stricmp.c

_strlwr.o: $(MWLIB)/c/str_g/_strlwr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strlwr.c

_strncat.o: $(MWLIB)/c/str_g/_strncat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strncat.c

_strnicm.o: $(MWLIB)/c/str_g/_strnicm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strnicm.c

_strnset.o: $(MWLIB)/c/str_g/_strnset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strnset.c

_strset.o: $(MWLIB)/c/str_g/_strset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strset.c

_strtokr.o: $(MWLIB)/c/str_g/_strtokr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strtokr.c

_strupr.o: $(MWLIB)/c/str_g/_strupr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_strupr.c

_uchrccp.o: $(MWLIB)/c/str_g/_uchrccp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_uchrccp.c

_uchrchr.o: $(MWLIB)/c/str_g/_uchrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_uchrchr.c

_uchrcmp.o: $(MWLIB)/c/str_g/_uchrcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_uchrcmp.c

_uchrcpy.o: $(MWLIB)/c/str_g/_uchrcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_uchrcpy.c

_uchricm.o: $(MWLIB)/c/str_g/_uchricm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_uchricm.c

_uchrset.o: $(MWLIB)/c/str_g/_uchrset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_uchrset.c

_ustrcat.o: $(MWLIB)/c/str_g/_ustrcat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrcat.c

_ustrchr.o: $(MWLIB)/c/str_g/_ustrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrchr.c

_ustrcmi.o: $(MWLIB)/c/str_g/_ustrcmi.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrcmi.c

_ustrcmp.o: $(MWLIB)/c/str_g/_ustrcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrcmp.c

_ustrcpy.o: $(MWLIB)/c/str_g/_ustrcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrcpy.c

_ustrcsp.o: $(MWLIB)/c/str_g/_ustrcsp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrcsp.c

_ustrdup.o: $(MWLIB)/c/str_g/_ustrdup.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrdup.c

_ustrerr.o: $(MWLIB)/c/str_g/_ustrerr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrerr.c

_ustricm.o: $(MWLIB)/c/str_g/_ustricm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustricm.c

_ustrlen.o: $(MWLIB)/c/str_g/_ustrlen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrlen.c

_ustrlwr.o: $(MWLIB)/c/str_g/_ustrlwr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrlwr.c

_ustrnca.o: $(MWLIB)/c/str_g/_ustrnca.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrnca.c

_ustrncm.o: $(MWLIB)/c/str_g/_ustrncm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrncm.c

_ustrncp.o: $(MWLIB)/c/str_g/_ustrncp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrncp.c

_ustrnic.o: $(MWLIB)/c/str_g/_ustrnic.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrnic.c

_ustrnst.o: $(MWLIB)/c/str_g/_ustrnst.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrnst.c

_ustrpbr.o: $(MWLIB)/c/str_g/_ustrpbr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrpbr.c

_ustrrch.o: $(MWLIB)/c/str_g/_ustrrch.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrrch.c

_ustrset.o: $(MWLIB)/c/str_g/_ustrset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrset.c

_ustrspn.o: $(MWLIB)/c/str_g/_ustrspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrspn.c

_ustrstr.o: $(MWLIB)/c/str_g/_ustrstr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrstr.c

_ustrtkr.o: $(MWLIB)/c/str_g/_ustrtkr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrtkr.c

_ustrtok.o: $(MWLIB)/c/str_g/_ustrtok.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrtok.c

_ustrupr.o: $(MWLIB)/c/str_g/_ustrupr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_ustrupr.c

_vdmemmove.o: $(MWLIB)/c/str_g/_vdmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_vdmemmove.c

_vmemset.o: $(MWLIB)/c/str_g/_vmemset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_vmemset.c

_vsdmemmove.o: $(MWLIB)/c/str_g/_vsdmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_vsdmemmove.c

_vsmemmove.o: $(MWLIB)/c/str_g/_vsmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/_vsmemmove.c

_vdmemcpy.o: $(MWLIB)/c/str_g/_vdmemcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/str_g/_vdmemcpy.s

_vsdmemcpy.o: $(MWLIB)/c/str_g/_vsdmemcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/str_g/_vsdmemcpy.s

_vsmemcpy.o: $(MWLIB)/c/str_g/_vsmemcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/str_g/_vsmemcpy.s

memcpy.o: $(MWLIB)/c/str_g/memcpy.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/str_g/memcpy.s

memcpy_s.o: $(MWLIB)/c/str_g/memcpy_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/memcpy_s.c

_a5_strcmp.o: $(MWLIB)/c/str_g/_a5_strcmp.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/str_g/_a5_strcmp.s

_a5_strlen.o: $(MWLIB)/c/str_g/_a5_strlen.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c/str_g/_a5_strlen.s

f_memset.o: $(MWLIB)/c/str_g/f_memset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) -O $(MWLIB)/c/str_g/f_memset.c

memchr.o: $(MWLIB)/c/str_g/memchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/memchr.c

memcmp.o: $(MWLIB)/c/str_g/memcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/memcmp.c

memmove.o: $(MWLIB)/c/str_g/memmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/memmove.c

memmove_s.o: $(MWLIB)/c/str_g/memmove_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/memmove_s.c

memset_s.o: $(MWLIB)/c/str_g/memset_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/memset_s.c

s_memcpy.o: $(MWLIB)/c/str_g/s_memcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/s_memcpy.c

s_memset.o: $(MWLIB)/c/str_g/s_memset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/s_memset.c

strcasecmp.o: $(MWLIB)/c/str_g/strcasecmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcasecmp.c

strcat.o: $(MWLIB)/c/str_g/strcat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcat.c

strcat_s.o: $(MWLIB)/c/str_g/strcat_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcat_s.c

strchr.o: $(MWLIB)/c/str_g/strchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strchr.c

strcoll.o: $(MWLIB)/c/str_g/strcoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcoll.c

strcpy.o: $(MWLIB)/c/str_g/strcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcpy.c

strcpy_s.o: $(MWLIB)/c/str_g/strcpy_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcpy_s.c

strcspn.o: $(MWLIB)/c/str_g/strcspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strcspn.c

strerror.o: $(MWLIB)/c/str_g/strerror.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strerror.c

strerror_r.o: $(MWLIB)/c/str_g/strerror_r.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strerror_r.c

strerror_s.o: $(MWLIB)/c/str_g/strerror_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strerror_s.c

strnlen_s.o: $(MWLIB)/c/str_g/strnlen_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strnlen_s.c

strncasecmp.o: $(MWLIB)/c/str_g/strncasecmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strncasecmp.c

strncat.o: $(MWLIB)/c/str_g/strncat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strncat.c

strncat_s.o: $(MWLIB)/c/str_g/strncat_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strncat_s.c

strncmp.o: $(MWLIB)/c/str_g/strncmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strncmp.c

strncpy.o: $(MWLIB)/c/str_g/strncpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strncpy.c

strncpy_s.o: $(MWLIB)/c/str_g/strncpy_s.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strncpy_s.c

strpbrk.o: $(MWLIB)/c/str_g/strpbrk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strpbrk.c

strrchr.o: $(MWLIB)/c/str_g/strrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strrchr.c

strspn.o: $(MWLIB)/c/str_g/strspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strspn.c

strstr.o: $(MWLIB)/c/str_g/strstr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strstr.c

strtok.o: $(MWLIB)/c/str_g/strtok.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strtok.c

strxfrm.o: $(MWLIB)/c/str_g/strxfrm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/str_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/str_g/strxfrm.c

clkgettm.o: $(MWLIB)/c/time_g/clkgettm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/clkgettm.c

_asctime.o: $(MWLIB)/c/time_g/_asctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_asctime.c

_ctime.o: $(MWLIB)/c/time_g/_ctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_ctime.c

_get_tz.o: $(MWLIB)/c/time_g/_get_tz.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_get_tz.c

_gmtime.o: $(MWLIB)/c/time_g/_gmtime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_gmtime.c

_localtm.o: $(MWLIB)/c/time_g/_localtm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_localtm.c

_mktime.o: $(MWLIB)/c/time_g/_mktime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_mktime.c

_strdate.o: $(MWLIB)/c/time_g/_strdate.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_strdate.c

_strtime.o: $(MWLIB)/c/time_g/_strtime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_strtime.c

_tzdata.o: $(MWLIB)/c/time_g/_tzdata.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_tzdata.c

_tzset.o: $(MWLIB)/c/time_g/_tzset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_tzset.c

_u_ascti.o: $(MWLIB)/c/time_g/_u_ascti.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_u_ascti.c

_u_ctime.o: $(MWLIB)/c/time_g/_u_ctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_u_ctime.c

_uasctim.o: $(MWLIB)/c/time_g/_uasctim.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_uasctim.c

_uctime.o: $(MWLIB)/c/time_g/_uctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_uctime.c

_ustrfti.o: $(MWLIB)/c/time_g/_ustrfti.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_ustrfti.c

_utzdata.o: $(MWLIB)/c/time_g/_utzdata.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_utzdata.c

_utzset.o: $(MWLIB)/c/time_g/_utzset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/_utzset.c

asctime.o: $(MWLIB)/c/time_g/asctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/asctime.c

ctime.o: $(MWLIB)/c/time_g/ctime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/ctime.c

difftime.o: $(MWLIB)/c/time_g/difftime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/difftime.c

gmtime.o: $(MWLIB)/c/time_g/gmtime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/gmtime.c

localtm.o: $(MWLIB)/c/time_g/localtm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/localtm.c

mktime.o: $(MWLIB)/c/time_g/mktime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/mktime.c

strftime.o: $(MWLIB)/c/time_g/strftime.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/time_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/time_g/strftime.c

system.o: $(MWLIB)/c/unix_g/system.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/unix_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/unix_g/system.c

fenv.o: $(MWLIB)/c/misc_g/fenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/misc_g/fenv.c

cmplx.o: $(MWLIB)/c/complex_g/cmplx.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cmplx.c

cmplxf.o: $(MWLIB)/c/complex_g/cmplxf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/cmplxf.c

cabs.o: $(MWLIB)/c/complex_g/cabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/cabs.c

cabsf.o: $(MWLIB)/c/complex_g/cabsf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/cabsf.c

carg.o: $(MWLIB)/c/complex_g/carg.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/carg.c

cargf.o: $(MWLIB)/c/complex_g/cargf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/cargf.c

cacos.o: $(MWLIB)/c/complex_g/cacos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cacos.c

cacosf.o: $(MWLIB)/c/complex_g/cacosf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cacosf.c

cacosh.o: $(MWLIB)/c/complex_g/cacosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cacosh.c

cacoshf.o: $(MWLIB)/c/complex_g/cacoshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cacoshf.c

casin.o: $(MWLIB)/c/complex_g/casin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/casin.c

casinf.o: $(MWLIB)/c/complex_g/casinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/casinf.c

casinh.o: $(MWLIB)/c/complex_g/casinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/casinh.c

casinhf.o: $(MWLIB)/c/complex_g/casinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/casinhf.c

catan.o: $(MWLIB)/c/complex_g/catan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/catan.c

catanf.o: $(MWLIB)/c/complex_g/catanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/catanf.c

catanh.o: $(MWLIB)/c/complex_g/catanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/catanh.c

catanhf.o: $(MWLIB)/c/complex_g/catanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/catanhf.c

ccos.o: $(MWLIB)/c/complex_g/ccos.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ccos.c

ccosf.o: $(MWLIB)/c/complex_g/ccosf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ccosf.c

ccosh.o: $(MWLIB)/c/complex_g/ccosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ccosh.c

ccoshf.o: $(MWLIB)/c/complex_g/ccoshf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ccoshf.c

cexp.o: $(MWLIB)/c/complex_g/cexp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cexp.c

cexpf.o: $(MWLIB)/c/complex_g/cexpf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cexpf.c

clog.o: $(MWLIB)/c/complex_g/clog.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/clog.c

clogf.o: $(MWLIB)/c/complex_g/clogf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/clogf.c

cimag.o: $(MWLIB)/c/complex_g/cimag.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cimag.c

cimagf.o: $(MWLIB)/c/complex_g/cimagf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/cimagf.c

cpow.o: $(MWLIB)/c/complex_g/cpow.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cpow.c

cpowf.o: $(MWLIB)/c/complex_g/cpowf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cpowf.c

cproj.o: $(MWLIB)/c/complex_g/cproj.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cproj.c

cprojf.o: $(MWLIB)/c/complex_g/cprojf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/cprojf.c

conj.o: $(MWLIB)/c/complex_g/conj.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/conj.c

conjf.o: $(MWLIB)/c/complex_g/conjf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/conjf.c

creal.o: $(MWLIB)/c/complex_g/creal.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/creal.c

crealf.o: $(MWLIB)/c/complex_g/crealf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) -Hlto $(MWLIB)/c/complex_g/crealf.c

csin.o: $(MWLIB)/c/complex_g/csin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/csin.c

csinf.o: $(MWLIB)/c/complex_g/csinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/csinf.c

csinh.o: $(MWLIB)/c/complex_g/csinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/csinh.c

csinhf.o: $(MWLIB)/c/complex_g/csinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/csinhf.c

csqrt.o: $(MWLIB)/c/complex_g/csqrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/csqrt.c

csqrtf.o: $(MWLIB)/c/complex_g/csqrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/csqrtf.c

ctan.o: $(MWLIB)/c/complex_g/ctan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ctan.c

ctanf.o: $(MWLIB)/c/complex_g/ctanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ctanf.c

ctanh.o: $(MWLIB)/c/complex_g/ctanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ctanh.c

ctanhf.o: $(MWLIB)/c/complex_g/ctanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/complex_g/ctanhf.c

fgetwc.o: $(MWLIB)/c/wfio_g/fgetwc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/fgetwc.c

fgetws.o: $(MWLIB)/c/wfio_g/fgetws.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/fgetws.c

fputwc.o: $(MWLIB)/c/wfio_g/fputwc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/fputwc.c

fputws.o: $(MWLIB)/c/wfio_g/fputws.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/fputws.c

fwide.o: $(MWLIB)/c/wfio_g/fwide.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/fwide.c

getwchar.o: $(MWLIB)/c/wfio_g/getwchar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/getwchar.c

putwchar.o: $(MWLIB)/c/wfio_g/putwchar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/putwchar.c

swprintf.o: $(MWLIB)/c/wfio_g/swprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/swprintf.c

swscanf.o: $(MWLIB)/c/wfio_g/swscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/swscanf.c

ungetwc.o: $(MWLIB)/c/wfio_g/ungetwc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/ungetwc.c

wfio.o: $(MWLIB)/c/wfio_g/wfio.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/wfio.c

wfprintf.o: $(MWLIB)/c/wfio_g/wfprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/wfprintf.c

wfread.o: $(MWLIB)/c/wfio_g/wfread.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/wfread.c

wfwrite.o: $(MWLIB)/c/wfio_g/wfwrite.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/wfwrite.c

wscanf.o: $(MWLIB)/c/wfio_g/wscanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wfio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wfio_g/wscanf.c

btowc.o: $(MWLIB)/c/wstr_g/btowc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/btowc.c

mbrlen.o: $(MWLIB)/c/wstr_g/mbrlen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/mbrlen.c

mbrtowc.o: $(MWLIB)/c/wstr_g/mbrtowc.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/mbrtowc.c

mbsinit.o: $(MWLIB)/c/wstr_g/mbsinit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/mbsinit.c

mbsrtowcs.o: $(MWLIB)/c/wstr_g/mbsrtowcs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/mbsrtowcs.c

mbsnrtowcs.o: $(MWLIB)/c/wstr_g/mbsnrtowcs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/mbsnrtowcs.c

utf8.o: $(MWLIB)/c/wstr_g/utf8.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/utf8.c

wcrtomb.o: $(MWLIB)/c/wstr_g/wcrtomb.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcrtomb.c

wcscat.o: $(MWLIB)/c/wstr_g/wcscat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcscat.c

wcschr.o: $(MWLIB)/c/wstr_g/wcschr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcschr.c

wcscmp.o: $(MWLIB)/c/wstr_g/wcscmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcscmp.c

wcscoll.o: $(MWLIB)/c/wstr_g/wcscoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcscoll.c

wcscpy.o: $(MWLIB)/c/wstr_g/wcscpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcscpy.c

wcscspn.o: $(MWLIB)/c/wstr_g/wcscspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcscspn.c

wcslen.o: $(MWLIB)/c/wstr_g/wcslen.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcslen.c

wcsncat.o: $(MWLIB)/c/wstr_g/wcsncat.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsncat.c

wcsncmp.o: $(MWLIB)/c/wstr_g/wcsncmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsncmp.c

wcsncpy.o: $(MWLIB)/c/wstr_g/wcsncpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsncpy.c

wcspbrk.o: $(MWLIB)/c/wstr_g/wcspbrk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcspbrk.c

wcsrchr.o: $(MWLIB)/c/wstr_g/wcsrchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsrchr.c

wcsnrtombs.o: $(MWLIB)/c/wstr_g/wcsnrtombs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsnrtombs.c

wcsrtombs.o: $(MWLIB)/c/wstr_g/wcsrtombs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsrtombs.c

wcsspn.o: $(MWLIB)/c/wstr_g/wcsspn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsspn.c

wcsstr.o: $(MWLIB)/c/wstr_g/wcsstr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsstr.c

wcstod.o: $(MWLIB)/c/wstr_g/wcstod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstod.c

wcstof.o: $(MWLIB)/c/wstr_g/wcstof.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstof.c

wcstok.o: $(MWLIB)/c/wstr_g/wcstok.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstok.c

wcstol.o: $(MWLIB)/c/wstr_g/wcstol.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstol.c

wcstoll.o: $(MWLIB)/c/wstr_g/wcstoll.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstoll.c

wcstoul.o: $(MWLIB)/c/wstr_g/wcstoul.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstoul.c

wcstoull.o: $(MWLIB)/c/wstr_g/wcstoull.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcstoull.c

wcsxfrm.o: $(MWLIB)/c/wstr_g/wcsxfrm.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wcsxfrm.c

wctob.o: $(MWLIB)/c/wstr_g/wctob.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wctob.c

wctype.o: $(MWLIB)/c/wstr_g/wctype.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wctype.c

wctrans.o: $(MWLIB)/c/wstr_g/wctrans.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wctrans.c

wctype_l.o: $(MWLIB)/c/wstr_g/wctype_l.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wctype_l.c

wmemchr.o: $(MWLIB)/c/wstr_g/wmemchr.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wmemchr.c

wmemcmp.o: $(MWLIB)/c/wstr_g/wmemcmp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wmemcmp.c

wmemcpy.o: $(MWLIB)/c/wstr_g/wmemcpy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wmemcpy.c

wmemmove.o: $(MWLIB)/c/wstr_g/wmemmove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wmemmove.c

wmemset.o: $(MWLIB)/c/wstr_g/wmemset.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/wstr_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/wstr_g/wmemset.c

cxa_atexit.o: $(MWLIB)/c/llvm_g/cxa_atexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/llvm_g/cxa_atexit.c

cxa_thread_atexit.o: $(MWLIB)/c/llvm_g/cxa_thread_atexit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c/llvm_g/cxa_thread_atexit.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
