#include "HttpServer.h"

HttpServer::HttpServer(){
    HttpResponse::init_content_type_map();
    load_config("../Minihttpdconf.cfg");
    init_controller_map();
    startup();
}

HttpServer::~HttpServer(){
    for(auto i:controller_map){
        delete i.second;
    }
}

void HttpServer::init_controller_map(){
    //在此注册的所有controller不要手动释放，在server对象消亡时析构函数会自动释放所有controller
    BasicController* hello_controller = new HelloController();
    BasicController* hello_controller2 = new HelloController2();
    controller_map.insert(pair<string,BasicController*>("/api",hello_controller));
    controller_map.insert(pair<string,BasicController*>("/api2",hello_controller2));
}

void HttpServer::load_config(string path){
    Log::log("Loading config",INFO);
    Config config;
    try{
        config.readFile(path.c_str());
        int i_port;
        if(!config.lookupValue("server.port",i_port)){
            Log::log("server port setting not found, use default setting",WARN);
            port = 0;
        }
        else{
            port = i_port;
        }
        if(!config.lookupValue("server.root",baseURL)){
            Log::log("server root setting not found, use default setting",WARN);
            baseURL = "/home/wuyuixn/Webroot";
        }
        if(!config.lookupValue("server.index",index)){
            Log::log("server index setting not found, use default setting",WARN);
            index = "index.html";
        }
        if(!config.lookupValue("server.max_request",request_queue_length)){
            Log::log("server max_request setting not found, use default setting",WARN);
            request_queue_length = 5;
        }
    }
    catch(FileIOException io_exception){
        cout<<"Config not found, use default settings"<<endl;
        port = 0;
        baseURL = "/home/wuyuixn/Webroot";
        request_queue_length = 5;
    }
    catch(SettingNotFoundException set_not_found){
        Log::log("settings not found",ERROR);
    }
}

void HttpServer::startup(){
    int on = 1;
    struct sockaddr_in name;

    server_sock = socket(PF_INET, SOCK_STREAM, 0);    //使用TCP协议
    if (server_sock == -1)
        Log::log("create socket failed",ERROR);
    memset(&name, 0, sizeof(name));
    name.sin_family = AF_INET;
    name.sin_port = htons(port);
    name.sin_addr.s_addr = htonl(INADDR_ANY);
    if ((setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)  {  
        Log::log("setsockopt failed",ERROR);
    }
    if (bind(server_sock, (struct sockaddr *)&name, sizeof(name)) < 0){
        Log::log("bind failed",ERROR);
    }

    if (port == 0){  //动态分配端口
        socklen_t namelen = sizeof(name);
        if (getsockname(server_sock, (struct sockaddr *)&name, &namelen) == -1)
            Log::log("getsockname failed",ERROR);
        port = ntohs(name.sin_port);
    }
    if (listen(server_sock, request_queue_length) < 0)   //listen第二个参数为连接请求队列长度，代表最多同时接受n个连接请求
        Log::log("listen failed",ERROR);
}

void HttpServer::start_listen(){
    stringstream ss;
    string s_port;
    ss<<port;
    ss>>s_port;
    Log::log("Minihttpd running on port "+s_port,INFO);

    epollfd = epoll_create(5);
    add_epoll_fd(server_sock);      //把监听socket加入内核事件表

    epoll_event events[MAX_EVENT_NUMBER];
    while(1){
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        for (int i = 0; i < number; i++){
            int sockfd = events[i].data.fd;

            //处理新到的客户连接
            if (sockfd == server_sock){
                int client_sock = -1;
                struct sockaddr_in client_name;
                socklen_t  client_name_len = sizeof(client_name);
                client_sock = accept(server_sock,
                        (struct sockaddr *)&client_name,
                        &client_name_len);
                if (client_sock == -1){
                    Log::log("accept failed",ERROR);
                    continue;
                }
                add_epoll_fd(client_sock);      //把客户端socket加入内核事件表
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)){
                Log::log("epoll end",DEBUG);
                //服务器端关闭连接
            }
            //处理客户连接上接收到的数据
            else if (events[i].events & EPOLLIN){
                thread accept_thread(accept_request,sockfd,this);
                accept_thread.detach();
            }
            else if (events[i].events & EPOLLOUT){
                Log::log("epoll out",DEBUG);
            }
        }
    }

    close(server_sock);
}

void HttpServer::accept_request(int client_sock, HttpServer* t)
{
    int client = client_sock;
    char buf[1024];
    read(client_sock,(void*)buf,1024);
    string req(buf);
    HttpRequest request(req);

    HttpResponse response(200);

    //根据匹配情况区分文件请求与url请求
    BasicController* match_controller = t->match_url(request);
    if(match_controller != NULL){
        response = match_controller->Accept(request);
    }
    else{
        Log::log("request file",DEBUG);
        response = t->file_request(request);
    }

    send(client,response.get_response(),response.get_response_size(),0);
    close(client);
}

HttpResponse HttpServer::file_request(HttpRequest& request){
    string url = request.get_url();
    Log::log("request url = "+url,INFO);
    HttpResponse response(200);

    string req_url;
    if(url=="/")
        req_url = baseURL + '/' + index;
    else
        req_url = baseURL + request.get_url();

    auto header = request.get_header();
    Log::log("[GET REQUEST]: Host = "+header.find("Host")->second,INFO);

    //判断是否使用gzip压缩格式
    auto encode_iter = header.find("Accept-Encoding");
    if(encode_iter!=header.end()){
        auto list = splitString(encode_iter->second,",",false);
        for(auto &i:list){
            if(i[0]==' ')
                i.erase(i.begin());
            if(i=="gzip"){
                response.Content_Encoding = "gzip";
            }
        }
    }

    response.load_from_file(req_url);
    response.generate_response();
    return response;
}

BasicController* HttpServer::match_url(HttpRequest& request){
    string url = request.get_url();
    for(auto i:controller_map){
        std::regex route_path("^"+i.first+"/");
        std::cmatch m;
        bool is_servlet = std::regex_search(url.c_str(),m,route_path);
        if(is_servlet){
            Log::log("match route: "+i.first,DEBUG);
            // Log::log("suffix: "+m.suffix().str(),DEBUG);
            request.set_url("/" + m.suffix().str());
            return i.second;
        }
    }
    return NULL;
}

void HttpServer::add_epoll_fd(int event_fd){
    epoll_event event;
    event.data.fd = event_fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, event_fd, &event);
}