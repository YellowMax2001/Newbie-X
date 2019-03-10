#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# main Makefile configs.
# config cross compile, compile flags
# and so on.

CROSS_COMPILE := ${COMPILE_PREFIX}

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld

CC_FLAGS_DEFAULT := 
CC_INC_DEFAULT := -I${TOP_DIR}/MaxCLib/unity_tools

CC_FLAGS := $(CC_FLAGS_DEFAULT)
CC_INC := $(CC_INC_DEFAULT)

BUILD_RESET := $(shell pwd)/reset.mk
BUILD_BIN := $(shell pwd)/bin.mk
BUILD_LIB := $(shell pwd)/shared_lib.mk
ALL_MODULES :=
ALL_OBJS :=

# end of main Makefile configs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# build.mk configs. for target

MAXBUILD_DIR ?=
MAXBUILD_SUBDIRS ?=
MAXBUILD_MODULE ?=

# end of build.mk configs
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<

DBG :=

ifneq ($(MAXBUILD_SUBDIRS),)
$(foreach ,$(MAXBUILD_SUBDIRS),include ($(MAXBUILD_SUBDIRS)/build.mk))
endif

ifneq ($(strip $(build)),)
include $(build)
endif

all : modules
ifeq ($(DBG),1)
	@echo "final target"
	@echo "MAXBUILD_MODULE=$(MAXBUILD_MODULE)"
	@echo "BUILD_BIN=$(BUILD_BIN)"
	@echo "build=$(build)"
endif
.PHONY: all

modules : $(ALL_MODULES)
ifeq ($(DBG),1)
	@echo "final target"
	@echo "MAXBUILD_MODULE=$(MAXBUILD_MODULE)"
	@echo "BUILD_BIN=$(BUILD_BIN)"
	@echo "build=$(build)"
endif

.PHONY : clean
clean :
	rm -f $(ALL_MODULES)
	rm -f $(ALL_OBJS)

