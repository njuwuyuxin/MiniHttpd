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

#include "HttpRequest.h"
#include "HttpResponse.h"

using namespace std;

class HttpServer{
public:
    HttpServer();
    HttpServer(u_short p);
    inline int get_sock_id(){ return server_sock; };
    inline u_short get_port(){ return port; };
    void start_listen();
    static void accept_request(int client_sock,HttpServer* t);
private:
    int server_sock;
    u_short port;
    string baseURL;

    void startup();
};