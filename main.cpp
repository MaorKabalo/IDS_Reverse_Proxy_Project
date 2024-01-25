#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"

#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define EBPF_BANDWIDTH_NAME "bandwidth_limiting"


int main()
{

    //EBPF_Runner bandwidth_limiter(EBPF_BANDWIDTH_NAME);

    //if(bandwidth_limiter.compileAndRunEBPFProgram()) {
    //    ReverseProxy reverse_proxy;
    //    reverse_proxy.startHandleRequests();
    //}
    ReverseProxy reverse_proxy;
    reverse_proxy.startHandleRequests();


;

    return 0;
}
