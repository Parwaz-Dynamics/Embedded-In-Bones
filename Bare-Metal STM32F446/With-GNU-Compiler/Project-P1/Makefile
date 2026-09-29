# ===== Simple Makefile for Project-P1 =====
# Output goes into the Debug/ folder.

# ---- Toolchain ----
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ---- Folders ----
BUILD_DIR = Debug
SRC_DIR   = Core/Src
STARTUP_DIR = Core/Startup
INC_DIR   = Core/Inc

# ---- Output name ----
TARGET = blink

# ---- Linker script (place it in the project root) ----
LDSCRIPT = stm32f446.ld

# ---- CPU flags ----
CPU = -mcpu=cortex-m4 -mthumb

# ---- Compiler flags ----
CFLAGS  = $(CPU) -g -O0 -Wall -I$(INC_DIR)

# ---- Linker flags ----
LDFLAGS = $(CPU) -T$(LDSCRIPT) -nostartfiles --specs=nano.specs

# ---- Object files (built inside Debug/) ----
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/startup.o

# ===== Rules =====

# Default: build everything
all: $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

# Make the Debug folder if it doesn't exist
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Compile main.c -> Debug/main.o
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble startup -> Debug/startup.o
$(BUILD_DIR)/startup.o: $(STARTUP_DIR)/startup_stm32f446xx.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link both .o -> Debug/blink.elf
$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@
	$(SIZE) $@

# ELF -> BIN
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# ELF -> HEX
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Delete the whole Debug folder
clean:
	rmdir /S /Q $(BUILD_DIR)

.PHONY: all clean
