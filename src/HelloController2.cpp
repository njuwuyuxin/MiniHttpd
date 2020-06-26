#include "HelloController2.h"

HttpResponse HelloController2::Accept(const HttpRequest& request) {
    cout<<"[DEBUG]: This is HelloController2"<<endl;
    HttpResponse response(200);
    return response;
}
