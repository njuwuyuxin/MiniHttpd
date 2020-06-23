#include "HttpResponse.h"

string simplePage(){
    string page;
    page += "<html>";
    page += "<head>";
    page += "<title>Simple page</title>";
    page += "</head>";
    page += "<body>";
    page += "<p> this is a simple page </p>";
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

void HttpResponse::set_header(string key, string val){
    custom_header.insert(pair<string,string>(key,val));
}

string HttpResponse::get_response(){
    string response;
    response += generate_header();
    response += simplePage();
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