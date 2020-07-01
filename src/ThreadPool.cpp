#include "ThreadPool.h"
#include "HttpServer.h"

ThreadPool::ThreadPool(HttpServer* server, int workthread){
    work_thread = workthread;
    http_server = server;
}

void ThreadPool::append(int sockfd){
    //TODO: lock and unlock,sem op
    request_list.push(sockfd);
}

void ThreadPool::init(){
    for(int i=0;i<1;i++){
        thread work_thread(ThreadPool::work,this);
        work_thread.detach();
    }
}

void ThreadPool::work(ThreadPool* pool){
    pool->run();
}

void ThreadPool::run(){
    while(1){
        if(request_list.empty())
            continue;
        int sockfd = request_list.front();
        request_list.pop();
        thread accept_thread(HttpServer::accept_request,sockfd,http_server);
        accept_thread.detach();
    }
}