
#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"
#include "Port_Scanning_Detector.h"


#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define EBPF_BANDWIDTH_NAME "bandwidth_limiting"
#define EBPF_RATE_LIMITING_NAME "rate_limiting"


int main()
{

    // EBPF_Runner rate_limiter(EBPF_RATE_LIMITING_NAME);
    //
    // if(rate_limiter.compileAndRunEBPFProgram()) {
    //     ReverseProxy reverse_proxy;
    //     reverse_proxy.startHandleRequests();
    // }


    EBPF_Runner bandwith_limiter(EBPF_BANDWIDTH_NAME);


    if(bandwith_limiter.compileAndRunEBPFProgram()) {
        ReverseProxy reverse_proxy;
        reverse_proxy.startHandleRequests();
    }




    return 0;
}



