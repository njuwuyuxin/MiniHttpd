#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include "./utils/Sem.h"
using namespace std;

enum Level {INFO , DEBUG , WARN , ERROR };

class Log{
public:
    static Log *get_instance();
    static void init();                                    //初始化Log模块，启动工作线程
    static void log(string info, Level level);
private:
    static queue<string> log_queue;                 //代写入文件的日志队列
    static mutex log_queue_mutex;
    static Sem log_queue_sem;

    static void async_write_file();                 //日志类的工作线程
};


#endif