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
#include <thread>

#include "HttpRequest.h"
#include "HttpResponse.h"
using namespace std;

void error_die(const char *sc)
{
    perror(sc);
    exit(1);
}

int startup(u_short *port)
{
    int httpd = 0;
    int on = 1;
    struct sockaddr_in name;

    httpd = socket(PF_INET, SOCK_STREAM, 0);    //使用TCP协议
    if (httpd == -1)
        error_die("socket");
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(*port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(httpd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)  
    {  
        error_die("setsockopt failed");
    }
    if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
        error_die("bind");
    if (*port == 0)  /* if dynamically allocating a port */
    {
        socklen_t namelen = sizeof(name);
        if (getsockname(httpd, (struct sockaddr *)&name, &namelen) == -1)
            error_die("getsockname");
        *port = ntohs(name.sin_port);   //函数使用指针，就是为了这里可以将动态分配的端口返回给函数外部
    }
    if (listen(httpd, 5) < 0)   //listen第二个参数为连接请求队列长度，5代表最多同时接受5个连接请求
        error_die("listen");
    return(httpd);
}

void accept_request(int client_sock)
{
    // cout<<"create accept thread success"<<endl;
    int client = client_sock;
    char buf[1024];
    size_t numchars;
    char method[255];
    char url[255];
    char path[512];
    size_t i, j;
    struct stat st;
    int cgi = 0;      /* becomes true if server decides this is a CGI
                       * program */
    char *query_string = NULL;

    read(client_sock,(void*)buf,1024);
    string req(buf);
    HttpRequest request(req);
    auto header = request.get_header();
    cout<<"[GET REQUEST]: Host = "<<header.find("Host")->second<<endl;

    HttpResponse response(200);
    response.Content_Type = "text/html";
    string res_string = response.get_response();
    cout<<res_string<<endl;
    send(client,res_string.c_str(),strlen(res_string.c_str()),0);
    close(client);
}

int main(){
    int server_sock = -1;
    u_short port = 9999;
    int client_sock = -1;
    struct sockaddr_in client_name;
    socklen_t  client_name_len = sizeof(client_name);
    pthread_t newthread;

    server_sock = startup(&port);
    printf("httpd running on port %d\n", port);

    while (1)
    {
        //accept函数用来保存请求客户端的地址相关信息
        client_sock = accept(server_sock,
                (struct sockaddr *)&client_name,
                &client_name_len);
        if (client_sock == -1)
            error_die("accept");
        /* accept_request(&client_sock); */
        cout<<"create accept thread,client sock_id="<<client_sock<<endl;
        thread accept_thread(accept_request,client_sock);
        accept_thread.join();
    }

    close(server_sock);
}