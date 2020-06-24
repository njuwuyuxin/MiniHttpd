#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <thread>
#include <libconfig.h++>

#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;
using namespace libconfig;

class HttpServer{
public:
    HttpServer();
    inline int get_sock_id(){ return server_sock; };
    inline u_short get_port(){ return port; };
    void start_listen();
    static void accept_request(int client_sock,HttpServer* t);
private:
    int server_sock;
    u_short port;
    string baseURL;                         //服务器根目录
    string index;                           //网站首页文件
    unsigned int request_queue_length;      //请求队列长度，即最大可同时处理请求
    void load_config(string path);
    void startup();
};