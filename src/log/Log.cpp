#include "log/Log.h"

string Log::log_file_name;
queue<string> Log::log_queue; 
mutex Log::log_queue_mutex;
Sem Log::log_queue_sem;

Log* Log::get_instance(){
    static Log instance;
    return &instance;
}

void Log::init(){
    Time t;
    log_file_name = t.get_date();
    thread write_thread(Log::async_write_file);
    write_thread.detach();
}

void Log::log(string info, Level level){
    string log_string;
    Time t;
    string time_s = t.get_time();
    log_string += (time_s + " ");
    switch(level){
        case INFO:{
            log_string += "[INFO]:";
            cout<<time_s<<" \033[32m[INFO]: \033[0m"<<info<<endl;break;
        }
        case DEBUG:{
            log_string += "[DEBUG]:";
            cout<<time_s<<" \033[34m[DEBUG]: \033[0m"<<info<<endl;break;
        }
        case WARN:{
            log_string += "[WARN]:";
            cerr<<time_s<<" \033[33m[WARN]: \033[0m"<<info<<endl;break;
        }
        case ERROR:{
            log_string += "[ERROR]:";
            cerr<<time_s<<" \033[31m[ERROR]: \033[0m"<<info<<endl;break;
        }
    }

    log_string += info;
    log_queue_mutex.lock();
    log_queue.push(log_string);
    log_queue_mutex.unlock();
    log_queue_sem.post();
}

void Log::async_write_file(){
    ofstream out_log(log_file_name,ios::app);
    if(out_log.fail()){
        Log::log("async log write thread start failed",ERROR);
        return;
    }
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