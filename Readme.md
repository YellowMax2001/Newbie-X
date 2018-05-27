## Newbie-C
### 简介
该工程目前是基于C语言来完成的，主要提供或者期望提供以下几个功能：
- 一个通用的小型工程编译Makefile，使用方法与简介见这里：[Build系统介绍](https://github.com/YellowMax2001/Newbie-C/blob/master/build_tools/build_system_design.md)
- 常见的数据结构，比如环形缓冲区、红黑树、二叉树等。
- 常见的字符串处理操作。
- 工程上常用的小功能模块，比如消息分发器、数据流分发管理模块等。

### 目录结构
```
├── build_tools
│   ├── bin.mk
│   ├── build_system_design.txt
│   ├── build_system.src
│   ├── clean.mk
│   ├── main.c
│   ├── Makefile
│   ├── shard_lib.mk
│   ├── static.mk
│   └── test_build.c
├── compile.mk
├── LICENSE
├── main.c
├── Makefile
├── Makefile.build
├── MaxCLib
│   ├── build
│   │   └── build_system_design.txt
│   ├── data_structs
│   │   ├── fifo
│   │   │   ├── Makefile
│   │   │   ├── Readme.md
│   │   │   ├── ring_buffer
│   │   │   │   ├── Makefile
│   │   │   │   ├── rb_test.c
│   │   │   │   ├── Readme.md
│   │   │   │   └── ring_buffer.c
│   │   │   └── test_build.c
│   │   └── Makefile
│   ├── include
│   │   ├── data_structs
│   │   │   └── ring_buffer.h
│   │   └── streams
│   │       └── msg_factory.h
│   ├── main.c
│   ├── Makefile
│   ├── streams
│   │   └── msg_factory.c
│   ├── test.c
│   ├── text_proc
│   └── unity_tools
│       ├── cell_types.h
│       ├── plat_log.h
│       └── types.h
├── Readme.md
└── Readme.txt
```

主要有
1. 编译模块：build_tools
2. 数据结构：data_structs
3. 数据流模块：streams
4. 通用工具：unity_tools
5. 文本处理工具：unity_tools
