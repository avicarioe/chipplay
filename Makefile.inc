# vim: syntax=make
# Makefile.inc version 1.6_pic
# 28/01/2020
# Alejandro Vicario Espinosa

# Project src files
PROJ_SRC = $(wildcard $(SRC_DIR)/*.c)
PROJ_SRC += $(wildcard $(SRC_DIR)/*.S)
SRC_FILES +=$(PROJ_SRC)
INC_FOLDERS += $(INC_DIR)
OBJ_DIR := $(OUTPUT_DIRECTORY)/obj
MAKEFILE_PATHS = $(wildcard Makefile.paths)
VERSION_FILE = $(INC_DIR)/version.h
VERSION_IN = $(INC_DIR)/version.h.in

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LDFLAGS += -lc -lm $(LIBS)
LDFLAGS += -T$(LINKER_SCRIPT)

CFLAGS += -D__PRJ_NAME__=$(PROJECT_NAME)
CFLAGS += -D__DEVICE__=$(DEVICE)

OUTPUT_FILE=$(OUTPUT_DIRECTORY)/$(PROJECT_NAME)_$(DEVICE)

# echo suspend
ifeq ($(VERBOSE),1)
  NO_ECHO :=
else
  NO_ECHO := @
endif

#Default target
.PHONY: default
default: .clang_complete .vimlocal \
	$(OUTPUT_FILE).hex $(OUTPUT_FILE).elf
	$(NO_ECHO)which figlet >/dev/null 2>&1 && figlet DONE: $(PROJECT_NAME) || true
	$(info DONE: $(PROJECT_NAME))

# Functions

# $1 type of item
# $2 items paths to check
define ensure_exists_each
$(foreach item, $(2), \
  $(if $(wildcard $(item)),, $(warning Cannot find $(1): $(item))))
endef

# Objects
$(call ensure_exists_each,source file, $(SRC_FILES))
OBJ_FILES := $(foreach src_file, $(SRC_FILES), \
	$(eval obj_file := $(OBJ_DIR)/$(notdir $(src_file).o)) \
	$(eval $(obj_file)_SRC := $(src_file)) \
	$(eval $(obj_file): \
		Makefile \
		Makefile.inc \
		$(MAKEFILE_PATHS) \
		$(VERSION_FILE) |  $(OBJ_DIR)) \
	$(obj_file))

# Includes
$(call ensure_exists_each,include folder,$(INC_FOLDERS))
INC_PATHS := $(foreach folder,$(INC_FOLDERS),-I$(folder))
LIB_PATHS := $(foreach folder,$(LIB_FOLDERS),-L$(folder))

# Commands
MK := $(NO_ECHO)mkdir
RM := $(NO_ECHO)rm -rf
CTAGS    := $(NO_ECHO)ctags

CC      := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)gcc
CXX     := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)c++
AS      := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)as
AR      := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)ar -r
LD      := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)ld
NM      := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)nm
OBJDUMP := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)objdump
OBJCOPY := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)objcopy
SIZE    := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)size
AVRDUDE := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)avrdude
BIN2HEX := $(NO_ECHO)$(GNU_INSTALL_ROOT)$(GNU_PREFIX)bin2hex

# Targets
.PHONY: clean
clean:
	$(info Removing $(OUTPUT_DIRECTORY))
	$(RM) $(OUTPUT_DIRECTORY)
	$(RM) .clang_complete
	$(RM) .vimlocal
	$(RM) tags
	$(RM) $(SRC_DIR)/*.orig $(INC_DIR)/*.orig
	$(RM) $(VERSION_FILE)

# Create build directories
$(OUTPUT_DIRECTORY):
	$(MK) $@
$(OBJ_DIR) : | $(OUTPUT_DIRECTORY)
	$(MK) $@


# Include object prerequisites files
include $(wildcard $(OBJ_DIR)/*.d)


# Create object files from C source files
%.c.o:
	$(info [C]Compiling file: $(notdir $($@_SRC)))
	$(CC) -MP -MD -c -o $@ $($@_SRC) $(CFLAGS) $(INC_PATHS)

# Create object files from C++ source files
%.cpp.o:
	$(info [CXX]Compiling file: $(notdir $($@_SRC)))
	$(CXX) -MP -MD -c -o $@ $($@_SRC) $(CFLAGS) $(CXXFLAGS) $(INC_PATHS)
# Create object files from assembly source files
%.S.o %.s.o.o:
	$(info [ASS]Assembling file: $(notdir $($@_SRC)))
	$(CC) -MP -MD -c -o $@ $($@_SRC) $(ASMFLAGS) $(INC_PATHS)

$(OUTPUT_FILE).elf: $(OBJ_FILES) $(LIB_FILES)
%.elf: | $(OUTPUT_DIRECTORY)
	$(info Linking target: $@ $(LINKER_SCRIPT))
	$(CC) $(LIB_PATHS) $(OBJ_FILES) $(LIB_FILES) $(LDFLAGS) -Wl,-Map=$(@:.elf=.map) -o $@
	$(SIZE) $@

# Create binary .hex file from the .elf file
%.hex: %.elf | $(OUTPUT_DIRECTORY)
	$(info Preparing: $@)
	$(BIN2HEX) -a $<

CLANG := $(foreach inc, $(INC_PATHS), $(inc)\n)

$(VERSION_FILE): $(VERSION_IN) Makefile \
	Makefile.inc \
	$(MAKEFILE_PATHS)
	$(info Generating: $(notdir $@))
	@sed -e 's/[@]VMAJOR[@]/$(VERSION_MAJOR)/'    \
		-e 's/[@]VMINOR[@]/$(VERSION_MINOR)/'     \
		-e 's/[@]VFIX[@]/$(VERSION_FIX)/'         \
		-e 's/[@]CID[@]/$(COMPANY_ID)/'           \
		-e 's/[@]PID[@]/$(PRODUCT_ID)/' $< > $@

# Generating clang complete file
.clang_complete: Makefile \
	Makefile.inc \
	$(MAKEFILE_PATHS) \
	$(VERSION_FILE)
	$(info Generating: $@)
	$(file >$@)
	$(foreach inc,$(INC_PATHS),$(file >> $@,$(inc)))
	$(foreach def,$(filter -D%, $(CFLAGS)),$(file >> $@,$(def)))
	$(file >> $@,-U__unix)
	$(file >> $@,-arch=mips32r2)
	$(file >> $@,-D__LANGUAGE_C__)
	$(file >> $@,--target=mips)
	$(file >> $@,-I/opt/mcb32tools/lib/gcc/mipsel-mcb32-elf/4.9.2/include/)


.vimlocal: Makefile \
	Makefile.inc \
	$(MAKEFILE_PATHS) \
	$(VERSION_FILE)
	$(info Generating: $@)
	$(file >$@)
	$(foreach inc,$(INC_FOLDERS),$(file >> $@,set path+=$(inc)))
.PHONY: flash flash_softdevice erase flash_all indent

tags: Makefile \
	Makefile.inc \
	$(MAKEFILE_PATHS)
	$(info Generating: $@)
	$(CTAGS) -R --exclude=proc $(INC_FOLDERS) $(SRC_FILES)

indent:
	$(NO_ECHO)astyle --project=.astylerc -v $(PROJ_SRC) $(INC_DIR)/*.h

PHONY: install

# Flash the program
install: $(OUTPUT_FILE).hex
	$(info Flashing: $<)
	$(AVRDUDE) -v -p $(shell echo "$(DEVICE)" | tr '[:lower:]' '[:upper:]') -c stk500v2 -P "$(TTYDEV)" -b $(TTYBAUD) -U "flash:w:$<"
