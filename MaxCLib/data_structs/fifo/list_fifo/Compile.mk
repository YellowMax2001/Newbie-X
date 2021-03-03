include $(ENV_CLEAN_SCRIPT)

CompileHeaderFiles += $(addprefix $(call my_dir)/, $(wildcard *.h))

CompileHeaderDirs += $(call my_dir)

CompileSrcFiles += $(addprefix $(call my_dir)/, $(wildcard list_fifo.c))

SplittedCompileSrcFiles +=

CompileCCFlags += -Wpointer-to-int-cast

CompileCCLinkFlags += -lm -lstd

CompileTarget := ListFifo.so

include $(ENV_SHARELIB_SCRIPT)

distclean::
	$(call rm_files, \
		$(addprefix $(TOP_DIR)/MaxCLib/targets/share_libs/, \
		${CompileTarget}))

CompilePhonyAdditionalTarget:
	$(info \
	$(call copy_to_dir,\
		$(TOP_DIR)/MaxCLib/targets/share_libs/,\
		${CompileTarget}))
