#include "utils/Time.h"

Time::Time(){
    time_t t = time(0);
    char ch[64];
    strftime(ch, sizeof(ch), "%Y-%m-%d %H:%M:%S", localtime(&t)); //年-月-日 时-分-秒
    time_str = ch;
}

string Time::get_date(){
    size_t pos = time_str.find(" ");
    return time_str.substr(0,pos);
}

string Time::get_time(){
    size_t pos = time_str.find(" ");
    return time_str.substr(pos+1);
}