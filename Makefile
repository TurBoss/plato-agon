TARGET_EXEC ?= plato.bin

BUILD_DIR ?= build
SRC_DIR ?= src
INC_DIR ?= include

CC=zcc
ASM=z80asm

SRCS := $(shell find $(SRC_DIR) -name *.cpp -or -name *.c -or -name *.asm)

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)


CFLAGS=+agon
LDFLAGS=-I$(INC_DIR) -O3 -lndos -lm -create-app
ASFLAGS=-mez80_z80 -I$(INC_DIR)


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@
	
$(BUILD_DIR)/%.asm.o: %.asm
	$(MKDIR_P) $(dir $@)
	$(ASM) $(ASFLAGS) $< -o$@

upload: $(BUILD_DIR)/$(TARGET_EXEC)
	python3 send.py $< /dev/ttyUSB0 115200

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)


MKDIR_P ?= mkdir -p
