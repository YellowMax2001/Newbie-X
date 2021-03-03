include $(ENV_CLEAN_SCRIPT)

CompileHeaderFiles += $(addprefix $(call my_dir)/, $(wildcard *.h))

CompileHeaderDirs += $(call my_dir)

CompileSrcFiles +=

SplittedCompileSrcFiles += $(addprefix $(call my_dir)/, $(wildcard *.c))

CompileCCFlags +=

CompileCCLinkFlags += -lm

CompileTarget := 

include $(ENV_BIN_TARGET_SCRIPT)

distclean::
	$(call rm_files, \
		$(addprefix $(TOP_DIR)/MaxCLib/targets/bin/, \
		$(patsubst %.c, %, $(notdir ${SplittedCompileSrcFiles}))))

CompilePhonyAdditionalTarget:
	$(info \
	$(call copy_to_dir,\
		$(TOP_DIR)/MaxCLib/targets/bin,\
		$(patsubst %.c, %, ${SplittedCompileSrcFiles})))
