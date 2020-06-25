#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

#include <zlib.h>
#include <zconf.h>
#include <string.h>
using namespace std;

class HttpResponse{
public:
    HttpResponse(int st);
    void set_header(string key, string val);    //设置头部自定义字段
    void load_from_file(string url);
    int get_response(char*& resp_buffer);

    static void init_content_type_map();        //初始化映射表
    static map<string,string> content_type_map; //文件扩展名与Content-Type映射表

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
    string version;                             //http版本
    string status;                              //http状态码
    string date;                                //response生成时间
    string server;                              //http服务器名称
    map<string,string> custom_header;           //自定义头部字段
    string generate_header();                   //使用全部信息组装HTTP Response头部
    void auto_set_content_type(string url);     //根据扩展名判断文件类型设置Content-Type字段
    string response_body;                       //返回内容体
};

#endif