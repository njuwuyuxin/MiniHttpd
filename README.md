## Minihttpd
---
一个Linux下C++轻量化HTTP服务器，用于学习网络通信，理解Http服务器原理等

- B/S模型
- 使用epoll进行并发处理（ET模式）
- 半连接半反应堆线程池,信号量+自旋锁保证线程同步（旧版本使用互斥锁）
- HTTP报文解析 —— GET/POST
- 基于Controller的微服务框架
- Webbench测试支持**上万并发连接**
- 使用libconfig对服务器运行参数高度配置
- 异步日志输出模块，记录服务器运行状态，支持日志信息级别筛选

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

### 微服务框架
目前还在实现中

若需使用微服务框架，可参考src/controller下的HelloController，实现自己的Controller类
- 派生类需继承BasicController
- 将派生类对象和对应路由的映射添加至HttpServer的路由表中
- 需为派生类实现Accept函数，当有服务器收到http请求时会根据url路径分发至各controller
- 派生类内路由使用了成员函数指针维护映射表，可参考HelloController
- Accept函数接收一个HttpRequest参数，返回HttpResponse
- 可使用HttpRequest类进行对请求解析，可解析http头和GET请求参数

### 压力测试
使用Web bench进行压力测试，模拟10000个客户端，测试时间10秒

`webbench -c 10000 -t 10 http://127.0.0.1:9999/`

#### 使用互斥锁
![互斥锁](https://s1.ax1x.com/2020/07/11/U1AiTg.png)

#### 使用自旋锁
![自旋锁](https://s1.ax1x.com/2020/07/11/U1AP0S.png)