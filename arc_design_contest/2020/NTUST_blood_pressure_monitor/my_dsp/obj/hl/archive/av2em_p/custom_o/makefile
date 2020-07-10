# libhl arc.av2em.custom
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

AR_NAME=libhlt.a
AR_OPT=""

LIST_hl=_exit.o _hl_access.o _hl_close.o _hl_getcwd.o _hl_isatty.o _hl_lseek.o\
	_hl_lseek64.o _hl_mkdir.o _hl_open.o _hl_read.o _hl_rename.o\
	_hl_write.o access.o argv.o clock.o close.o environ.o getcwd.o getenv.o\
	getpid.o hl_bios.o hl_data.o hl_lock.o hostcom.o isatty.o lseek.o\
	lseek64.o mkdir.o open.o putenv.o read.o remove.o rename.o sbrk.o\
	time.o tmpnam.o unlink.o write.o

LIST_extra_objs=hl_buf.o

LIST_ALL=$(LIST_hl)

all:  extra_objs $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_hl)

hl: $(LIST_hl)
extra_objs: $(LIST_extra_objs)

_exit.o: $(MWLIB)/hl/hl_g/_exit.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_exit.c

_hl_access.o: $(MWLIB)/hl/hl_g/_hl_access.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_access.c

_hl_close.o: $(MWLIB)/hl/hl_g/_hl_close.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_close.c

_hl_getcwd.o: $(MWLIB)/hl/hl_g/_hl_getcwd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_getcwd.c

_hl_isatty.o: $(MWLIB)/hl/hl_g/_hl_isatty.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_isatty.c

_hl_lseek.o: $(MWLIB)/hl/hl_g/_hl_lseek.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_lseek.c

_hl_lseek64.o: $(MWLIB)/hl/hl_g/_hl_lseek64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_lseek64.c

_hl_mkdir.o: $(MWLIB)/hl/hl_g/_hl_mkdir.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_mkdir.c

_hl_open.o: $(MWLIB)/hl/hl_g/_hl_open.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_open.c

_hl_read.o: $(MWLIB)/hl/hl_g/_hl_read.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_read.c

_hl_rename.o: $(MWLIB)/hl/hl_g/_hl_rename.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_rename.c

_hl_write.o: $(MWLIB)/hl/hl_g/_hl_write.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/_hl_write.c

access.o: $(MWLIB)/hl/hl_g/access.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/access.c

argv.o: $(MWLIB)/hl/hl_g/argv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/argv.c

clock.o: $(MWLIB)/hl/hl_g/clock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/clock.c

close.o: $(MWLIB)/hl/hl_g/close.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/close.c

environ.o: $(MWLIB)/hl/hl_g/environ.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/environ.c

getcwd.o: $(MWLIB)/hl/hl_g/getcwd.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/getcwd.c

getenv.o: $(MWLIB)/hl/hl_g/getenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/getenv.c

getpid.o: $(MWLIB)/hl/hl_g/getpid.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/getpid.c

hl_bios.o: $(MWLIB)/hl/hl_g/hl_bios.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/hl_bios.c

hl_buf.o: $(MWLIB)/hl/hl_g/hl_buf.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/hl/hl_g/hl_buf.s

hl_data.o: $(MWLIB)/hl/hl_g/hl_data.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/hl_data.c

hl_lock.o: $(MWLIB)/hl/hl_g/hl_lock.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/hl_lock.c

hostcom.o: $(MWLIB)/hl/hl_g/hostcom.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/hostcom.c

isatty.o: $(MWLIB)/hl/hl_g/isatty.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/isatty.c

lseek.o: $(MWLIB)/hl/hl_g/lseek.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/lseek.c

lseek64.o: $(MWLIB)/hl/hl_g/lseek64.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/lseek64.c

mkdir.o: $(MWLIB)/hl/hl_g/mkdir.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/mkdir.c

open.o: $(MWLIB)/hl/hl_g/open.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/open.c

putenv.o: $(MWLIB)/hl/hl_g/putenv.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/putenv.c

read.o: $(MWLIB)/hl/hl_g/read.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/read.c

remove.o: $(MWLIB)/hl/hl_g/remove.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/remove.c

rename.o: $(MWLIB)/hl/hl_g/rename.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/rename.c

sbrk.o: $(MWLIB)/hl/hl_g/sbrk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/sbrk.c

time.o: $(MWLIB)/hl/hl_g/time.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/time.c

tmpnam.o: $(MWLIB)/hl/hl_g/tmpnam.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/tmpnam.c

unlink.o: $(MWLIB)/hl/hl_g/unlink.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/unlink.c

write.o: $(MWLIB)/hl/hl_g/write.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/hl/hl_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/hl/hl_g/write.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
