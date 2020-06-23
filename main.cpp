#include "HttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
using namespace std;

int main(){
    HttpServer server(9999);
    server.start_listen();
    return 0;
}