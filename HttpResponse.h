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
    void set_header(string key, string val);    //设置头部自定义字段
    string get_response();

    /* 基础头部字段，供快速填充，自定义字段需手动设置 */
    string Allow;
    string Content_Encoding;
    string Content_Length;
    string Content_Type;
    string Expires;
    string Last_Modified;
    string Location;
    string Refresh;
    string Set_Cookie;
    string WWW_Authenticate;
    
private:
    string version;                     //http版本
    string status;                      //http状态码
    string date;                        //response生成时间
    string server;                      //http服务器名称
    map<string,string> custom_header;   //自定义头部字段
    string generate_header();           //使用全部信息组装HTTP Response头部
};

// #endif