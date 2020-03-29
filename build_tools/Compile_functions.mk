# $1: copy directory path
# $2: files need to copy
define copy_to_dir
    $(shell $(Mewbie_Cp) $2 $1 -rfd)
endef

define my_dir
    $(shell pwd)
endef

# move $1 into $2
define mv_files
    $(shell $(Newbie_Mv) $1 $2)
endef

# remove files
define rm_files
    $(shell $(Mewbie_Rm) -rf $1)
endef