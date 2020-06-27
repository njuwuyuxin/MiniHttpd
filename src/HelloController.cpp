#include "HelloController.h"

HelloController::HelloController(){
    //添加新的消息处理函数需要在这里注册
    router_map.insert(pair<string,CMF>("/login",&HelloController::Login));
    router_map.insert(pair<string,CMF>("/register",&HelloController::Register));
    router_map.insert(pair<string,CMF>("/hello",&HelloController::Hello));
}

//仅做路径匹配，不负责参数解析
CMF HelloController::match_url(string url){
    for(auto i:router_map){
        std::regex route_path("(^"+i.first+"$)"+"|(^"+i.first+"\?)");
        std::cmatch m;
        bool flag = std::regex_search(url.c_str(),m,route_path);
        if(flag){
            Log::log("[HelloController]: match route: "+i.first,DEBUG);
            return i.second;
        }
    }
    return NULL;
}

HttpResponse HelloController::Accept(const HttpRequest& request) {
    string url = request.get_url();
    Log::log("servlet request url="+url,DEBUG);
    HttpResponse response(200);

    //根据url匹配情况调用不同的成员函数处理
    CMF func = match_url(request.get_url());
    if(func!=NULL){
        response =(this->*func)(request);
    }
    else{
        response.status = "404";
        response.response_body = "404 not found";
    }
    response.generate_response();
    return response;
}

HttpResponse HelloController::Login(const HttpRequest& request){
    HttpResponse response(200);
    response.Content_Type = "application/json";
    response.response_body = "{\"statusCode\":0,\"info\":\"登录成功\"}";
    return response;
}

HttpResponse HelloController::Register(const HttpRequest& request){
    HttpResponse response(200);
    response.Content_Type = "application/json";
    response.response_body = "{\"statusCode\":-1,\"info\":\"注册失败\"}";
    return response;
}

HttpResponse HelloController::Hello(const HttpRequest& request){
    HttpResponse response(200);
    response.Content_Type = "text/plain";
    response.response_body = "Hello world";
    return response;
}