# libfpu arc.av2em.custom
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

AR_NAME=libfpu.a
AR_OPT=""

LIST_fp=_acosh.o _asinh.o _atan.o _atanf.o _atanh.o _hypot.o _j0.o _j1.o _jn.o\
	_log2.o _pandq0.o _pandq1.o _pneval.o _y0.o _y1.o _yn.o asin.o asinf.o\
	atan.o atanf.o atan2.o atan2f.o cbrt.o cbrtf.o ceil.o ceilf.o erf.o\
	exp.o exp2.o exp10.o expf.o exp2f.o exp10f.o fabs.o fabsf.o floor.o\
	floorf.o fmod.o fmodf.o fp.o lgamma.o lgammaf.o log.o logf.o log2.o\
	log2f.o log10.o log10f.o modf.o modff.o pow.o powf.o sin.o sinf.o\
	sinh.o sinhf.o sqrt.o sqrtf.o tan.o tanf.o tanh.o tanhf.o

LIST_complex=cabs.o

LIST_ALL=$(LIST_fp) $(LIST_complex)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_fp)
	$(AR) rv $(AR_NAME) $(LIST_complex)

fp: $(LIST_fp)
complex: $(LIST_complex)

_acosh.o: $(MWLIB)/fpu/fp_g/_acosh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_acosh.c

_asinh.o: $(MWLIB)/fpu/fp_g/_asinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_asinh.c

_atan.o: $(MWLIB)/fpu/fp_g/_atan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_atan.c

_atanf.o: $(MWLIB)/fpu/fp_g/_atanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_atanf.c

_atanh.o: $(MWLIB)/fpu/fp_g/_atanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_atanh.c

cabs.o: $(MWLIB)/fpu/complex_g/cabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/complex_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/complex_g/cabs.c

_hypot.o: $(MWLIB)/fpu/fp_g/_hypot.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_hypot.c

_j0.o: $(MWLIB)/fpu/fp_g/_j0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_j0.c

_j1.o: $(MWLIB)/fpu/fp_g/_j1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_j1.c

_jn.o: $(MWLIB)/fpu/fp_g/_jn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_jn.c

_log2.o: $(MWLIB)/fpu/fp_g/_log2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_log2.c

_pandq0.o: $(MWLIB)/fpu/fp_g/_pandq0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_pandq0.c

_pandq1.o: $(MWLIB)/fpu/fp_g/_pandq1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_pandq1.c

_pneval.o: $(MWLIB)/fpu/fp_g/_pneval.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_pneval.c

_y0.o: $(MWLIB)/fpu/fp_g/_y0.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_y0.c

_y1.o: $(MWLIB)/fpu/fp_g/_y1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_y1.c

_yn.o: $(MWLIB)/fpu/fp_g/_yn.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/_yn.c

asin.o: $(MWLIB)/fpu/fp_g/asin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/asin.c

asinf.o: $(MWLIB)/fpu/fp_g/asinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/asinf.c

atan.o: $(MWLIB)/fpu/fp_g/atan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/atan.c

atanf.o: $(MWLIB)/fpu/fp_g/atanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/atanf.c

atan2.o: $(MWLIB)/fpu/fp_g/atan2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/atan2.c

atan2f.o: $(MWLIB)/fpu/fp_g/atan2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/atan2f.c

cbrt.o: $(MWLIB)/fpu/fp_g/cbrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/cbrt.c

cbrtf.o: $(MWLIB)/fpu/fp_g/cbrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/cbrtf.c

ceil.o: $(MWLIB)/fpu/fp_g/ceil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/ceil.c

ceilf.o: $(MWLIB)/fpu/fp_g/ceilf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/ceilf.c

erf.o: $(MWLIB)/fpu/fp_g/erf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/erf.c

exp.o: $(MWLIB)/fpu/fp_g/exp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/exp.c

exp2.o: $(MWLIB)/fpu/fp_g/exp2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/exp2.c

exp10.o: $(MWLIB)/fpu/fp_g/exp10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/exp10.c

expf.o: $(MWLIB)/fpu/fp_g/expf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/expf.c

exp2f.o: $(MWLIB)/fpu/fp_g/exp2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/exp2f.c

exp10f.o: $(MWLIB)/fpu/fp_g/exp10f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/exp10f.c

fabs.o: $(MWLIB)/fpu/fp_g/fabs.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/fabs.c

fabsf.o: $(MWLIB)/fpu/fp_g/fabsf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/fabsf.c

floor.o: $(MWLIB)/fpu/fp_g/floor.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/floor.c

floorf.o: $(MWLIB)/fpu/fp_g/floorf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/floorf.c

fmod.o: $(MWLIB)/fpu/fp_g/fmod.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/fmod.c

fmodf.o: $(MWLIB)/fpu/fp_g/fmodf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/fmodf.c

fp.o: $(MWLIB)/fpu/fp_g/fp.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/fp.c

lgamma.o: $(MWLIB)/fpu/fp_g/lgamma.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/lgamma.c

lgammaf.o: $(MWLIB)/fpu/fp_g/lgammaf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/lgammaf.c

log.o: $(MWLIB)/fpu/fp_g/log.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/log.c

logf.o: $(MWLIB)/fpu/fp_g/logf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/logf.c

log2.o: $(MWLIB)/fpu/fp_g/log2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/log2.c

log2f.o: $(MWLIB)/fpu/fp_g/log2f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/log2f.c

log10.o: $(MWLIB)/fpu/fp_g/log10.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/log10.c

log10f.o: $(MWLIB)/fpu/fp_g/log10f.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/log10f.c

modf.o: $(MWLIB)/fpu/fp_g/modf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/modf.c

modff.o: $(MWLIB)/fpu/fp_g/modff.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/modff.c

pow.o: $(MWLIB)/fpu/fp_g/pow.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/pow.c

powf.o: $(MWLIB)/fpu/fp_g/powf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/powf.c

sin.o: $(MWLIB)/fpu/fp_g/sin.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/sin.c

sinf.o: $(MWLIB)/fpu/fp_g/sinf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/sinf.c

sinh.o: $(MWLIB)/fpu/fp_g/sinh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/sinh.c

sinhf.o: $(MWLIB)/fpu/fp_g/sinhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/sinhf.c

sqrt.o: $(MWLIB)/fpu/fp_g/sqrt.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/sqrt.c

sqrtf.o: $(MWLIB)/fpu/fp_g/sqrtf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/sqrtf.c

tan.o: $(MWLIB)/fpu/fp_g/tan.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/tan.c

tanf.o: $(MWLIB)/fpu/fp_g/tanf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/tanf.c

tanh.o: $(MWLIB)/fpu/fp_g/tanh.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/tanh.c

tanhf.o: $(MWLIB)/fpu/fp_g/tanhf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/fpu/fp_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/fpu/fp_g/tanhf.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
