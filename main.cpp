#include "HttpServer.h"

using namespace std;

int main(){
    HttpServer server;      //创建服务器实例，绑定端口
    server.start_listen();  //开始监听请求
    return 0;
}