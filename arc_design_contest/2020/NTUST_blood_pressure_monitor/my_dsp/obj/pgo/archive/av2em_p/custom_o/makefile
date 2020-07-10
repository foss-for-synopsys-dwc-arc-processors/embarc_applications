# libpgo arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hclangopt=-triple -Hclangopt=arc-unknown-unknown-gnu -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
CC_INC=
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libpgo.a
AR_OPT=""

LIST_pgo=InstrProfiling.o InstrProfilingValue.o InstrProfilingBuffer.o\
	InstrProfilingFile.o InstrProfilingMerge.o InstrProfilingMergeFile.o\
	InstrProfilingNameVar.o InstrProfilingWriter.o\
	InstrProfilingPlatformLinux.o InstrProfilingPlatformOther.o\
	InstrProfilingRuntime.o InstrProfilingUtil.o

LIST_ALL=$(LIST_pgo)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_pgo)

pgo: $(LIST_pgo)

InstrProfiling.o: $(MWLIB)/pgo/pgo_g/InstrProfiling.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 -fdata-sections $(MWLIB)/pgo/pgo_g/InstrProfiling.c

InstrProfilingValue.o: $(MWLIB)/pgo/pgo_g/InstrProfilingValue.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingValue.c

InstrProfilingBuffer.o: $(MWLIB)/pgo/pgo_g/InstrProfilingBuffer.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingBuffer.c

InstrProfilingFile.o: $(MWLIB)/pgo/pgo_g/InstrProfilingFile.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingFile.c

InstrProfilingMerge.o: $(MWLIB)/pgo/pgo_g/InstrProfilingMerge.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingMerge.c

InstrProfilingMergeFile.o: $(MWLIB)/pgo/pgo_g/InstrProfilingMergeFile.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingMergeFile.c

InstrProfilingNameVar.o: $(MWLIB)/pgo/pgo_g/InstrProfilingNameVar.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingNameVar.c

InstrProfilingWriter.o: $(MWLIB)/pgo/pgo_g/InstrProfilingWriter.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingWriter.c

InstrProfilingPlatformLinux.o: $(MWLIB)/pgo/pgo_g/InstrProfilingPlatformLinux.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingPlatformLinux.c

InstrProfilingPlatformOther.o: $(MWLIB)/pgo/pgo_g/InstrProfilingPlatformOther.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingPlatformOther.c

InstrProfilingRuntime.o: $(MWLIB)/pgo/pgo_g/InstrProfilingRuntime.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingRuntime.cc

InstrProfilingUtil.o: $(MWLIB)/pgo/pgo_g/InstrProfilingUtil.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/pgo/pgo_g $(CC_INC) -c $(OPT_LEV) -Hsdata0 $(MWLIB)/pgo/pgo_g/InstrProfilingUtil.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
