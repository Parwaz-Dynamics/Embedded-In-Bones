# ===== Makefile for STM32F446RE (Project-P1) =====

# ---- Toolchain ----
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

# ---- Folders ----
BUILD_DIR   = Debug
SRC_DIR     = Core/Src
STARTUP_DIR = Core/Startup
INC_DIR     = Core/Inc

# ---- Output name ----
TARGET = blink

# ---- Linker script ----
LDSCRIPT = STM32F446RETX_FLASH.ld

# ---- Flags ----
CPU     = -mcpu=cortex-m4 -mthumb
CFLAGS  = $(CPU) -g -O0 -Wall -I$(INC_DIR) -MMD -MP
LDFLAGS = $(CPU) -T$(LDSCRIPT) -nostartfiles \
          -Wl,-Map=$(BUILD_DIR)/$(TARGET).map -Wl,--gc-sections

# ---- Objects (auto-discovered) ----
SRCS = $(wildcard $(SRC_DIR)/*.c)
ASMS = $(wildcard $(STARTUP_DIR)/*.s)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/Src/%.o,$(SRCS)) \
       $(patsubst $(STARTUP_DIR)/%.s,$(BUILD_DIR)/Startup/%.o,$(ASMS))

# ---- Shell-portable helpers ----
ifeq ($(OS),Windows_NT)
	SHELL := cmd.exe
  MKDIR = if not exist "$(subst /,\,$@)" mkdir "$(subst /,\,$@)"
  RM    = if exist "$(subst /,\,$(BUILD_DIR))" del /Q /S "$(subst /,\,$(BUILD_DIR))\*.*" >nul 2>&1
else
  MKDIR = mkdir -p $@
  RM    = rm -f $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.hex $(BUILD_DIR)/*.map \
                $(BUILD_DIR)/Src/* $(BUILD_DIR)/Startup/*
endif

# ===== Rules =====
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex

# Output directories
$(BUILD_DIR)/Src $(BUILD_DIR)/Startup:
	$(MKDIR)

# Core/Src/*.c -> Debug/Src/*.o
$(BUILD_DIR)/Src/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/Src
	$(CC) $(CFLAGS) -c $< -o $@

# Core/Startup/*.s -> Debug/Startup/*.o
$(BUILD_DIR)/Startup/%.o: $(STARTUP_DIR)/%.s | $(BUILD_DIR)/Startup
	$(CC) $(CFLAGS) -c $< -o $@

# Link -> .elf
$(BUILD_DIR)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

# .elf -> .bin / .hex
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O ihex $< $@

# Removes build outputs but keeps the Debug/Src and Debug/Startup folders
clean:
	$(RM)

flash: $(BUILD_DIR)/$(TARGET).bin
	STM32_Programmer_CLI -c port=SWD -w $< 0x08000000 -v -rst

size: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex
ifeq ($(OS),Windows_NT)
	powershell -NoProfile -Command "Get-Item '$(BUILD_DIR)\$(TARGET).elf','$(BUILD_DIR)\$(TARGET).bin','$(BUILD_DIR)\$(TARGET).hex' | Select-Object Name, Length"
else
	wc -c $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin $(BUILD_DIR)/$(TARGET).hex
endif

# Header dependency tracking
-include $(OBJS:.o=.d)

.PHONY: all clean flash