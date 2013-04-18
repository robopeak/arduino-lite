#
#   ArduinoLite Building System
#   By Shikai Chen (csk@live.com)
#
#

.SUFFIXES:
.SUFFIXES: .o .cpp .s

.PHONY: all make_lib clean addition additionclean make_subs

make_subs:
	@for subdir in $(MAKE_TARGETS) ; do \
	  $(MAKE) -C $$subdir $(MAKECMDGOALS) ; \
	  done

# Detect any Eclipse set configs
ifdef AVRARGETCPU
MCU = $(AVRARGETCPU)
endif 

ifdef AVRARGETFCPU
F_CPU = $(AVRARGETFCPU)
endif 



# Define all object files.

_CSRC   = $(filter-out %*.c,$(CSRC))
_CXXSRC = $(filter-out %*.cpp,$(CXXSRC))
_ASRC   = $(filter-out %*.S,$(ASRC))


OBJ += $(patsubst %.c, $(TARGET_OBJ_ROOT)/%.o, $(_CSRC))
OBJ += $(patsubst %.cpp, $(TARGET_OBJ_ROOT)/%.o, $(_CXXSRC))
OBJ += $(patsubst %.S, $(TARGET_OBJ_ROOT)/%.o, $(_ASRC))

OBJ_DUMP = $(patsubst %.c, $(TARGET_OBJ_ROOT)/%.txt, $(_CSRC))
OBJ_DUMP += $(patsubst %.cpp, $(TARGET_OBJ_ROOT)/%.txt, $(_CXXSRC))
OBJ_DUMP += $(patsubst %.S, $(TARGET_OBJ_ROOT)/%.txt, $(_ASRC))

BIN_DUMP = $(TARGET).txt

# Define all listing files.

LST += $(patsubst %.c, $(TARGET_OBJ_ROOT)/%.lst, $(_CSRC))
LST += $(patsubst %.cpp, $(TARGET_OBJ_ROOT)/%.lst, $(_CXXSRC))
LST += $(patsubst %.S, $(TARGET_OBJ_ROOT)/%.lst, $(_ASRC))

DEP_FILE += $(patsubst %.c, $(TARGET_OBJ_ROOT)/%.d, $(_CSRC))
DEP_FILE += $(patsubst %.cpp, $(TARGET_OBJ_ROOT)/%.d, $(_CXXSRC))

build: elf hex 

clean: additionalclean
	$(REMOVE) -r $(AVR_OUTPUT_ROOT)/*

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Program the device. 

# Burning command

# BUILD FUSE updating commanding based on the actual configuration
ifdef EFUSE
BURNING_WRITE_FUSE += -U efuse:w:0x$(EFUSE):m
endif

ifdef HFUSE
BURNING_WRITE_FUSE += -U hfuse:w:0x$(HFUSE):m
endif

ifdef LFUSE
BURNING_WRITE_FUSE += -U lfuse:w:0x$(LFUSE):m
endif

ifdef LOCKFUSE
BURNING_WRITE_LOCKFUSE = -U lock:w:0x$(LOCKFUSE):m
endif


# erase the chip and burning the hex file only
burn: $(TARGET).hex
	$(AVRDUDE) $(BURNING_FLAGS) $(BURNING_WRITE_FLASH)

fuse:
	$(AVRDUDE) $(BURNING_FLAGS) $(BURNING_WRITE_FUSE)

lock:
	$(AVRDUDE) $(BURNING_FLAGS) $(BURNING_WRITE_LOCKFUSE)

erase:
	$(AVRDUDE) $(BURNING_FLAGS) $(BURNING_EARSE_CHIP)

# Bootloader command
upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

usbupload: $(TARGET).hex
	$(HIDDOWNLOADER) $(TARGET).hex

# DEBUGING SUPPORT

# disassembly all the output file (*.o)
dumpobj: $(OBJ_DUMP)
 
# disassembly the elf file
dump: $(BIN_DUMP)

sizebefore:
	@if [ -f $(TARGET).elf ]; then echo; $(ELFSIZE); echo; fi

sizeafter:
	@if [ -f $(TARGET).elf ]; then echo; $(HEXSIZE); echo; fi

coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(TARGET).elf $(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).cof


.SUFFIXES: .elf .hex .eep .lss .sym

ifneq ($(MAKECMDGOALS), clean)
sinclude $(DEP_FILE)
endif

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--no-change-warnings --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

$(TARGET).elf: $(OBJ) $(EXTRA_OBJ)
	$(MKDIR) `$(DIRNAME) $@`
	$(CC) $(ALL_CFLAGS) --cref -o $@ -L. $^ $(LDFLAGS) 

# Compile: create object files from C++ source files.
$(TARGET_OBJ_ROOT)/%.o: %.cpp
	$(MKDIR) `$(DIRNAME) $@`
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

# Compile: create object files from C source files.
$(TARGET_OBJ_ROOT)/%.o: %.c
	$(MKDIR) `$(DIRNAME) $@`
	$(CC) -c $(ALL_CFLAGS) $< -o $@ 


# Compile: create assembler files from C source files.
$(TARGET_OBJ_ROOT)/%.s: %.c
	$(MKDIR) `$(DIRNAME) $@`
	$(CC) -S $(ALL_CFLAGS) $< -o $@


# Assemble: create object files from assembler source files.
$(TARGET_OBJ_ROOT)/%.o: %.S
	$(MKDIR) `$(DIRNAME) $@`
	$(CC) -c $(ALL_ASFLAGS) $< -o $@


# Automatic dependencies
$(TARGET_OBJ_ROOT)/%.d: %.c
	$(MKDIR) `$(DIRNAME) $@`
	($(CC) -M $(ALL_CFLAGS) $< | $(SED) "s;$(notdir $*).o:;$(TARGET_OBJ_ROOT)/$*.o $(TARGET_OBJ_ROOT)/$*.d:;" ) > $@

$(TARGET_OBJ_ROOT)/%.d: %.cpp
	$(MKDIR) `$(DIRNAME) $@`
	($(CXX) -M $(ALL_CXXFLAGS) $< | $(SED) "s;$(notdir $*).o:;$(TARGET_OBJ_ROOT)/$*.o $(TARGET_OBJ_ROOT)/$*.d:;" ) > $@

# Debugibility Support
$(TARGET_OBJ_ROOT)/%.txt: $(TARGET_OBJ_ROOT)/%.o
	$(OBJDUMP) -d $< > $@

$(TARGET).txt: $(TARGET).elf lss
	$(OBJDUMP) -d $< > $@

.PHONY:	all build elf hex eep lss sym program coff extcoff clean applet_files sizebefore sizeafter dump dumpobj



