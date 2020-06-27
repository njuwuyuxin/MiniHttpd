#ifndef BASIC_CONTROLLER_H
#define BASIC_CONTROLLER_H

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Log.h"

class BasicController{
public:
    virtual HttpResponse Accept(const HttpRequest& request) = 0;
    virtual ~BasicController() = 0;
private:

};

#endif