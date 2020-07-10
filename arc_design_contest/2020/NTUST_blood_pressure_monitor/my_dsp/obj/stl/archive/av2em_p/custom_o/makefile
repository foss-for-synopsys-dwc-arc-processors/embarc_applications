# libstl arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hpurge -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
CC_INC= -I$(MWLIB)/stl/stlport -I$(MWLIB)/c/inc -I$(MWLIB)/stl/incc
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libstl.a
AR_OPT=""

LIST_stl=allocators.o bitset.o c_locale.o codecvt.o collate.o complex.o\
	complex_io.o complex_trig.o ctype.o cxa.o dll_main.o facets_byname.o\
	fstream.o instantiate.o ios.o iostream.o istream.o locale.o\
	locale_catalog.o locale_impl.o math_dummy.o messages.o monetary.o\
	num_get.o num_get_float.o num_put.o num_put_float.o numpunct.o\
	ostream.o sstream.o stdio_streambuf.o string.o strstream.o\
	time_facets.o

LIST_misc=newhandl.o badalloc.o eh_exception.o opdel.o opdela.o opnew.o\
	opnewa.o opnewap.o opnewp.o

LIST_llvm=cxa_demangle.o eh_alloc.o eh_aux_runtime.o eh_call.o eh_catch.o\
	eh_globals.o eh_personality.o pure.o eh_term_handler.o eh_terminate.o\
	eh_throw.o eh_type.o eh_unex_handler.o guard.o tinfo1.o tinfo2.o\
	unwind-dw2.o unwind-dw2-fde.o vec.o

LIST_ALL=$(LIST_stl) $(LIST_misc) $(LIST_llvm)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_stl)
	$(AR) rv $(AR_NAME) $(LIST_misc)
	$(AR) rv $(AR_NAME) $(LIST_llvm)

stl: $(LIST_stl)
misc: $(LIST_misc)
llvm: $(LIST_llvm)

allocators.o: $(MWLIB)/stl/stl_g/allocators.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/allocators.cpp

bitset.o: $(MWLIB)/stl/stl_g/bitset.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/bitset.cpp

c_locale.o: $(MWLIB)/stl/stl_g/c_locale.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/c_locale.c

codecvt.o: $(MWLIB)/stl/stl_g/codecvt.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/codecvt.cpp

collate.o: $(MWLIB)/stl/stl_g/collate.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/collate.cpp

complex.o: $(MWLIB)/stl/stl_g/complex.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/complex.cpp

complex_io.o: $(MWLIB)/stl/stl_g/complex_io.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/complex_io.cpp

complex_trig.o: $(MWLIB)/stl/stl_g/complex_trig.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/complex_trig.cpp

ctype.o: $(MWLIB)/stl/stl_g/ctype.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/ctype.cpp

cxa.o: $(MWLIB)/stl/stl_g/cxa.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/cxa.c

dll_main.o: $(MWLIB)/stl/stl_g/dll_main.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/dll_main.cpp

facets_byname.o: $(MWLIB)/stl/stl_g/facets_byname.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/facets_byname.cpp

fstream.o: $(MWLIB)/stl/stl_g/fstream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/fstream.cpp

instantiate.o: $(MWLIB)/stl/stl_g/instantiate.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/instantiate.cpp

ios.o: $(MWLIB)/stl/stl_g/ios.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/ios.cpp

iostream.o: $(MWLIB)/stl/stl_g/iostream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/iostream.cpp

istream.o: $(MWLIB)/stl/stl_g/istream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/istream.cpp

locale.o: $(MWLIB)/stl/stl_g/locale.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/locale.cpp

locale_catalog.o: $(MWLIB)/stl/stl_g/locale_catalog.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/locale_catalog.cpp

locale_impl.o: $(MWLIB)/stl/stl_g/locale_impl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/locale_impl.cpp

math_dummy.o: $(MWLIB)/stl/stl_g/math_dummy.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/math_dummy.c

messages.o: $(MWLIB)/stl/stl_g/messages.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/messages.cpp

monetary.o: $(MWLIB)/stl/stl_g/monetary.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/monetary.cpp

num_get.o: $(MWLIB)/stl/stl_g/num_get.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/num_get.cpp

num_get_float.o: $(MWLIB)/stl/stl_g/num_get_float.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/num_get_float.cpp

num_put.o: $(MWLIB)/stl/stl_g/num_put.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/num_put.cpp

num_put_float.o: $(MWLIB)/stl/stl_g/num_put_float.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/num_put_float.cpp

numpunct.o: $(MWLIB)/stl/stl_g/numpunct.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/numpunct.cpp

ostream.o: $(MWLIB)/stl/stl_g/ostream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/ostream.cpp

sstream.o: $(MWLIB)/stl/stl_g/sstream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/sstream.cpp

stdio_streambuf.o: $(MWLIB)/stl/stl_g/stdio_streambuf.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/stdio_streambuf.cpp

string.o: $(MWLIB)/stl/stl_g/string.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/string.cpp

strstream.o: $(MWLIB)/stl/stl_g/strstream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/strstream.cpp

time_facets.o: $(MWLIB)/stl/stl_g/time_facets.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/stl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/stl_g/time_facets.cpp

newhandl.o: $(MWLIB)/stl/misc_g/newhandl.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/newhandl.cpp

badalloc.o: $(MWLIB)/stl/misc_g/badalloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/badalloc.cpp

eh_exception.o: $(MWLIB)/stl/misc_g/eh_exception.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/eh_exception.cpp

opdel.o: $(MWLIB)/stl/misc_g/opdel.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/opdel.cpp

opdela.o: $(MWLIB)/stl/misc_g/opdela.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/opdela.cpp

opnew.o: $(MWLIB)/stl/misc_g/opnew.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/opnew.cpp

opnewa.o: $(MWLIB)/stl/misc_g/opnewa.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/opnewa.cpp

opnewap.o: $(MWLIB)/stl/misc_g/opnewap.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/opnewap.cpp

opnewp.o: $(MWLIB)/stl/misc_g/opnewp.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/misc_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/misc_g/opnewp.cpp

cxa_demangle.o: $(MWLIB)/stl/llvm_g/cxa_demangle.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/llvm_g/cxa_demangle.cc

eh_alloc.o: $(MWLIB)/stl/llvm_g/eh_alloc.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_alloc.cc

eh_aux_runtime.o: $(MWLIB)/stl/llvm_g/eh_aux_runtime.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_aux_runtime.cc

eh_call.o: $(MWLIB)/stl/llvm_g/eh_call.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_call.cc

eh_catch.o: $(MWLIB)/stl/llvm_g/eh_catch.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_catch.cc

eh_globals.o: $(MWLIB)/stl/llvm_g/eh_globals.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_globals.cc

eh_personality.o: $(MWLIB)/stl/llvm_g/eh_personality.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_personality.cc

pure.o: $(MWLIB)/stl/llvm_g/pure.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/pure.cc

eh_term_handler.o: $(MWLIB)/stl/llvm_g/eh_term_handler.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_term_handler.cc

eh_terminate.o: $(MWLIB)/stl/llvm_g/eh_terminate.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_terminate.cc

eh_throw.o: $(MWLIB)/stl/llvm_g/eh_throw.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_throw.cc

eh_type.o: $(MWLIB)/stl/llvm_g/eh_type.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_type.cc

eh_unex_handler.o: $(MWLIB)/stl/llvm_g/eh_unex_handler.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/eh_unex_handler.cc

guard.o: $(MWLIB)/stl/llvm_g/guard.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/stl/llvm_g/guard.cc

tinfo1.o: $(MWLIB)/stl/llvm_g/tinfo1.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -fdata-sections -ffunction-sections $(MWLIB)/stl/llvm_g/tinfo1.cc

tinfo2.o: $(MWLIB)/stl/llvm_g/tinfo2.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -fdata-sections -ffunction-sections $(MWLIB)/stl/llvm_g/tinfo2.cc

unwind-dw2.o: $(MWLIB)/stl/llvm_g/unwind-dw2.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -D__METAWARE__ -Hoff=stackcheck_alloca -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/unwind-dw2.c

unwind-dw2-fde.o: $(MWLIB)/stl/llvm_g/unwind-dw2-fde.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -D__METAWARE__ -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/unwind-dw2-fde.c

vec.o: $(MWLIB)/stl/llvm_g/vec.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/stl/llvm_g $(CC_INC) -c $(OPT_LEV) -Wno-include-next-absolute-path $(MWLIB)/stl/llvm_g/vec.cc

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
