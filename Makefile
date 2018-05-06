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

obj-y :=
obj-share :=
obj-static :=

TARGET := main

obj-y += main.c
obj-y += MaxCLib/

$(warning Top Makefile parser)

all:
	@ make -C $(shell pwd) -f $(TOP_DIR)/Makefile.build
	@ $(CC) -o $(TARGET) built-in.o $(LINK_FLAG)
#	 @ $(AR) rcs -o $(TARGET) built-in.o

clean:
	find -name "*.o" -delete
	rm $(TARGET)

.PHONY : clean
