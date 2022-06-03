#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <map>


class Response {

    std::string html_ver;
    int status_code;
    std::map<std::string, std::string> headers;
    std::string body;

public:
    Response(){};
    Response(std::string _html_ver, int _status_code, std::string _body, std::map<std::string, std::string> _headers) : 
        html_ver(_html_ver), 
        status_code(_status_code),
        body(_body),
        headers(_headers)
        {}

    Response(std::string _html_ver, int _status_code, std::string _body) : 
        html_ver(_html_ver), 
        status_code(_status_code),
        body(_body)
        {
            headers.insert({"Content-Length", std::to_string(_body.size())});
        }

    std::string get_string() {
        std::string response_str = "";
        response_str += html_ver + ' ';
        response_str += std::to_string(status_code) + '\n';
        for(auto header : headers) {
            response_str += header.first + ": " + header.second + '\n';
        }
        response_str += '\n';
        if (body.size() != 0) {
            response_str += body; 
        }
        return response_str;
    }
};


#endif