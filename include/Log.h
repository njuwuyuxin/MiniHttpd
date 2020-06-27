#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

enum Level {INFO , DEBUG , WARN , ERROR };

class Log{
public:
    static void log(string info, Level level);
};


#endif