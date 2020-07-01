#ifndef HELLO_CONTROLLER2_H
#define HELLO_CONTROLLER2_H
#include "BasicController.h"

class HelloController2: public BasicController{
public:
    HttpResponse Accept(const HttpRequest& request) override;
    ~HelloController2(){};
    
};
#endif