#include "httpserver.hpp"


Request parse_request(std::string request_str);
void request_process(int clientFd, std::map< std::pair<std::string,std::string>, std::function<Response(Request)>> handlers);

HTTPServer::HTTPServer(int _port, size_t _threads_num) : 
    port(_port), threads_num(_threads_num), 
    handlers(std::make_unique< std::map< std::pair<std::string,std::string>, std::function<Response(Request)> >>()) 
    {}

void HTTPServer::run(){
    boost::asio::thread_pool thread_pool(threads_num);

    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (serverFd < 0)
    {
        std::cerr << "Error while creating a socket due to error "<< strerror(errno) << std::endl;
        exit(errno);
    }
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    int bound = bind(serverFd, (const struct sockaddr *)&address, sizeof(address));

    if (bound < 0)
    {
        std::cerr << "Could not bind to given port" << std::endl;
        exit(errno);
    }
    
    int listening = listen(serverFd, 1024);

    if (listening < 0)
    {
        std::cerr << "Could not start listening" << std::endl;
        exit(errno);
    }

    while (true)
    {
        sockaddr_in clientAddress;
        // are we supposed to assign int
        unsigned int clientAddressLength = 0;
        int clientFd = accept(serverFd, (struct sockaddr *)&clientAddress, &clientAddressLength);
     
        struct in_addr clientAddr = clientAddress.sin_addr;
        int clientPort = ntohs(clientAddress.sin_port);
        std::cout << "Addr: " << clientAddr.s_addr << std::endl;

        // thread pool
        boost::asio::post(thread_pool, std::bind(request_process, clientFd, *handlers));
    }
}

void HTTPServer::addHandler(const std::string &method, const std::string & path, const std::function<Response(Request)> &f){
    handlers->insert({std::pair<std::string, std::string>(method, path), f});
}


void request_process(int clientFd, std::map< std::pair<std::string,std::string>, std::function<Response(Request)>> handlers) {
    // it will read from socket to buffer
    
    char buff[100];
    ssize_t receivedBytes = recv(clientFd, (void *)&buff, sizeof(buff), 0);

    if (receivedBytes < 0)
    {
        std::cerr << "Could not read from client. Error: " << strerror(errno) << std::endl;
        close(clientFd);
    }

    std::cout << "Got request" << std::endl;

    // make a request from string
    std::string buff_str(buff);
    Request request = parse_request(buff_str);

    std::function<Response(Request)> f = handlers[{request.get_method(), request.get_path()}];
    
    // pass it to maps function
    Response response = f(request);
    std::string send_buff_str = response.get_string();

    // char[] send_buff = send_buff_str.c_str();

    // send request
    ssize_t sentBytes = send(clientFd, (const void *)&send_buff_str, sizeof(send_buff_str), 0);
    if (sentBytes < 0)
    {
        std::cerr << "Could not write to client. Error: " << errno << std::endl;
        close(clientFd);
    }
    std::cout << "Sent response " << std::endl;

    // close connection
    close(clientFd);
}


Request parse_request(std::string request_str) {
    // parse and make request object
    Request request = Request("POST", "/", "HTTP 1.0", "heyyy");
    return request;
}
