#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <queue>
#include <mutex>
#include "Sem.h"
using namespace std;

class HttpServer;

class ThreadPool{
public:
    ThreadPool(HttpServer* server, int workthread = 8);
    void append(int sockfd);                //把事件加入请求队列
    void init();                            //创建N个工作线程并运行
    static void work(ThreadPool* pool);     //运行工作线程
private:
    HttpServer* http_server;                //与之绑定的HttpServer对象
    int thread_count;                       //线程池线程数
    queue<int> request_list;                //请求队列
    Sem request_list_sem;                   //请求队列信号量
    mutex request_list_mutex;               //请求队列互斥锁

    void run();                             //每个工作线程执行函数
};

#endif