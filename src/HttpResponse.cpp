#include "HttpResponse.h"

map<string,string> HttpResponse::content_type_map;

string simplePage(){
    string page;
    page += "<html>";
    page += "<head>";
    page += "<title>404 page not found</title>";
    page += "</head>";
    page += "<body>";
    page += "<h2> 404 Page Not Found </h2>";
    page += "</body>";
    page += "</html>";
    return page;
}

string GetGmtTime()
{
    time_t rawTime;
    struct tm* timeInfo;
    char szTemp[30]={0};
    time(&rawTime);
    timeInfo = gmtime(&rawTime);
    strftime(szTemp,sizeof(szTemp),"%a, %d %b %Y %H:%M:%S GMT",timeInfo);
    string GmtTime(szTemp);
 
    return GmtTime;
}

HttpResponse::HttpResponse(int st){
    version = "HTTP/1.1";
    status = to_string(st);
    date = GetGmtTime();
    server = "Minihttpd";
    raw_response = NULL;
}

HttpResponse::HttpResponse(const HttpResponse& resp){
    Allow = resp.Allow;
    Content_Encoding = resp.Content_Encoding;
    Content_Length = resp.Content_Length;
    Content_Type = resp.Content_Type;
    Expires = resp.Expires;
    Last_Modified = resp.Last_Modified;
    Location = resp.Location;
    Refresh = resp.Refresh;
    Set_Cookie = resp.Set_Cookie;
    WWW_Authenticate = resp.WWW_Authenticate;

    version = resp.version;
    status = resp.status;
    date = resp.date;
    server = resp.server;
    custom_header = resp.custom_header;
    response_body = resp.response_body;
    raw_response_size = resp.raw_response_size;

    raw_response = new char[raw_response_size];
    memcpy(raw_response,resp.raw_response,raw_response_size);
}

HttpResponse& HttpResponse::operator=(const HttpResponse& resp){
    Allow = resp.Allow;
    Content_Encoding = resp.Content_Encoding;
    Content_Length = resp.Content_Length;
    Content_Type = resp.Content_Type;
    Expires = resp.Expires;
    Last_Modified = resp.Last_Modified;
    Location = resp.Location;
    Refresh = resp.Refresh;
    Set_Cookie = resp.Set_Cookie;
    WWW_Authenticate = resp.WWW_Authenticate;

    version = resp.version;
    status = resp.status;
    date = resp.date;
    server = resp.server;
    custom_header = resp.custom_header;
    response_body = resp.response_body;
    raw_response_size = resp.raw_response_size;

    raw_response = new char[raw_response_size];
    memcpy(raw_response,resp.raw_response,raw_response_size);
    return *this;
}


HttpResponse::~HttpResponse(){
    if(raw_response!=NULL){
        delete raw_response;
    }
}

void HttpResponse::init_content_type_map(){
    content_type_map.insert(pair<string,string>("html","text/html"));
    content_type_map.insert(pair<string,string>("htm","text/html"));
    content_type_map.insert(pair<string,string>("shtml","text/html"));
    content_type_map.insert(pair<string,string>("css","text/css"));
    content_type_map.insert(pair<string,string>("js","text/javascript"));
    content_type_map.insert(pair<string,string>("txt","text/plain"));
    content_type_map.insert(pair<string,string>("js","text/javascript"));
    content_type_map.insert(pair<string,string>("xml","text/xml"));

    content_type_map.insert(pair<string,string>("ico","image/x-icon"));
    content_type_map.insert(pair<string,string>("jpg","image/jpeg"));
    content_type_map.insert(pair<string,string>("jpeg","image/jpeg"));
    content_type_map.insert(pair<string,string>("jpe","image/jpeg"));
    content_type_map.insert(pair<string,string>("gif","image/gif"));
    content_type_map.insert(pair<string,string>("png","image/png"));
    content_type_map.insert(pair<string,string>("tiff","image/tiff"));
    content_type_map.insert(pair<string,string>("tif","image/tiff"));
    content_type_map.insert(pair<string,string>("rgb","image/x-rgb"));

    content_type_map.insert(pair<string,string>("mpeg","video/mpeg"));
    content_type_map.insert(pair<string,string>("mpg","video/mpeg"));
    content_type_map.insert(pair<string,string>("mpe","video/mpeg"));
    content_type_map.insert(pair<string,string>("qt","video/quicktime"));
    content_type_map.insert(pair<string,string>("mov","video/quicktime"));
    content_type_map.insert(pair<string,string>("avi","video/x-msvideo"));
    content_type_map.insert(pair<string,string>("movie","video/x-sgi-movie"));

    content_type_map.insert(pair<string,string>("woff","application/font-woff"));
    content_type_map.insert(pair<string,string>("ttf","application/octet-stream"));
}

void HttpResponse::set_header(string key, string val){
    custom_header.insert(pair<string,string>(key,val));
}

void HttpResponse::load_from_file(string url){
    ifstream in_file(url);
    if(in_file.fail()){
        cerr<<"[404]: "<<url<<" not found"<<endl;
        status = "404";
        response_body=simplePage();
        return;
    }
    auto_set_content_type(url);
    stringstream buffer;
    buffer << in_file.rdbuf();
    response_body = buffer.str();
    in_file.close();
}

const char* HttpResponse::get_response(){
    return raw_response;
}

unsigned int HttpResponse::get_response_size(){
    return raw_response_size;
}

void HttpResponse::auto_set_content_type(string url){
    size_t pos = url.find_last_of('.');
    if(pos==string::npos){
        return;
    }
    string ext = url.substr(pos+1);
    cout<<"[DEBUG]: file ext="<<ext<<endl;
    auto iter = content_type_map.find(ext);
    if(iter!=content_type_map.end()){
        Content_Type = iter->second;
    }
}

//raw_data为原始数据，buffer为压缩后数据存储缓冲区，buffer_size为缓冲区大小，返回值为压缩后数据字节数
uLong gzip_compress(string raw_data,Bytef*& buffer,int buffer_size){
    size_t raw_data_size = raw_data.size();
    z_stream strm;
    z_stream d_stream;
    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = NULL;
    d_stream.next_in = (Bytef*)raw_data.c_str();
    d_stream.avail_in = raw_data_size;
    d_stream.next_out = buffer;
    d_stream.avail_out = buffer_size;

    int ret = deflateInit2(&d_stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
						MAX_WBITS + 16, 8, Z_DEFAULT_STRATEGY);
    if (Z_OK != ret)
    {
        cerr<<"[ERROR]: init deflate error"<<endl;
        cout<< ret <<endl;
    }

    int err = 0;
    int flag = 0;
    for(;;) {
        if((err = deflate(&d_stream, Z_FINISH)) == Z_STREAM_END) break;
        if(flag > 3){
            cerr<<"[ERROR]: deflate failed,errNo = "<<err<<endl;
            return 0;
        }

        //输出缓冲区不足，尝试扩容，最多三次扩容失败则放弃压缩
        if(err == Z_BUF_ERROR){
            flag++;
            delete buffer;
            buffer_size = buffer_size*1.5;
            buffer = new Bytef[buffer_size];
            d_stream.next_out = buffer;
            d_stream.avail_out = buffer_size;
            cout<<"[WARN]: deflate buffer error,try larger buffer :"<<flag<<endl;
        }
    }
    if(deflateEnd(&d_stream) != Z_OK){
        cerr<<"[ERROR]: deflate failed when end"<<endl;
    }
    return d_stream.total_out;
}

//生成完整响应，其中判断响应体是否压缩处理
void HttpResponse::generate_response(){
    string header(generate_header());

    //无压缩模式
    if(Content_Encoding.size()==0){
        cout<<"[DEBUG]: not gzip"<<endl;
        string response = header + response_body;
        raw_response = new char[response.size()];
        if(raw_response==NULL){
            cout<<"[ERROR]: In HttpResponse.cpp: raw_response is NULL!!!"<<endl;
        }
        memcpy(raw_response,response.c_str(),response.size());
        raw_response_size = response.size();
        return ;
    }
    //gzip压缩
    else{
        Bytef* buffer = new Bytef[response_body.size()];
        uLong out_size = gzip_compress(response_body,buffer,response_body.size());
        //压缩失败处理
        if(out_size == 0){
            cout<<"[WARN]: gzip compress failed, use raw data"<<endl;
            string().swap(Content_Encoding);    //去除编码字段
            header = generate_header();         //重新生成header    
            memcpy(buffer,response_body.c_str(),response_body.size());
            out_size = response_body.size();
        }
        //响应头和响应体拼接
        size_t header_size = header.size();
        raw_response = new char[header_size+out_size];
        memcpy(raw_response,header.c_str(),header_size);
        char* body_p = raw_response + header_size;
        memcpy(body_p,buffer,out_size);

        delete buffer;
        raw_response_size = header_size+out_size;
        return;
    }  
}

string HttpResponse::generate_header(){
    string header;
    header += (version + " ");
    header += (status + "\n");
    (header += "Date:") += (date + "\n");
    (header += "Server:") += (server + "\n");

    if(Allow.size()!=0)
        (header += "Allow:") += (Allow + "\n");
    if(Content_Encoding.size()!=0)
        (header += "Content-Encoding:") += (Content_Encoding + "\n");
    if(Content_Length.size()!=0)
        (header += "Content-Length:") += (Content_Length + "\n");
    if(Content_Type.size()!=0)
        (header += "Content-Type:") += (Content_Type + "\n");
    if(Expires.size()!=0)
        (header += "Expires:") += (Expires + "\n");
    if(Last_Modified.size()!=0)
        (header += "Last-Modified:") += (Last_Modified + "\n");
    if(Location.size()!=0)
        (header += "Location:") += (Location + "\n");
    if(Refresh.size()!=0)
        (header += "Refresh:") += (Refresh + "\n");
    if(Set_Cookie.size()!=0)
        (header += "Set-Cookie:") += (Set_Cookie + "\n");
    if(WWW_Authenticate.size()!=0)
        (header += "WWW-Authenticate:") += (WWW_Authenticate + "\n");
    
    
    //基本字段添加完成后，填充用户自定义字段
    for(auto i:custom_header){
        ((header += i.first) += ":") += i.second += "\n";
    }

    header += "\r\n";
    return header;
}