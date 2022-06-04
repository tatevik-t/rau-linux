#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <map>


class Request {
    
    std::string method;
    std::string html_ver;
    std::string path;
    std::map<std::string, std::string> headers;
    std::string body;

public:

    Request(std::string _method, std::string _path, std::string _html_ver, std::string _body, std::map<std::string, std::string> _headers = {}) : 
        method(_method),
        html_ver(_html_ver),
        path(_path),
        headers(_headers),
        body(_body) 
        {}

    std::string get_body() {
        return body;
    }
        std::string get_html_ver() {
        return html_ver;
    }
    std::string get_path() {
        return path;
    }
    std::map<std::string, std::string> get_headers() {
        return headers;
    }
    std::string get_method() {
        return method;
    }

    void add_body(std::string _body) {
        body = _body;
    }
};


#endif