#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <map>
#include <memory>

#include "response.hpp"
#include "request.hpp"
#include <functional>

#include <sys/socket.h>
#include <sys/types.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

#include <functional>

#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>


class HTTPServer {

    int port;
    size_t threads_num;
    std::unique_ptr< std::map< std::pair<std::string,std::string>, std::function<Response(Request)> >> handlers;

public:
    HTTPServer(int _port, size_t _threads_num);
    void run();
    void addHandler(const std::string &method, const std::string & path, const std::function<Response(Request)> &f);
    
};

#endif