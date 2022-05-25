#include <iostream>
#include "http-server/httpserver.hpp"

int main() {
    HTTPServer* server = new HTTPServer(8080, 4);
    // server->addHandler();
    // server->addPathHandler("GET", "skfd;lk", function);
    // server->run();

    return 0;
}