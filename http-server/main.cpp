#include <iostream>
#include "http-server/httpserver.hpp"

Response greeting(Request req) {
    Response a("HTTP/1.0", 200, "hey");
    return a;
}

int main() {
    HTTPServer* server = new HTTPServer(8080, 4);
    server->addHandler("GET", "/hey", greeting);
    server->run();
    return 0;
}