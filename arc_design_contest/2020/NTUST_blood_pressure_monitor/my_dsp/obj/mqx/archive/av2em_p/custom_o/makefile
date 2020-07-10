# libmqx arc.av2em.custom
MWLIB=../../../..
# Edit the following line to reference your RTOS installation:
RTOS_INC=-I$(MWLIB)/mqx/include

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Heos=mqx -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
CC_INC=$(RTOS_INC)  -I$(MWLIB)/c/inc/internal -I$(MWLIB)/c/inc
cflags=

AS=$(CCAC)
AS_OPT=-c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libmqx.a
AR_OPT=""

LIST_mqx=mqxerrno.o mqxiface.o mqxtls.o startk.o

LIST_ALL=$(LIST_mqx)

all:   $(AR_NAME)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_mqx)

mqx: $(LIST_mqx)

mqxerrno.o: $(MWLIB)/mqx/mqx_g/mqxerrno.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mqx/mqx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mqx/mqx_g/mqxerrno.c

mqxiface.o: $(MWLIB)/mqx/mqx_g/mqxiface.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mqx/mqx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mqx/mqx_g/mqxiface.c

mqxtls.o: $(MWLIB)/mqx/mqx_g/mqxtls.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mqx/mqx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mqx/mqx_g/mqxtls.c

startk.o: $(MWLIB)/mqx/mqx_g/startk.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/mqx/mqx_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/mqx/mqx_g/startk.c

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a
