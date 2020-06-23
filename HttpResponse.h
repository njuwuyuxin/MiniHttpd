// #ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class HttpResponse{
public:
    HttpResponse(int st);
    string get_response();
    string Allow;
    string Content_Type;
    
private:
    string version; //http版本
    string status;     //http状态码
    string date;       //response生成时间
    string server;  //http服务器名称
    // map<string,string> header;
};

// #endif