#include "httpserver.hpp"


HTTPServer::HTTPServer(int _port, size_t _threads_num) : 
    port(_port), threads_num(_threads_num), 
    handlers(std::make_unique< std::map< std::pair<std::string,std::string>, std::function<Response(Request)> >>()) 
    {}

int HTTPServer::run(){
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (serverFd < 0)
    {
        std::cerr << "Error while creating a socket due to error "<< strerror(errno) << std::endl;
        return errno;
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
    }
}

int HTTPServer::addHandler(std::string method, const std::string & path, std::function<Response(Request)>){

}









// int number;
// ssize_t receivedBytes = recv(clientFd, (void *)&number, sizeof(number), 0);

// if (receivedBytes < 0)
// {
//     std::cerr << "Could not read from client. Error: " << strerror(errno) << std::endl;
//     close(clientFd);
//     continue;
// }

// std::cout << "Got number " << number << " from client" << std::endl;

// int pow = number * number;

// ssize_t sentBytes = send(clientFd, (const void *)&pow, sizeof(pow), 0);

// if (sentBytes < 0)
// {
//     std::cerr << "Could not write to client. Error: " << errno << std::endl;
//     close(clientFd);
//     continue;
// }
// std::cout << "Sent power of given number " << number << " = " << pow << std::endl;
// close(clientFd);