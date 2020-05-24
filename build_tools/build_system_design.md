这个简单的编译模块有下面几个功能：
1. 一次设置（执行一个初始化脚本），后面就可以全部编译了。
2. 目前仅支持两级编译，就是一个文件汇总需要编译的模块顶层文件夹一级，模块子文件夹里面放置模块文件相关的 Compile.mk 文件来指明模块所需的文件和要编译的类型（可执行文件、动态库、静态库）。
3. 一个文件夹里面可以有多个目标，会编译编译生成，比如两个完整的可执行文件，一个动态库同时在一个 Compile.mk 文件里面指定编译。
4. 可以自定义设置需要拷贝生成的编译目标到某个文件夹里面去。
5. 可以编译全部指定目标，也可以在某一个子目录下编译单个指定目标文件。

整个仓库的根目录下面有一个 build_tools 文件夹，里面包含了主要 make 系统的实现，比如 Compile_functions.mk 文件就定义了一些常用的脚本函数，用于 make 过程中调用，比如 copy，rm，显示当前文件夹路径等。如果需要使用这个编译模块的话就需要在仓库的根目录下面执行：`source ./build_tools/newbie_build.src` 来初始化当前的整个编译环境，用于后续具体目标编译，下面是根目录下面一个模块列表手机文件 Compile_list.mk 文件，里面内容如下：
```
#cat Compile_list.mk

# If we have some **build dependency**,
# **must put the dependency items in the top position**.
CompileTargetLists += $(TOP_DIR)/MaxCLib/algorithm/search
CompileTargetLists += $(TOP_DIR)/MaxCLib/algorithm/sort
CompileTargetLists += $(TOP_DIR)/MaxCLib/algorithm/graphics
```

这个是指明整个项目都需要编译哪些个模块，如果模块之间有相互依赖的话就需要把被依赖项放在前面先编译出来，目前没办法处理互相依赖的模块，而一个 Compile.mk 文件示例如下：
```
# cat MaxCLib/algorithm/sort/Compile.mk
include $(ENV_CLEAN_SCRIPT)

CompileHeaderFiles += $(addprefix $(call my_dir)/, $(wildcard *.h))

# 用于添加需要编译的源文件，如果这些源文件对应同一个目标的话就可以使用这个变量进行设置。
CompileSrcFiles +=

# 用于添加需要分别生成单个目标的源文件，这个变量列表里面所有的文件都会单独编译出来独立的对应的可执行文件或者是库文件
SplittedCompileSrcFiles += $(addprefix $(call my_dir)/, $(wildcard *.c))

CompileCCFlags +=

CompileCCLinkFlags += -lm

CompileTarget :=

include $(ENV_BIN_TARGET_SCRIPT)

distclean::
        $(call rm_files, \
                $(addprefix $(TOP_DIR)/MaxCLib/targets/bin/, \
                $(patsubst %.c, %, $(notdir ${SplittedCompileSrcFiles}))))

# 这个变量用于整个上面编译过程完成之后的自定义命令，这里就是拷贝该文件下面无后缀名的文件（可执行文件）到目录 `$(TOP_DIR)/MaxCLib/targets/bin` 里面去。
CompilePhonyAdditionalTarget:
        $(info \
        $(call copy_to_dir,\
                $(TOP_DIR)/MaxCLib/targets/bin,\
                $(patsubst %.c, %, ${SplittedCompileSrcFiles})))
```

编译的时候可以调用 mMake.sh 脚本，这个脚本的路径在 source 命令初始化编译环境的时候已经设置到系统路径里面去了，可以在根目录下面直接执行，会编译整个 Compile_list.mk 文件里面所有的模块目标，也可以在某一个包含了 Compile.mk 的文件夹下面执行，这个时候只会编译 Compile.mk 文件指定的模块目标。