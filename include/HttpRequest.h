#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Log.h"
using namespace std;

class HttpRequest{
public:
    HttpRequest(string raw_data);
    inline const string get_method(){ return method; };
    inline const string& get_url() const { return url; };
    inline void set_url(string u){ url = u; };
    inline const map<string,string>& get_header(){ return header; };
    inline const map<string,string>& get_params(){ return params; };
private:
    void resolve_get_params();      //解析GET请求参数

    string method;                  //该http请求方法
    string url;                     //请求URL
    string version;                 //http版本
    map<string,string> header;      //头部字段信息
    map<string,string> params;      //get请求参数,对post请求无效
    string req_body;                //post请求体,get请求则为空
};

std::vector<std::string> splitString(std::string srcStr, std::string delimStr,bool repeatedCharIgnored);

#endif