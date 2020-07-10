# libc++ arc.av2em.custom.except
MWLIB=../../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30 -Hexcept
CC_INC= -Mb -std=c++1z -I$(MWLIB)/c++/inc -I$(MWLIB)/c++/abi_g/include -I$(MWLIB)/c++/unwind_g/include -I$(MWLIB)/c/inc -ffunction-sections -fdata-sections -Wall -Wextra -W -Wwrite-strings -Wno-unused-parameter -Wno-long-long -Werror=return-type -Wno-cast-align -Wno-user-defined-literals -Wno-covered-switch-default -D_LIBCPP_BUILDING_LIBRARY=
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libc++.a
AR_OPT=""

LIST_abi=abort_message.o cxa_aux_runtime.o cxa_default_handlers.o\
	cxa_demangle.o cxa_exception.o cxa_exception_storage.o cxa_guard.o\
	cxa_handlers.o cxa_noexception.o cxa_personality.o cxa_unexpected.o\
	cxa_vector.o cxa_virtual.o fallback_malloc.o private_typeinfo.o\
	stdlib_exception.o stdlib_new_delete.o stdlib_typeinfo.o

LIST_arc=thread_support.o

LIST_src=algorithm.o any.o bind.o charconv.o chrono.o condition_variable.o\
	condition_variable_destructor.o debug.o exception.o memory_resource.o\
	functional.o future.o hash.o ios.o iostream.o locale.o memory.o mutex.o\
	mutex_destructor.o new.o optional.o random.o regex.o shared_mutex.o\
	stdexcept.o string.o strstream.o system_error.o thread.o utility.o\
	variant.o valarray.o vector.o

LIST_unwind=libunwind.o Unwind-EHABI.o UnwindLevel1-gcc-ext.o UnwindLevel1.o\
	UnwindRegistersRestore.S.o UnwindRegistersSave.S.o

LIST_ALL=$(LIST_abi) $(LIST_arc) $(LIST_src) $(LIST_unwind)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_abi)
	$(AR) rv $(AR_NAME) $(LIST_arc)
	$(AR) rv $(AR_NAME) $(LIST_src)
	$(AR) rv $(AR_NAME) $(LIST_unwind)

abi: $(LIST_abi)
arc: $(LIST_arc)
src: $(LIST_src)
unwind: $(LIST_unwind)

abort_message.o: $(MWLIB)/c++/abi_g/abort_message.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/abort_message.cpp

cxa_aux_runtime.o: $(MWLIB)/c++/abi_g/cxa_aux_runtime.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_aux_runtime.cpp

cxa_default_handlers.o: $(MWLIB)/c++/abi_g/cxa_default_handlers.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_default_handlers.cpp

cxa_demangle.o: $(MWLIB)/c++/abi_g/cxa_demangle.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) -fno-exceptions $(MWLIB)/c++/abi_g/cxa_demangle.cpp

cxa_exception.o: $(MWLIB)/c++/abi_g/cxa_exception.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_exception.cpp

cxa_exception_storage.o: $(MWLIB)/c++/abi_g/cxa_exception_storage.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_exception_storage.cpp

cxa_guard.o: $(MWLIB)/c++/abi_g/cxa_guard.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_guard.cpp

cxa_handlers.o: $(MWLIB)/c++/abi_g/cxa_handlers.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_handlers.cpp

cxa_noexception.o: $(MWLIB)/c++/abi_g/cxa_noexception.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_noexception.cpp

cxa_personality.o: $(MWLIB)/c++/abi_g/cxa_personality.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_personality.cpp

cxa_unexpected.o: $(MWLIB)/c++/abi_g/cxa_unexpected.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_unexpected.cpp

cxa_vector.o: $(MWLIB)/c++/abi_g/cxa_vector.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_vector.cpp

cxa_virtual.o: $(MWLIB)/c++/abi_g/cxa_virtual.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/cxa_virtual.cpp

fallback_malloc.o: $(MWLIB)/c++/abi_g/fallback_malloc.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/fallback_malloc.cpp

private_typeinfo.o: $(MWLIB)/c++/abi_g/private_typeinfo.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/private_typeinfo.cpp

stdlib_exception.o: $(MWLIB)/c++/abi_g/stdlib_exception.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/stdlib_exception.cpp

stdlib_new_delete.o: $(MWLIB)/c++/abi_g/stdlib_new_delete.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/stdlib_new_delete.cpp

stdlib_typeinfo.o: $(MWLIB)/c++/abi_g/stdlib_typeinfo.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/abi_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/abi_g/stdlib_typeinfo.cpp

thread_support.o: $(MWLIB)/c++/arc_g/thread_support.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/arc_g $(CC_INC) -c $(OPT_LEV) -D_LIBCPP_BUILDING_THREAD_LIBRARY_EXTERNAL $(MWLIB)/c++/arc_g/thread_support.cpp

algorithm.o: $(MWLIB)/c++/src_g/algorithm.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/algorithm.cpp

any.o: $(MWLIB)/c++/src_g/any.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/any.cpp

bind.o: $(MWLIB)/c++/src_g/bind.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/bind.cpp

charconv.o: $(MWLIB)/c++/src_g/charconv.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/charconv.cpp

chrono.o: $(MWLIB)/c++/src_g/chrono.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/chrono.cpp

condition_variable.o: $(MWLIB)/c++/src_g/condition_variable.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/condition_variable.cpp

condition_variable_destructor.o: $(MWLIB)/c++/src_g/condition_variable_destructor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/condition_variable_destructor.cpp

debug.o: $(MWLIB)/c++/src_g/debug.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/debug.cpp

exception.o: $(MWLIB)/c++/src_g/exception.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) -DLIBCXXRT $(MWLIB)/c++/src_g/exception.cpp

memory_resource.o: $(MWLIB)/c++/src_g/memory_resource.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/memory_resource.cpp

functional.o: $(MWLIB)/c++/src_g/functional.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/functional.cpp

future.o: $(MWLIB)/c++/src_g/future.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/future.cpp

hash.o: $(MWLIB)/c++/src_g/hash.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/hash.cpp

ios.o: $(MWLIB)/c++/src_g/ios.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/ios.cpp

iostream.o: $(MWLIB)/c++/src_g/iostream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/iostream.cpp

locale.o: $(MWLIB)/c++/src_g/locale.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) -Mb $(MWLIB)/c++/src_g/locale.cpp

memory.o: $(MWLIB)/c++/src_g/memory.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/memory.cpp

mutex.o: $(MWLIB)/c++/src_g/mutex.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/mutex.cpp

mutex_destructor.o: $(MWLIB)/c++/src_g/mutex_destructor.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/mutex_destructor.cpp

new.o: $(MWLIB)/c++/src_g/new.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) -DLIBCXX_BUILDING_LIBCXXABI $(MWLIB)/c++/src_g/new.cpp

optional.o: $(MWLIB)/c++/src_g/optional.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/optional.cpp

random.o: $(MWLIB)/c++/src_g/random.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/random.cpp

regex.o: $(MWLIB)/c++/src_g/regex.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/regex.cpp

shared_mutex.o: $(MWLIB)/c++/src_g/shared_mutex.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/shared_mutex.cpp

stdexcept.o: $(MWLIB)/c++/src_g/stdexcept.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/stdexcept.cpp

string.o: $(MWLIB)/c++/src_g/string.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/string.cpp

strstream.o: $(MWLIB)/c++/src_g/strstream.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/strstream.cpp

system_error.o: $(MWLIB)/c++/src_g/system_error.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/system_error.cpp

thread.o: $(MWLIB)/c++/src_g/thread.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/thread.cpp

utility.o: $(MWLIB)/c++/src_g/utility.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/utility.cpp

variant.o: $(MWLIB)/c++/src_g/variant.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/variant.cpp

valarray.o: $(MWLIB)/c++/src_g/valarray.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/valarray.cpp

vector.o: $(MWLIB)/c++/src_g/vector.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/src_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/c++/src_g/vector.cpp

libunwind.o: $(MWLIB)/c++/unwind_g/libunwind.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/unwind_g $(CC_INC) -c $(OPT_LEV) -g -funwind-tables $(MWLIB)/c++/unwind_g/libunwind.cpp

Unwind-EHABI.o: $(MWLIB)/c++/unwind_g/Unwind-EHABI.cpp
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/unwind_g $(CC_INC) -c $(OPT_LEV) -g $(MWLIB)/c++/unwind_g/Unwind-EHABI.cpp

UnwindLevel1-gcc-ext.o: $(MWLIB)/c++/unwind_g/UnwindLevel1-gcc-ext.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/unwind_g $(CC_INC) -c $(OPT_LEV) -std=c99 -g $(MWLIB)/c++/unwind_g/UnwindLevel1-gcc-ext.c

UnwindLevel1.o: $(MWLIB)/c++/unwind_g/UnwindLevel1.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/c++/unwind_g $(CC_INC) -c $(OPT_LEV) -std=c99 -g $(MWLIB)/c++/unwind_g/UnwindLevel1.c

UnwindRegistersRestore.S.o: $(MWLIB)/c++/unwind_g/UnwindRegistersRestore.S
	$(AS) $(AS_OPT) $(aflags) -g $(AS_INC) -o $@ $(MWLIB)/c++/unwind_g/UnwindRegistersRestore.S

UnwindRegistersSave.S.o: $(MWLIB)/c++/unwind_g/UnwindRegistersSave.S
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/c++/unwind_g/UnwindRegistersSave.S

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
