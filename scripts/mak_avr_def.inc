
#####################################
# ArduinoLite Building System
# By Shikai Chen (csk@live.com)
# Modified from Arduino's Makefile
#
# Dependency:
# HOME_TREE , PRJ_NAME, BINARY_NAME, WINAVR_HOME
BUILD_ROOT  := build
SOURCE_ROOT := $(HOME_TREE)/src
TOOL_ROOT   := $(HOME_TREE)/bin
OUTPUT_ROOT := $(BUILD_ROOT)
OBJ_ROOT    := $(BUILD_ROOT)/obj
LIB_ROOT    := $(OBJ_ROOT)/lib

ifndef WINAVR_HOME
WINAVR_HOME := $(TOOL_ROOT)/win32/WinAVR-20090313
endif

REMOVE      := $(TOOL_ROOT)/win32/posix/rm -f
MV          := $(TOOL_ROOT)/win32/posix/mv -f
MKDIR       := $(TOOL_ROOT)/win32/posix/mkdir -p
SED         := $(TOOL_ROOT)/win32/posix/sed
FIND        := $(TOOL_ROOT)/win32/posix/find
DIRNAME     := $(TOOL_ROOT)/win32/posix/dirname
#we use the modified avrdude version provided by Arduino
AVRDUDE     := $(TOOL_ROOT)/win32/avrdude-arduino.exe

#HID based usb bootloader download app for win32
HIDDOWNLOADER := $(TOOL_ROOT)/win32/bootloadHID.exe

AVR_RUNTIME_LIB  := core.a
AVR_RUNTIME_PATH := $(SOURCE_ROOT)/core
AVR_OBJ_ROOT     := $(OBJ_ROOT)
AVR_OUTPUT_ROOT  := $(OUTPUT_ROOT)
TARGET           := $(AVR_OUTPUT_ROOT)/$(BINARY_NAME)
TARGET_OBJ_ROOT  := $(AVR_OBJ_ROOT)

#################################################################
#The follow are default config, please overide them if necessary.
UPLOAD_PORT := COM3
PORT_RATE := 19200

MCU := atmega168
F_CPU := 16000000L

AVRDUDE_PROGRAMMER := stk500v1
BURNING_PROGRAMMER := stk500v2
######################################
AVR_TOOLS_PATH = $(WINAVR_HOME)/bin

FORMAT := ihex

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU) -D_AVR_ $(EXTRA_DEFS)
CXXDEFS = -DF_CPU=$(F_CPU) -D_AVR_ $(EXTRA_DEFS)


# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)


CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEFS) $(CINCS) -O$(OPT)
ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs $(CDEFS) $(CINCS)
LDFLAGS = -lm -Wl,--gc-sections -Os

# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -D -U flash:w:$(TARGET).hex
AVRDUDE_FLAGS = -V \
-p $(MCU) -P $(UPLOAD_PORT) -c $(AVRDUDE_PROGRAMMER) \
-b $(PORT_RATE)

BURNING_EARSE_CHIP = -e
BURNING_WRITE_FUSE = 
BURNING_WRITE_LOCKFUSE = 
BURNING_WRITE_FLASH = -U flash:w:$(TARGET).hex
BURNING_FLAGS = -F -p $(MCU) -P $(BURNING_PORT) -c $(BURNING_PROGRAMMER)
BURNING_PORT = avrdoper

BOOTLOADER_SERIAL_M8_8M = $(SOURCE_ROOT)/bootloaders/atmega8/ATmegaBOOT@8.hex
BOOTLOADER_SERIAL_M8_16M = $(SOURCE_ROOT)/bootloaders/atmega8/ATmegaBOOT@16.hex
BOOTLOADER_SERIAL_M88_8M = $(SOURCE_ROOT)/bootloaders/atmegax8/ATmegaBOOT_88_pro_8MHz.hex
BOOTLOADER_SERIAL_M88_12M = $(SOURCE_ROOT)/bootloaders/atmegax8/ATmegaBOOT_88_pro_12MHz.hex
BOOTLOADER_SERIAL_M168_8M = $(SOURCE_ROOT)/bootloaders/atmegax8/ATmegaBOOT_168_pro_8MHz.hex
BOOTLOADER_SERIAL_M168_16M = $(SOURCE_ROOT)/bootloaders/atmegax8/ATmegaBOOT_168_diecimila.hex

BOOTLOADER_USB_M8_12M = $(SOURCE_ROOT)/bootloaders/hidbootloader/pre-compiled/mega8_12M.hex
BOOTLOADER_USB_M88_12M = $(SOURCE_ROOT)/bootloaders/hidbootloader/pre-compiled/mega88_12M.hex
BOOTLOADER_USB_M168_12M = $(SOURCE_ROOT)/bootloaders/hidbootloader/pre-compiled/mega168_12M.hex
########################################
#Override the default compiler setting

CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm

# Combine all necessary flags and optional flags.
# Add target processor to flags.

INCLUDE_PATH += -I.

ALL_CFLAGS = -ffunction-sections -fdata-sections -mmcu=$(MCU) $(INCLUDE_PATH) $(CFLAGS)
ALL_CXXFLAGS = -fno-exceptions -ffunction-sections -fdata-sections -mmcu=$(MCU) $(INCLUDE_PATH) $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) $(INCLUDE_PATH) -x assembler-with-cpp $(ASFLAGS)

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE)  $(TARGET).elf

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000 
