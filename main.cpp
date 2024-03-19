#include <iostream>

#include "BlockIP.h"
#include  "ReverseProxy.h"
#include "checkIP.h"

#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888


int main()
{
    //checkIP::CheckIP("43.131.241.36");
    //std::cout << checkIP::CheckIP("8.8.8.8") << std::endl;
    //checkIP::CheckIP("8.8.8.8");
    BlockIP::blockIP(0x2B83F124);
    //BlockIP::blockIP(0x7f000001);
    //BlockIP::unblockIP(0x7f000001);
    //boost::asio::io_context io_context;
    //boost::asio::ip::tcp::socket socket(io_context);
    //ReverseProxy reverse_proxy(io_context, PROXY_PORT, IP, SERVER_PORT);
    //io_context.run();
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
