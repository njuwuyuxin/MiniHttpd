#include "HelloController.h"

HttpResponse HelloController::Accept(const HttpRequest& request) {
    Log::log("This is HelloController1",DEBUG);
    Log::log("request url="+request.get_url(),DEBUG);
    HttpResponse response(200);


    return response;
}
