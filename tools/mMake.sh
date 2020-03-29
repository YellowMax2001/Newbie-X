#!/bin/sh

#set -x

CUR_DIR="`pwd`"

# This is common make command,
# it will make the current main target.
BUILD_MK_FILE="${BUILD_TOOLS_DIR}/Compile_build.mk"
BUILD="${CUR_DIR}/Compile.mk"
MAXBUILD_DIR="${CUR_DIR}"

echo "\033[31mCUR_DIR:\033[0m \033[34m ${CUR_DIR} \033[0m"
echo "\033[31mBUILD_MK_FILE:\033[0m \033[34m ${BUILD_MK_FILE} \033[0m"
echo "\033[31mBUILD:\033[0m \033[34m ${BUILD} \033[0m"
echo "\033[31mMAXBUILD_DIR:\033[0m \033[34m ${MAXBUILD_DIR} \033[0m"

if [ "$1" = "clean" ]; then
#	find -name "*.o" -delete
	make -f ${BUILD_MK_FILE}  BUILD_SCRIPT_FILE=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} clean
elif [ "$1" = "" ]; then
	make -f ${BUILD_MK_FILE}  BUILD_SCRIPT_FILE=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} all
	echo "$1"
elif [ "$1" = "dbg" ]; then
	make -f ${BUILD_MK_FILE}  BUILD_SCRIPT_FILE=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} BUILD_DBG="1" all
	echo "$1"
elif [ "$1" = "help" ]; then
	echo "mMake.sh [opt]"
	echo "opt:"
	echo "    1. clean: clean the created files you compiled"
	echo "    2. all: compile modules specified in the 'build.mk' file, 'MAXBUILD_MODULE'"
	echo "    3. dbg: display debug informations for debug build system"
	echo "    4. if the opt is null, then will build all targets, include modules and so on"
else
	make -f ${BUILD_MK_FILE} build=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} $@
	echo "$1"
fi

