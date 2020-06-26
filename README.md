## Minihttpd
---
一个轻量化HTTP服务器，用于学习网络通信，socket编程等

### 编译环境
- c++11

### 编译工具
- cmake 3.17.1
- ninja 1.5.1

### 库依赖
- libconfig
- zlib

### 编译
首先确保libconfig,zlib全局安装

头文件在`usr/local/include`

库文件在`usr/local/lib`

之后新建一个构建目录，并进入
```
mkdir cmake-build-debug
cd cmake-build-debug
```
之后在构建目录执行

```
cmake -G Ninja ..
ninja
```