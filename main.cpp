#include <iostream>
#include  "ReverseProxy.h"

int main()
{
    boost::asio::io_context io_context;
    short int server_port, proxy_port;
    std::string ip;
    std::cout<<"enter server ip";
    std::cin>>ip;
    std::cout<<"enter server port";
    std::cin>>server_port;
    std::cout<<"enter proxy port";
    std::cin>>proxy_port;
    ReverseProxy reverse_proxy(io_context, proxy_port, ip, server_port);
    return 0;
}
