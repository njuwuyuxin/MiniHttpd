#include "HelloController2.h"

HttpResponse HelloController2::Accept(const HttpRequest& request) {
    cout<<"[DEBUG]: This is HelloController2"<<endl;
    cout<<"[DEBUG]: request url="<<request.get_url()<<endl;
    HttpResponse response(200);
    return response;
}
