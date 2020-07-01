#include "utils/Log.h"

void Log::log(string info, Level level){
    switch(level){
        case INFO:cout<<"\033[32m[INFO]: \033[0m"<<info<<endl;break;
        case DEBUG:cout<<"\033[34m[DEBUG]: \033[0m"<<info<<endl;break;
        case WARN:cerr<<"\033[33m[WARN]: \033[0m"<<info<<endl;break;
        case ERROR:cerr<<"\033[31m[ERROR]: \033[0m"<<info<<endl;break;
    }
}