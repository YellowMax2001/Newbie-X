include $(ENV_CLEAN_SCRIPT)

CompileHeaderFiles += $(call my_dir)

CompileSrcFiles += $(addprefix $(call my_dir)/, $(wildcard *.cpp))

SplittedCompileSrcFiles +=

CompileCCFlags +=

CompileCCLinkFlags += -lm -lstd

CompileTarget := Constructor

include $(ENV_BIN_TARGET_SCRIPT)

distclean::
	$(call rm_files, \
		$(addprefix $(TOP_DIR)/MaxCLib/targets/bin/, \
		${CompileTarget}))

CompilePhonyAdditionalTarget:
	$(info \
	$(call copy_to_dir,\
		$(TOP_DIR)/MaxCLib/targets/bin,\
		${CompileTarget}))
