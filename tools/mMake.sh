#!/bin/sh

#set -x

BUILD="`pwd`/build.mk"
MAXBUILD_DIR="`pwd`"
BUILD_DIR="$TOP_DIR/build_tools/"

if [ "$1" = "clean" ]; then
#	find -name "*.o" -delete
	make -C ${BUILD_DIR} -f Makefile.mk  build=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} clean
elif [ "$1" = "" ]; then
	make -C ${BUILD_DIR} -f Makefile.mk  build=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} modules
	echo "$1"
elif [ "$1" = "dbg" ]; then
	make -C ${BUILD_DIR} -f Makefile.mk  build=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} DBG="1" modules
	echo "$1"
elif [ "$1" = "help" ]; then
	echo "mMake.sh [opt]"
	echo "opt:"
	echo "    1. clean: clean the created files you compiled"
	echo "    2. modules: compile modules specified in the 'build.mk' file, 'MAXBUILD_MODULE'"
	echo "    3. dbg: display debug informations for debug build system"
	echo "    4. if the opt is null, then will build all targets, include modules and so on"
else
	make -C ${BUILD_DIR} -f Makefile.mk build=${BUILD} MAXBUILD_DIR=${MAXBUILD_DIR} $@
	echo "$1"
fi

