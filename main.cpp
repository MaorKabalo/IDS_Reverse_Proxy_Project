#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"

#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

int main()
{
    boost::asio::io_context io_context;

    // short int server_port, proxy_port;
    // std::string ip;
    // std::cout<<"enter server ip";
    // std::cin>>ip;
    // std::cout<<"enter server port";
    // std::cin>>server_port;
    // std::cout<<"enter proxy port";
    // std::cin>>proxy_port;

    ReverseProxy reverse_proxy(io_context, PROXY_PORT, IP, SERVER_PORT);
    io_context.run();
    return 0;
}
