#include <string>

class Response {
    std::string method;
    int code;    
public:
    Response(std::string response_str);
    std::string get_string();

};