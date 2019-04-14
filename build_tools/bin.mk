target_src := $(addprefix $(MAXBUILD_DIR)/,$(MAXBUILD_SRC))
target_module := $(addprefix $(MAXBUILD_DIR)/,$(MAXBUILD_MODULE))

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# main Makefile config

ALL_MODULES += $(target_module)
ALL_OBJS += $(target_src:%.c=%.o)

CC_FLAGS += $(MAXBUILD_CCFLAGS)
CC_INC += $(MAXBUILD_INC)

# end of main Makefile config
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

target_objs := $(target_src:%.c=%.o)
target_deps := $(target_src:%.c=%.d)
$(target_module) : $(target_objs)
	$(CC) -o $@ $^ $(strip $(CC_FLAGS))

%.o : %.c
ifeq ($(DBG),1)
	@echo "CC=$(CC)"
	@echo "CC_INC=$(CC_INC)"
	@echo "CC_FLAGS=$(CC_FLAGS)"
	@echo "$<"
	@echo "$^"
endif
	$(CC) $(strip $(CC_INC)) -c -o $@ $< $(strip $(CC_FLAGS))

#%.o : %.d
#
#%.d : %.c
#	$(CC) $(CC_INC) $(strip $(CC_FLAGS)) -MM $< > $d
#
#include $(target_src:%.c=%.d)
#	$(CC) $(strip $(CC_INC)) $(strip $(CC_FLAGS)) -c -o $@ $<

