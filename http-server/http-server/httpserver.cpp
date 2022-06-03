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
        unsigned int clientAddressLength;
        int clientFd = accept(serverFd, (struct sockaddr *)&clientAddress, &clientAddressLength);
     
        struct in_addr clientAddr = clientAddress.sin_addr;
        int clientPort = ntohs(clientAddress.sin_port);
        std::cout << "Addr: " << clientAddr.s_addr << std::endl;

        // thread pool
        boost::asio::post(thread_pool, std::bind(request_process, clientFd, *handlers));
    }
    thread_pool.wait();
}


void HTTPServer::addHandler(const std::string &method, const std::string & path, const std::function<Response(Request)> &f){
    handlers->insert({std::pair<std::string, std::string>(method, path), f});
}


void request_process(int clientFd, std::map< std::pair<std::string,std::string>, std::function<Response(Request)>> handlers) {
    // read from socket to buffer
    int buffer_size = 100;
    char buff[buffer_size];
    ssize_t receivedBytes = recv(clientFd, (void *) buff, buffer_size, 0);

    if (receivedBytes < 0)
    {
        std::cerr << "Could not read from client. Error: " << strerror(errno) << std::endl;
        close(clientFd);
    }

    std::cout << "Got request: " << std::endl;

    // make a request from string
    std::string buff_str(buff, receivedBytes);;
    std::cout << buff_str << std::endl;
    
    Request request = parse_request(buff_str);

    // pass it to maps function
    std::function<Response(Request)> f = handlers[{request.get_method(), request.get_path()}];
    Response response;
    
    if (f == NULL){
        response = Response(request.get_html_ver(), 404, "");
    }
    else {
        response = f(request);
        std::cout << "Got function from handlers" << std::endl;    
    }

    std::string send_buff_str = response.get_string();
    char* send_buff = (char*) send_buff_str.c_str();

    // send request
    ssize_t sentBytes = send(clientFd, (void *) send_buff, send_buff_str.size(), 0);
    if (sentBytes < 0)
    {
        std::cerr << "Could not write to client. Error: " << errno << std::endl;
        close(clientFd);
    }

    std::cout << "Sent response: " << std::endl;
    std::cout << send_buff << std::endl;
    
    // close connection
    close(clientFd);
    std::cout << "Connection closed" << std::endl;
}


Request parse_request(std::string request_str) {
    // parse and make request object
    Request request = Request("GET", "/hey", "HTTP/1.0", "");
    return request;
}
