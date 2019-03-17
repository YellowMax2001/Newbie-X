#####################
include $(BUILD_RESET)

MAXBUILD_SRC := bubble_sort.c

MAXBUILD_MODULE := bubble_sort

include $(BUILD_BIN)

#####################
include $(BUILD_RESET)

MAXBUILD_SRC := insertion_sort.c

MAXBUILD_MODULE := insertion_sort

include $(BUILD_BIN)

#####################
include $(BUILD_RESET)

MAXBUILD_SRC := selection_sort.c

MAXBUILD_MODULE := selection_sort

include $(BUILD_BIN)

