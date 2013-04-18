HOME_TREE := ../..

# TODO: Set the WINAVR_HOME if you do not want to use the build-in winavr
# WINAVR_HOME := /d/WinAVR-20090313

# TODO: set the prj_name here
# default name is your folder's title
PRJ_NAME := $(notdir $(CURDIR))

# TODO: set the output binary name here
# default name is your folder's tilte
BINARY_NAME := $(notdir $(CURDIR))

# enable the following marco in case you want these features
#EXTRA_DEFS = -D_ENABLE_SERIAL_CALLBACK
#EXTRA_DEFS += -D_SERIAL_CUSTOM_RX_ROUTINE_

#includes the common definitions...
#do not remove them unless you know what you are doing
include $(HOME_TREE)/scripts/mak_avr_def.inc
#############################################
# the following are default settings,
# modify them if necessary...
UPLOAD_PORT := COM3
#PORT_RATE := 19200
#MCU := atmega168
#F_CPU := 16000000
#############################################



# TODO: list the C-Source files here
# All of the .c files within this folder will be included by default
CSRC := $(shell find src/ -name "*.c")

# TODO: list the CPP-Source files here
# All of the .cpp files within this folder will be included by default
CXXSRC := $(shell find src/ -name "*.cpp")

# TODO: list the Assmebly-Source files here
# All of the .S files within this folder will be included by default
ASRC := $(shell find src/ -name "*.S")

# TODO: Put the header's path that you want to include
INCLUDE_PATH +=  -I$(SOURCE_ROOT)/core


all:  build_avr_lib build sizebefore sizeafter

#include Arduino-Lite building rules.
include $(HOME_TREE)/scripts/mak_lib.inc

#include standard build logic
include $(HOME_TREE)/scripts/mak_avr_common.inc


additionalclean: clean_avr_lib
