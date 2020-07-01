#include "ThreadPool.h"
#include "HttpServer.h"

ThreadPool::ThreadPool(HttpServer* server, int workthread){
    thread_count = workthread;
    http_server = server;
}

void ThreadPool::append(int sockfd){
    //TODO: lock and unlock,sem op
    request_list_mutex.lock();
    request_list.push(sockfd);
    request_list_mutex.unlock();
}

void ThreadPool::init(){
    for(int i=0;i<thread_count;i++){
        thread work_thread(ThreadPool::work,this);
        work_thread.detach();
    }
}

void ThreadPool::work(ThreadPool* pool){
    pool->run();
}

void ThreadPool::run(){
    while(1){
        request_list_mutex.lock();
        if(request_list.empty()){
            request_list_mutex.unlock();
            continue;
        }
        int sockfd = request_list.front();
        request_list.pop();
        request_list_mutex.unlock();
        thread accept_thread(HttpServer::accept_request,sockfd,http_server);
        accept_thread.detach();
    }
}