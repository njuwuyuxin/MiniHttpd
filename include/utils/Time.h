#ifndef TIME_H
#define TIME_H

#include <string>
#include <assert.h>
using namespace std;

class Time{
public:
    Time();
    string get_date();
    string get_time();
private:
    string time_str;
};

#endif