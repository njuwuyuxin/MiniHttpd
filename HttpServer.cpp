#include "HttpServer.h"

HttpServer::HttpServer(){
    baseURL = "/home/wuyuixn/Webroot";
    port=0;
    startup();
}

HttpServer::HttpServer(u_short p){
    baseURL = "/home/wuyuixn/Webroot";
    port=p;
    startup();
}

void HttpServer::startup(){
    int on = 1;
    struct sockaddr_in name;

    server_sock = socket(PF_INET, SOCK_STREAM, 0);    //使用TCP协议
    if (server_sock == -1)
        cerr<<"[ERROR]: create socket failed"<<endl;
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)  
    {  
        cerr<< "[ERROR]: setsockopt failed"<<endl;
    }
    if (bind(server_sock, (struct sockaddr *)&name, sizeof(name)) < 0)
        cerr<<"[ERROR]: bind failed"<<endl;

    if (port == 0)  /* if dynamically allocating a port */
    {
        socklen_t namelen = sizeof(name);
        if (getsockname(server_sock, (struct sockaddr *)&name, &namelen) == -1)
            cerr<<"[ERROR]: getsockname failed"<<endl;
        port = ntohs(name.sin_port);   //函数使用指针，就是为了这里可以将动态分配的端口返回给函数外部
    }
    if (listen(server_sock, 5) < 0)   //listen第二个参数为连接请求队列长度，5代表最多同时接受5个连接请求
        cerr<<"[ERROR]: listen failed"<<endl;
}

void HttpServer::start_listen(){
    cout<<"httpd running on port "<<port<<endl;
    int client_sock = -1;
    struct sockaddr_in client_name;
    socklen_t  client_name_len = sizeof(client_name);
    pthread_t newthread;

    while (1)
    {
        //accept函数用来保存请求客户端的地址相关信息
        client_sock = accept(server_sock,
                (struct sockaddr *)&client_name,
                &client_name_len);
        if (client_sock == -1)
            cerr<<"[ERROR]: accept failed"<<endl;

        thread accept_thread(accept_request,client_sock,this);
        accept_thread.join();
    }

    close(server_sock);
}

void HttpServer::accept_request(int client_sock, HttpServer* t)
{
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
    cout<<"url: "<<request.get_url()<<endl;
    string req_url = t->baseURL + request.get_url();
    
    auto header = request.get_header();
    cout<<"[GET REQUEST]: Host = "<<header.find("Host")->second<<endl;

    HttpResponse response(200);
    response.load_from_file(req_url);
    response.Content_Type = "text/html";
    string res_string = response.get_response();
    // cout<<res_string<<endl;
    send(client,res_string.c_str(),strlen(res_string.c_str()),0);
    close(client);
}