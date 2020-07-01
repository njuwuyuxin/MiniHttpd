#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/epoll.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <regex>
#include <libconfig.h++>

#include "utils/Log.h"
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "thread/ThreadPool.h"
#include "controller/BasicController.h"
#include "controller/HelloController.h"
#include "controller/HelloController2.h"

using namespace std;
using namespace libconfig;

const int MAX_EVENT_NUMBER = 10000;

class HttpServer{
public:
    HttpServer();
    ~HttpServer();
    inline int get_sock_id(){ return server_sock; };
    inline u_short get_port(){ return port; };
    void start_listen();
    static void accept_request(int client_sock,HttpServer* t);
private:
    int server_sock;
    u_short port;
    ThreadPool thread_pool;                             //工作线程池
    int work_thread_count;                              //工作线程数
    int epollfd;                                        //epoll文件描述符
    string baseURL;                                     //服务器根目录
    string index;                                       //网站首页文件
    unsigned int request_queue_length;                  //请求队列长度，即最大可同时处理请求
    map<string,BasicController*> controller_map;        //路径-Controller映射表

    void load_config(string path);                      //加载配置文件
    void init_controller_map();                         //初始化controller路径映射表
    void startup();
    void add_epoll_fd(int event_fd);                    //向内核事件表注册事件

    BasicController* match_url(HttpRequest& request);   //匹配url属于哪个Controller
    HttpResponse file_request(HttpRequest& request);    //处理文件请求
};

#endif