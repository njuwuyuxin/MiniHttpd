#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <mutex>
#include <thread>

#include "./utils/Sem.h"
#include "./utils/Time.h"
using namespace std;

enum Level {INFO , DEBUG , WARN , ERROR };

class Log{
public:
    static Log *get_instance();
    static void init();                             //初始化Log模块，启动工作线程
    static void init(string path, bool Info_on, bool Debug_on, 
                    bool Warn_on, bool Error_on);   //初始化Log模块时设置log启动情况
    static void log(string info, Level level);
private:
    static string log_file_name;                    //日志文件名
    static queue<string> log_queue;                 //代写入文件的日志队列
    static mutex log_queue_mutex;
    static Sem log_queue_sem;
    static string log_path;
    static bool info_on;
    static bool debug_on;
    static bool warn_on;
    static bool error_on;

    static void async_write_file();                 //日志类的工作线程
};


#endif