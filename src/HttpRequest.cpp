#include "HttpRequest.h"

//最后参数为重复标志，即分隔符连续重复出现是否视为一个
std::vector<std::string> splitString(std::string srcStr, std::string delimStr,bool repeatedCharIgnored)
{
    std::vector<std::string> resultStringVector;
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c){if(delimStr.find(c)!=std::string::npos){return true;}else{return false;}}/*pred*/, delimStr.at(0));//将出现的所有分隔符都替换成为一个相同的字符（分隔符字符串的第一个）
    size_t pos=srcStr.find(delimStr.at(0));
    std::string addedString="";
    while (pos!=std::string::npos) {
        addedString=srcStr.substr(0,pos);
        if (!addedString.empty()||!repeatedCharIgnored) {
            resultStringVector.push_back(addedString);
        }
        srcStr.erase(srcStr.begin(), srcStr.begin()+pos+1);
        pos=srcStr.find(delimStr.at(0));
    }
    addedString=srcStr;
    if (!addedString.empty()||!repeatedCharIgnored) {
        resultStringVector.push_back(addedString);
    }
    return resultStringVector;
}

HttpRequest::HttpRequest(string raw_data){
    vector<string> lines = splitString(raw_data,"\n",false);

    vector<string> first_line = splitString(lines[0]," ",false);
    if(first_line.size()==3){
        method = first_line[0];
        url = first_line[1];
        version = first_line[2];
    }
    else{
        Log::log("bad http request when get method",WARN);
    }

    for(size_t i = 1;i<lines.size();i++){
        if(lines[i]==""||lines[i]=="\r"){
            break;
        }
        size_t pos = lines[i].find_first_of(':');
        if(pos==string::npos){
            cerr<<"bad request"<<endl;
            cout<<"this lines ascii is"<<endl;
            for(auto j:lines[i]){
                cout<<(int)(j)<<" ";
            }
            cout<<endl;
            break;
        }
        string key = lines[i].substr(0,pos);
        string val;
        //去除val前可能存在的空格
        if(lines[i][pos+1]==' '){
            val = lines[i].substr(pos+2);
        }
        else{
            val = lines[i].substr(pos+1);
        };
        header.insert(pair<string,string>(key,val));
    }
    resolve_get_params();
}

void HttpRequest::resolve_get_params(){
    size_t pos = url.find("?");
    if(pos == string::npos)
        return;
    else{
        string raw_params = url.substr(pos+1);
        Log::log("raw_params:"+raw_params,DEBUG);
        vector<string> param_list = splitString(raw_params,"&",true);
        for(auto i:param_list){
            vector<string> key_val = splitString(i,"=",false);
            if(key_val.size()!=2){
                Log::log("GET param format error",WARN);
                continue;
            }
            else{
                params.insert(pair<string,string>(key_val[0],key_val[1]));
            }
        }
    }

    //debug
    for(auto i:params)
        cout<<i.first<<" "<<i.second<<endl;
}