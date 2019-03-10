target_src := $(addprefix $(MAXBUILD_DIR)/,$(MAXBUILD_SRC))
target_module := $(addprefix $(MAXBUILD_DIR)/,$(MAXBUILD_MODULE))

#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# main Makefile config

ALL_MODULES += $(target_module)
ALL_OBJS += $(target_src:%.c=%.o)

CC_FLAGS += --shared -fPIC

# end of main Makefile config
#<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

target_objs := $(target_src:%.c=%.o)
$(target_module) : $(target_objs)
	$(CC) $(strip $(CC_FLAGS)) -o $@ $^

%.o : %.c %.h
ifeq ($(DBG),1)
	@echo "CC=$(CC)"
	@echo "CC_INC=$(CC_INC)"
	@echo "$<"
	@echo "$^"
endif
	$(CC) $(strip $(CC_INC)) $(strip $(CC_FLAGS)) -c -o $@ $<

