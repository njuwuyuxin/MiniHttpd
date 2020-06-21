// #ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class HttpRequest{
public:
    HttpRequest(string raw_data);
private:
    string method;  //该http请求方法
    string url;     //请求URL
    string version; //http版本
    map<string,string> header;
};

// #endif