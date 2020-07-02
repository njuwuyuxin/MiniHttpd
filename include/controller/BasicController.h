#ifndef BASIC_CONTROLLER_H
#define BASIC_CONTROLLER_H

#include <regex>
#include "http/HttpRequest.h"
#include "http/HttpResponse.h"
#include "log/Log.h"

class BasicController{
public:
    virtual HttpResponse Accept(const HttpRequest& request) = 0;
    virtual ~BasicController() = 0;
private:

};

#endif