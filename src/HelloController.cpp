#include "HelloController.h"

HttpResponse HelloController::Accept(const HttpRequest& request) {
    cout<<"[DEBUG]: This is HelloController1"<<endl;
    cout<<"[DEBUG]: request url="<<request.get_url()<<endl;
    HttpResponse response(200);


    return response;
}
