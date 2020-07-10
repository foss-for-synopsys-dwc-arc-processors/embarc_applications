# libmwclc arc.av2hs.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Hsdata0 -Hnoxcheck -av2hs -Os -Hreg_reserve=30
CC_INC= -I$(MWLIB)/libc_l/inc/internal -I$(MWLIB)/libc_l/inc
cflags=

AS=$(CCAC)
AS_OPT= -c -Hnoxcheck -av2hs
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libmwclc.a
AR_OPT=""

LIST_fio=clprintf.o

LIST_startup=vecstack.o

LIST_stu=stulib.o

LIST_ALL=$(LIST_fio) $(LIST_startup) $(LIST_stu)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_fio)
	$(AR) rv $(AR_NAME) $(LIST_startup)
	$(AR) rv $(AR_NAME) $(LIST_stu)

fio: $(LIST_fio)
startup: $(LIST_startup)
stu: $(LIST_stu)

clprintf.o: $(MWLIB)/mwclc/fio_g/clprintf.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwclc/fio_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwclc/fio_g/clprintf.c

vecstack.o: $(MWLIB)/mwclc/startup_g/vecstack.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwclc/startup_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwclc/startup_g/vecstack.c

stulib.o: $(MWLIB)/mwclc/stu_g/stulib.cc
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mwclc/stu_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mwclc/stu_g/stulib.cc

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
