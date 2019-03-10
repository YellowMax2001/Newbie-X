## 使用
首先在系统上面的根目录下面执行 source ./build_tools/newbie_build.src 来初始化环境，注意--一定要在根目录下。

## 功能需求
1. 可以选择整体进行编译（在build的基础上面实现一个类linux的编译方式），
并且可以选择内部的某些模块编译成动态库、可执行文件、静态库等
2. 可以选择局部编译，并且可以选择编译成动态库、可执行文件、静态库等
3. 每个字模块可以单独编译，比如数据结构模块、文件处理模块、格式解析模块等
4. 支持编译多个bin文件
    - 在需要编译成bin文件的根目录下面的Makefile中加入 MAXBUILD_BIN := main_file 字样
    - 保证 main_file.c 文件一定存在，并且该文件就是包含 main 函数的文件（需要使用该文件最终生成目标 bin）
    - 其余的子文件以及文件夹还是分别按照 obj-y += dir/ obj-y += file.c 的形式追加
    - 尽量不要再写 obj-y += main_file.c 追加 main 函数所在的文件
5. 支持编译多个动态库与静态库
    - 指定目标库的名字 **MAXBUILD_STATIC := name （静态库，最终会生成libname.a）	MAXBUILD_SHARED := name（动态库，最终会生成libname.so）**
    - obj-static 与 obj-shared 指定目标文件
6.支持将built-in.o限定与某一个文件夹下面，该文件夹下面的built-in.o不参与更上层的目标文件生成过程
	- obj-dir 指定限定文件夹的名字
7.支持在某一个文件夹下面指定目标进行make，生成目标文件
	- 在自定义工程的顶级目录下面首先键入 include $(BUILD_ENV)
	- 参照根目录下面的 Makefile 编写规则编写即可满足要求

## Makefile规则
1. 首要目标（终极目标）
make会找到第一个 target: 字样作为本次make的目标，如果你不想在make的时候指定目标，
比如make all想省略掉all，那么all这个目标就需要在所有依赖规则的最开始。你也可以在最开始
仅仅指定终极目标，而不指定其依赖，而在后面才去指定依赖关系，例如下面的写法：
```
__build :
#下面的all就会被忽略
all :
	do something
#下面这个才是终极目标的依赖
__build : xxx yyy
	do something
```