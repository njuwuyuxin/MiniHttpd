#include "log/Log.h"

queue<string> Log::log_queue; 
mutex Log::log_queue_mutex;
Sem Log::log_queue_sem;

Log* Log::get_instance(){
    static Log instance;
    return &instance;
}

void Log::init(){
    thread write_thread(Log::async_write_file);
    write_thread.detach();
}

void Log::log(string info, Level level){
    string log_string;
    switch(level){
        case INFO:{
            log_string = "[INFO]:";
            cout<<"\033[32m[INFO]: \033[0m"<<info<<endl;break;
        }
        case DEBUG:{
            log_string = "[DEBUG]:";
            cout<<"\033[34m[DEBUG]: \033[0m"<<info<<endl;break;
        }
        case WARN:{
            log_string = "[WARN]:";
            cerr<<"\033[33m[WARN]: \033[0m"<<info<<endl;break;
        }
        case ERROR:{
            log_string = "[ERROR]:";
            cerr<<"\033[31m[ERROR]: \033[0m"<<info<<endl;break;
        }
    }

    log_string += info;
    log_queue_mutex.lock();
    log_queue.push(log_string);
    log_queue_mutex.unlock();
    log_queue_sem.post();
}

void Log::async_write_file(){
    ofstream out_log("mylog.log");
    while(1){
        log_queue_sem.wait();
        log_queue_mutex.lock();
        string log_string = log_queue.front();
        log_queue.pop();
        log_queue_mutex.unlock();
        out_log<<log_string<<endl;
    }
    out_log.close();
}