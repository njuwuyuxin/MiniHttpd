#include "BasicController.h"

class HelloController: public BasicController{
public:
    HttpResponse Accept(const HttpRequest& request) override;
    ~HelloController(){};
    
};