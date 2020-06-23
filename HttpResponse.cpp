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
    // strcpy_s(szGmtTime,strlen(szTemp)+1,szTemp);//必须这样，避免内存释放，内容没有的问题。 
 
    return GmtTime;
}

HttpResponse::HttpResponse(int st){
    version = "HTTP/1.1";
    status = to_string(st);
    date = GetGmtTime();
    server = "Minihttpd";
}

string HttpResponse::get_response(){
    string response;
    response += version += " ";
    response += status += "\n";
    (response += "Date:") += date += "\n";
    if(Content_Type.size()!=0)
        (response += "Content-Type:") += Content_Type += "\n";
    
    response += "\r\n";
    response += simplePage();
    return response;
}