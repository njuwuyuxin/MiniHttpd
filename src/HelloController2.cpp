#include "HelloController2.h"

HttpResponse HelloController2::Accept(const HttpRequest& request) {
    Log::log("This is HelloController2",DEBUG);
    Log::log("request url="+request.get_url(),DEBUG);
    HttpResponse response(200);
    return response;
}
