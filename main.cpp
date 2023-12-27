#include <iostream>
#include  "ReverseProxy.h"

#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888


int main()
{
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket(io_context);
    ReverseProxy reverse_proxy(io_context, PROXY_PORT, IP, SERVER_PORT);
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
