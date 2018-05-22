TOP_DIR := $(shell pwd)
INCLUDE_DIR := $(TOP_DIR)/MaxCLib/unity_tools/ \
	$(shell find ${TOP_DIR} -name "include")
COMPILE := 

CC := $(COMPILE)gcc
AR := $(COMPILE)ar

COMPILE_FLAG := -std=gnu99
LINK_FLAG := -lpthread
INCLUDE_FLAG := $(addprefix -I,$(INCLUDE_DIR))

#to make the sub-make can use it
export TOP_DIR INCLUDE_DIR
export CC AR
export COMPILE_FLAG LINK_FLAG INCLUDE_FLAG
export CLEAN_OBJ

obj-y :=
obj-share :=
obj-static :=

TARGET_BIN := main

obj-y += MaxCLib/ build_tools/
obj-y += main.c

all:
	@ make -C $(shell pwd) -f $(TOP_DIR)/Makefile.build

clean:
	@ find -name "*.o" -delete
	@ make -C $(shell pwd) -f $(TOP_DIR)/Makefile.build \
CLEAN_OBJ=clean clean

distclean:
	@ find -name "*.o" -delete
	@ make -C $(shell pwd) -f $(TOP_DIR)/Makefile.build \
CLEAN_OBJ=distclean distclean

.PHONY : clean distclean

