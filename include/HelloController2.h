#include "BasicController.h"

class HelloController2: public BasicController{
public:
    HttpResponse Accept(const HttpRequest& request) override;
    ~HelloController2(){};
    
};