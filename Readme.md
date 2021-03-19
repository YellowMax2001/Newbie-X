## Links
CSDN：https://yellowmax.blog.csdn.net/

微信公众号
<center><img src="https://img-blog.csdnimg.cn/2020091909522415.PNG"></center>

## 文章目录
[NewbieX 个人代码仓库介绍](https://yellowmax.blog.csdn.net/article/details/106315553)

[NewbieX 二叉树的图形化显示](https://yellowmax.blog.csdn.net/article/details/115013903)

## Newbie-C
### 简介
该工程目前是基于C语言来完成的，主要提供或者期望提供以下几个功能：
- 一个通用的小型工程编译Makefile，使用方法与简介见这里：[Build系统介绍](https://yellowmax.blog.csdn.net/article/details/106315553)
- 常见的数据结构，比如环形缓冲区、红黑树、二叉树等。
- 常见的字符串处理操作。
- 工程上常用的小功能模块，比如消息分发器、数据流分发管理模块等。

### 目录结构
```
.
├── CPPs
│   ├── STLimpl
│   ├── basicLang
│   └── commonInc
├── Compile_list.mk
├── LICENSE
├── MaxCLib
│   ├── algorithm
│   ├── data_structs
│   ├── include
│   ├── main.c
│   ├── streams
│   ├── targets
│   └── test.c
├── Readme.md
├── Readme.txt
├── build_tools
│   ├── Compile_BinTarget.mk
│   ├── Compile_EnvClean.mk
│   ├── Compile_SharedLibTarget.mk
│   ├── Compile_StaticLibTarget.mk
│   ├── Compile_build.mk
│   ├── Compile_functions.mk
│   ├── build_system_design.md
│   ├── build_system_design.txt
│   ├── main.c
│   └── newbie_build.src
├── main.c
└── tools
    └── mMake.sh
```

主要有
1. 编译模块：build_tools
2. 数据结构：data_structs
3. 数据流模块：streams
4. 通用工具：unity_tools

## 全局编译
```shell
git clone git@github.com:YellowMax2001/Newbie-X.git
cd Newbie-C
source build_tools/newbie_build.src
mMake.mk
```