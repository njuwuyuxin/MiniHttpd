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
}

void HttpResponse::set_header(string key, string val){
    custom_header.insert(pair<string,string>(key,val));
}

void HttpResponse::load_from_file(string url){
    ifstream in_file(url);
    if(in_file.fail()){
        cerr<<"[404]: "<<url<<" not found"<<endl;
        response_body=simplePage();
        return;
    }
    auto_set_content_type(url);
    stringstream buffer;
    buffer << in_file.rdbuf();
    response_body = buffer.str();
    in_file.close();
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
    // if(ext=="html"||ext=="htm"){
    //     Content_Type = "text/html";
    // }
    // else if(ext=="css"){
    //     Content_Type = "text/css";
    // }
    // else if(ext=="js"){
    //     Content_Type = "text/javascript";
    // }
}

string HttpResponse::get_response(){
    string response;
    response += generate_header();
    response += response_body;
    return response;
}

string HttpResponse::generate_header(){
    string header;
    header += version += " ";
    header += status += "\n";
    (header += "Date:") += date += "\n";

    if(Allow.size()!=0)
        (header += "Allow:") += Allow += "\n";
    if(Content_Encoding.size()!=0)
        (header += "Content-Encoding:") += Content_Encoding += "\n";
    if(Content_Length.size()!=0)
        (header += "Content-Length:") += Content_Length += "\n";
    if(Content_Type.size()!=0)
        (header += "Content-Type:") += Content_Type += "\n";
    if(Expires.size()!=0)
        (header += "Expires:") += Expires += "\n";
    if(Last_Modified.size()!=0)
        (header += "Last-Modified:") += Last_Modified += "\n";
    if(Location.size()!=0)
        (header += "Location:") += Location += "\n";
    if(Refresh.size()!=0)
        (header += "Refresh:") += Refresh += "\n";
    if(Set_Cookie.size()!=0)
        (header += "Set-Cookie:") += Set_Cookie += "\n";
    if(WWW_Authenticate.size()!=0)
        (header += "WWW-Authenticate:") += WWW_Authenticate += "\n";
    
    //基本字段添加完成后，填充用户自定义字段
    for(auto i:custom_header){
        ((header += i.first) += ":") += i.second += "\n";
    }

    header += "\r\n";
    return header;
}