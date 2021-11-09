#################################################################################
APP_NAME = flash_programmer

BIN    = $(APP_NAME).elf
OBJS   = tensorflow/lite/micro/tools/make/targets/arc/iotdk/flash_programmer/main.o
MAP    = $(APP_NAME).map
IOTDK_TCF  ?= ../iotdk.tcf
IOTDK_LCF  ?= ../iotdk.lcf

CC = ccac
LD = ccac
RM = del /f


CFLAGS = -g -Hnocopyr -tcf=$(IOTDK_TCF) -Hheap=0 -Hstack=64
LDFLAGS = -Hnocopyr -tcf=$(IOTDK_TCF) -Hldopt=-Coutput=$(MAP) -Hldopt=-Csections -Hldopt=-Csize -Hldopt=-Cunmangle $(IOTDK_LCF)
#################################################################################

all: $(BIN)

$(BIN): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(BIN) $(OBJS) $(BOOT_OBJS) $(DUMP) $(MAP)