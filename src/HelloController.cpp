#include "HelloController.h"

typedef HttpResponse (HelloController::*CMF)(const HttpRequest& request); 

HelloController::HelloController(){
    //添加新的消息处理函数需要在这里注册
    CMF login_f = &HelloController::Login;
    router_map.insert(pair<string,CMF>("/login",login_f));
    CMF register_f = &HelloController::Register;
    router_map.insert(pair<string,CMF>("/register",register_f));
}

HttpResponse HelloController::Accept(const HttpRequest& request) {
    string url = request.get_url();
    Log::log("servlet request url="+url,DEBUG);
    HttpResponse response(200);
    //根据url匹配情况调用不同的成员函数处理
    for(auto i:router_map){
        if(i.first == url){
            response = (this->*i.second)(request);
            break;
        }
    }
    response.generate_response();
    return response;
}

HttpResponse HelloController::Login(const HttpRequest& request){
    HttpResponse response(200);
    response.Content_Type = "text/plain";
    response.response_body = "This is Login func";
    return response;
}

HttpResponse HelloController::Register(const HttpRequest& request){
    HttpResponse response(200);
    response.Content_Type = "text/plain";
    response.response_body = "This is Register func";
    return response;
}