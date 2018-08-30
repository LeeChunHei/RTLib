INC_PATHS=inc src config
SRC_PATH=src
OUT_LIB=rt
OUT_LIB_PATH=lib
OUT_OBJ_PATH=obj

TOOLCHAIN_PREFIX=arm-none-eabi-
CC=$(TOOLCHAIN_PREFIX)gcc --specs=nosys.specs
CXX=$(TOOLCHAIN_PREFIX)g++ --specs=nosys.specs
AR=$(TOOLCHAIN_PREFIX)gcc-ar

# Additional stuff
USER_INC_PATHS?=
USER_SYMBOLS?=

# Basically you are not going to mess up with things below

ifeq ($(OS),Windows_NT)
WIN32=1
$(info Host = Win32)

else
UNIX=1
$(info Host = *nix)

endif

ifneq ($(firstword $(sort $(MAKE_VERSION) 3.81)),3.81)
$(error Require GNU Make 3.81 or newer)

else
$(info Make version = $(MAKE_VERSION))

endif

ifdef WIN32
# TODO Also print CC version on win32

else ifdef UNIX
$(info CC = $(shell $(CC) --version | (read -r line; echo $$line)))

endif

$(info User include paths = $(USER_INC_PATHS))
$(info User symbols = $(USER_SYMBOLS))

.DEFAULT_GOAL:=all

CCFLAGS=
CXXFLAGS=
CPPFLAGS=
ARFLAGS=
BIN_SUFFIX=

CPPFLAGS+=$(addprefix -I,$(INC_PATHS)) $(addprefix -I,$(USER_INC_PATHS))
CPPFLAGS+=$(addprefix -D,$(USER_SYMBOLS))
CPPFLAGS+=-MMD

CCFLAGS+=-fmessage-length=0
CCFLAGS+=-fno-strict-aliasing -ffunction-sections -fdata-sections
CCFLAGS+=-fsigned-char
CCFLAGS+=-Wall -Wextra
# Wmissing-field-initializers is causing too much false warnings
CCFLAGS+=-Wno-missing-field-initializers

ARFLAGS+=-r

ifeq ($(BUILD),DEBUG)
BIN_SUFFIX:=$(BIN_SUFFIX)-d
CPPFLAGS+=-DDEBUG=1
CCFLAGS+=-O0 -g3
$(info Build = DEBUG)

else ifeq ($(BUILD),RELEASE)
BIN_SUFFIX:=$(BIN_SUFFIX)-r
CPPFLAGS+=
# CPPFLAGS+=-DNDEBUG
CCFLAGS+=-O3 -g0
$(info Build = RELEASE)

else
$(warning Unknown build type, defaulting to DEBUG (set BUILD))
BIN_SUFFIX:=$(BIN_SUFFIX)-d
CPPFLAGS+=-DDEBUG=1
CCFLAGS+=-O0 -g3
$(info Build = DEBUG)

endif


include MakeConfig.inc

ifeq ($(MCU),MIMXRT1052)
CPPFLAGS+=-DMIMXRT1052=1
CCFLAGS+=-mthumb -mthumb-interwork -mcpu=cortex-m7 -march=armv7e-m
CCFLAGS+=-mfpu=fpv5-sp-d16 -mfloat-abi=hard
$(info MCU = MIMXRT1052)

else
$(error Missing/Unknown MCU identifier '$(MCU)' (set MCU))

endif


ifeq ($(MAKECMDGOALS),dry)
CCFLAGS+=-fsyntax-only
$(info Performing dry run (no binary))
endif

# End of common CCFLAGS
CXXFLAGS:=$(CCFLAGS)

CCFLAGS+=-std=gnu99

CXXFLAGS+=-std=gnu++11
CXXFLAGS+=-pedantic
CXXFLAGS+=-fno-exceptions -fno-rtti

# End setting flags


$(info Building lib$(OUT_LIB)$(BIN_SUFFIX).a)

ifdef WIN32
rwildcard=$(wildcard $1/$2) $(foreach d,$(wildcard $1/*),$(call rwildcard,$(d),$2))
SRC_FILES:=$(call rwildcard,$(SRC_PATH),*.c)
SRC_FILES:=$(SRC_FILES) $(call rwildcard,$(SRC_PATH),*.S)
SRC_FILES:=$(SRC_FILES) $(call rwildcard,$(SRC_PATH),*.cpp)

else ifdef UNIX
SRC_FILES:=$(shell find $(SRC_PATH) -type f -name *.c -o -name *.S -o -name *.cpp)

endif

# Exclude files for other MCU
not_contain=$(foreach v,$2,$(if $(findstring $1,$v),,$v))

# Include only the specific pinout under the same family
SRC_FILES:=$(call not_contain,/pinout/,$(SRC_FILES))
ifeq ($(MCU),MIMXRT1052)
SRC_FILES+=$(SRC_PATH)/system/pinout/MIMXRT1052.cpp

endif


OBJ_FILES:=$(SRC_FILES:$(SRC_PATH)/%.c=$(OUT_OBJ_PATH)/%.o)
OBJ_FILES:=$(OBJ_FILES:$(SRC_PATH)/%.S=$(OUT_OBJ_PATH)/%.o)
OBJ_FILES:=$(OBJ_FILES:$(SRC_PATH)/%.cpp=$(OUT_OBJ_PATH)/%.o)
OBJ_FILES:=$(OBJ_FILES:%.o=%$(BIN_SUFFIX).o)

DEPENDS:=$(OBJ_FILES:.o=.d)
-include $(DEPENDS)

# Create all the DIRs
OUT_DIRS:=$(sort $(dir $(OBJ_FILES)))
ifdef WIN32
$(shell mkdir $(subst /,\,$(OUT_DIRS)) lib > nul)

else ifdef UNIX
$(shell mkdir -p $(OUT_DIRS) lib)

endif


.PHONY: all clean dry

all: $(OUT_LIB_PATH)/lib$(OUT_LIB)$(BIN_SUFFIX).a

dry: $(OBJ_FILES)

.SECONDEXPANSION:

$(OUT_LIB_PATH)/lib$(OUT_LIB)$(BIN_SUFFIX).a: $(OBJ_FILES)
	$(info Packing library)
	@$(AR) $(ARFLAGS) $@ $^

$(OUT_OBJ_PATH)/%.o: $$(subst $(BIN_SUFFIX),,$(SRC_PATH)/%.c)
	$(info Compiling $(<))
	@$(CC) $(CPPFLAGS) $(CCFLAGS) -c -o $@ $<

$(OUT_OBJ_PATH)/%.o: $$(subst $(BIN_SUFFIX),,$(SRC_PATH)/%.S)
	$(info Compiling $(<))
	@$(CC) $(CPPFLAGS) $(CCFLAGS) -c -o $@ $<

$(OUT_OBJ_PATH)/%.o: $$(subst $(BIN_SUFFIX),,$(SRC_PATH)/%.cpp)
	$(info Compiling $(<))
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	$(info Cleaning $(<))
ifdef WIN32
	@rmdir /s /q $(OUT_OBJ_PATH) $(OUT_LIB_PATH)

else ifdef UNIX
	@rm -f $(OUT_LIB_PATH)/*.a
	@find $(OUT_OBJ_PATH) -type f \( -name *.o -o -name *.d \) -exec rm -f {} \;

endif
