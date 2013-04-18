LIB_CSRC     := $(shell find $(AVR_RUNTIME_PATH)/ -name "*.c")
LIB_CXXSRC   := $(shell find $(AVR_RUNTIME_PATH)/ -name "*.cpp")


LIB_AVR_OBJ_PATH := $(TARGET_OBJ_ROOT)/arduinolite

INCLUDE_PATH += -I$(AVR_RUNTIME_PATH)

LIB_OBJ += $(patsubst $(AVR_RUNTIME_PATH)/%.c, $(LIB_AVR_OBJ_PATH)/%.o, $(LIB_CSRC))
LIB_OBJ += $(patsubst $(AVR_RUNTIME_PATH)/%.cpp, $(LIB_AVR_OBJ_PATH)/%.o, $(LIB_CXXSRC))
LIB_OBJ += $(patsubst $(AVR_RUNTIME_PATH)/%.S, $(LIB_AVR_OBJ_PATH)/%.o, $(LIB_ASRC))

EXTRA_OBJ += $(LIB_AVR_OBJ_PATH)/$(AVR_RUNTIME_LIB)

DEP_FILE+= $(patsubst $(AVR_RUNTIME_PATH)/%.c, $(LIB_AVR_OBJ_PATH)/%.d, $(LIB_CSRC))
DEP_FILE+= $(patsubst $(AVR_RUNTIME_PATH)/%.cpp, $(LIB_AVR_OBJ_PATH)/%.d, $(LIB_CXXSRC))


build_avr_lib: $(LIB_AVR_OBJ_PATH)/$(AVR_RUNTIME_LIB)

$(LIB_AVR_OBJ_PATH)/$(AVR_RUNTIME_LIB) : $(LIB_OBJ)
	@for i in $(LIB_OBJ); do echo "$(AR) rcs $(LIB_AVR_OBJ_PATH)/$(AVR_RUNTIME_LIB) $$i"; $(AR) rcs $(LIB_AVR_OBJ_PATH)/$(AVR_RUNTIME_LIB) $$i; done


clean_avr_lib:
	$(REMOVE) -r $(LIB_AVR_OBJ_PATH)

$(LIB_AVR_OBJ_PATH)/%.o: $(AVR_RUNTIME_PATH)/%.c
	$(MKDIR) `$(DIRNAME) $@`
	$(CC) -c $(ALL_CFLAGS) $< -o $@

$(LIB_AVR_OBJ_PATH)/%.o: $(AVR_RUNTIME_PATH)/%.cpp
	$(MKDIR) `$(DIRNAME) $@`
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

$(LIB_AVR_OBJ_PATH)/%.o: $(AVR_RUNTIME_PATH)/%.S
	$(MKDIR) `$(DIRNAME) $@`
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Automatic dependencies
$(LIB_AVR_OBJ_PATH)/%.d: $(AVR_RUNTIME_PATH)/%.cpp
	$(MKDIR) `$(DIRNAME) $@`
	($(CXX) -M $(ALL_CXXFLAGS) $< | $(SED) "s;$(notdir $*).o:;$(LIB_AVR_OBJ_PATH)/$*.o $(LIB_AVR_OBJ_PATH)/$*.d:;") > $@

$(LIB_AVR_OBJ_PATH)/%.d: $(AVR_RUNTIME_PATH)/%.c
	$(MKDIR) `$(DIRNAME) $@`
	($(CC) -M $(ALL_CFLAGS) $< | $(SED) "s;$(notdir $*).o:;$(LIB_AVR_OBJ_PATH)/$*.o $(LIB_AVR_OBJ_PATH)/$*.d:;") > $@


.PHONY: build_avr_lib build_lib_obj make_lib_path clean_avr_lib

