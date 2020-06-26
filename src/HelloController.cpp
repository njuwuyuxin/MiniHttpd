#include "HelloController.h"

HttpResponse HelloController::Accept(const HttpRequest& request) {
    cout<<"[DEBUG]: This is HelloController1"<<endl;
    HttpResponse response(200);
    return response;
}
