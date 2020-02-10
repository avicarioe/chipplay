# vim: syntax=make
# Configuration
PROJECT_NAME := chipplay
OUTPUT_DIRECTORY := build

GNU_INSTALL_ROOT = $(TOOLCHAIN_INSTALL_DIR)/bin/
GNU_PREFIX = $(TARGET)

VERSION_MAJOR := 0
VERSION_MINOR := 0
VERSION_FIX   := 0
PRODUCT_ID    := 0
COMPANY_ID    := 0

-include Makefile.paths
DEVICE ?= 32MX340F512H

TTYDEV ?=/dev/ttyUSB0
TTYBAUD ?=115200

LINKER_SCRIPT ?= p$(shell echo "$(DEVICE)" | tr '[:upper:]' '[:lower:]').ld

SRC_DIR := src
INC_DIR := include

VERBOSE ?= 0

# Flags
CFLAGS += -Os -g
CFLAGS += -Wall -Werror
CFLAGS += -march=mips32r2
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -DPIC32MX3
CFLAGS += -DLOG_ENABLED
CFLAGS += -DLOG_LEVEL_ENABLED=4
CFLAGS += -DDEBUG
CFLAGS += -std=gnu99

# C++ flags common to all targets
CXXFLAGS +=

# Assembler flags common to all targets
ASMFLAGS += $(ASFLAGS)
ASMFLAGS += -march=mips32r2 -Wa,-msoft-float
ASMFLAGS += -Wall -Werror

# Linker flags
LDFLAGS += $(OPT)
# let linker dump unused sections
#LDFLAGS += -Wl,--gc-sections

#External src files
SRC_FILES += \

INC_FOLDERS += \
	${TOOLCHAIN_INSTALL_DIR}/include

# Libraries
LIB_FILES += \

include Makefile.inc
