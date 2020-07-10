# libaom arc.av2em.custom
MWLIB=../../../..

CC=ccac

CCAC=ccac
OPT_LEV=
CC_OPT=-Hnocopyr -Haom -Hsdata0 -Hnoxcheck -av2em -Os -Hreg_reserve=30
CC_INC= -I$(MWLIB)/aom/inc -I$(MWLIB)/aom/aom_g -I$(MWLIB)/c/inc
cflags=

AS=$(CCAC)
AS_OPT=-Haom -c -Hnoxcheck -av2em 
AS_INC=
aflags=

AR=$(dir $(CC))arac
ifeq ($(AR),./arac)
    AR=arac
endif

AR_NAME=libaom.a
AR_OPT=""

LIST_aom=aom.o fixup_overlay_package.o aom_handlers.o aom_overlay_end.o

LIST_extra_objs=aom_tiny.o

LIST_ALL=$(LIST_aom)

LIST_DEPEND=aom.o fixup_overlay_package.o aom_tiny.o aom_handlers.o\
	aom_overlay_end.o

.PHONY: depend

all: depend extra_objs $(AR_NAME)

depend: makedepend

makedepend:
	-$(RM) $(LIST_DEPEND)
	$(MAKE) $(MAKEPARMS) --no-print-directory --silent cflags="-Hmake -Hrel" aflags="-Hmake -Hrel" $(LIST_DEPEND) > makedepend
	-$(RM) $(LIST_DEPEND)

$(AR_NAME): $(LIST_ALL)
	$(AR) rv $(AR_NAME) $(LIST_aom)

aom: $(LIST_aom)
extra_objs: $(LIST_extra_objs)

aom.o: $(MWLIB)/aom/aom_g/aom.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/aom/aom_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/aom/aom_g/aom.c

fixup_overlay_package.o: $(MWLIB)/aom/aom_g/fixup_overlay_package.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/aom/aom_g $(CC_INC) -c $(OPT_LEV) $(MWLIB)/aom/aom_g/fixup_overlay_package.c

aom_tiny.o: $(MWLIB)/aom/aom_g/aom_tiny.c
	$(CCAC) $(CC_OPT) $(cflags) -I. -I$(MWLIB)/aom/aom_g $(CC_INC) -c $(OPT_LEV) -DALLOW_DATA_OVERLAYS=0 -DFIXED_SIZE_OVERLAYS=1 -DOVERLAY_ENTRIES=16 $(MWLIB)/aom/aom_g/aom_tiny.c

aom_handlers.o: $(MWLIB)/aom/aom_g/aom_handlers.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/aom/aom_g/aom_handlers.s

aom_overlay_end.o: $(MWLIB)/aom/aom_g/aom_overlay_end.s
	$(AS) $(AS_OPT) $(aflags) $(AS_INC) -o $@ $(MWLIB)/aom/aom_g/aom_overlay_end.s

.DEFAULT:
	@echo "invalid target specified: " $@

RM ?= rm

clean:
	-$(RM) *.o
	-$(RM) *.a


-include makedepend

