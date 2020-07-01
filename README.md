## Minihttpd
---
一个Linux下C++轻量化HTTP服务器，用于学习网络通信，理解Http服务器原理等

- B/S模型
- 使用epoll进行并发处理（ET模式）
- 半连接半反应堆线程池
- HTTP报文解析 —— GET/POST
- 基于Controller的微服务框架
- Webbench测试支持**上万并发连接**
- 使用libconfig对服务器运行参数高度配置

### 编译环境
- Ubuntu 16.04, CentOS7
- c++11

### 构建工具
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
