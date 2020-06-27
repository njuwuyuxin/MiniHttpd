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
    HttpResponse Login(const HttpRequest& request);
    HttpResponse Register(const HttpRequest& request);
    map<string,CMF> router_map;
    
};