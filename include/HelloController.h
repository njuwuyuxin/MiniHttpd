#ifndef HELLO_CONTROLLER_H
#define HELLO_CONTROLLER_H

#include "BasicController.h"

//需要预先声明该类供typedef使用
class HelloController;
//CMF = Controller Member Function
typedef HttpResponse (HelloController::*CMF)(const HttpRequest& request); 

class HelloController: public BasicController{
public:
    HelloController();
    HttpResponse Accept(const HttpRequest& request) override;
    ~HelloController(){};

private:
    map<string,CMF> router_map;
    CMF match_url(string url);          //路径匹配

    HttpResponse Login(const HttpRequest& request);
    HttpResponse Register(const HttpRequest& request);
    HttpResponse Hello(const HttpRequest& request);
};
#endif