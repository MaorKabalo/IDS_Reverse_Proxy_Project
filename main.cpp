#include <iostream>
#include  "ReverseProxy.h"

int main()
{
    boost::asio::io_context io_context;
    short int server_port=8888, proxy_port=9090;
    std::string ip = "127.0.0.1";
    boost::asio::ip::tcp::socket socket(io_context);
    ReverseProxy reverse_proxy(io_context, proxy_port, ip, server_port);
    io_context.run();
    /*while (true)
    {
        if(io_context.stopped())
        {
            reverse_proxy.StartAccept();
            io_context.run();
        }
    }*/
    return 0;
}
