#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"

#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888


int main()
{
    ReverseProxy reverse_proxy;
    reverse_proxy.startHandleRequests();

    return 0;
}
