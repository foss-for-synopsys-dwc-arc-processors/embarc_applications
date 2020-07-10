# libitut arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -DARC_EMU -Hsdata0 -Hnoxcheck -av2em -O -Hon=long_enums
CC_INC= -I$(MWLIB)/itut/include -I$(MWLIB)/c/inc
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libitut.a
AR_OPT=""

LIST_itut=basop32.o

LIST_ALL=$(LIST_itut)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_itut)

itut: $(LIST_itut)

basop32.o: $(MWLIB)/itut/itut_g/basop32.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/itut/itut_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/itut/itut_g/basop32.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
