
#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"
#include "Port_Scanning_Detector.h"

#include "IPTools/checkIP.h"
#include "IPTools/BlockIP.h"
#include "IPTools/IPTools.h"

#define SUS_IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define EBPF_BANDWIDTH_NAME "bandwidth_limit"
#define EBPF_RATE_LIMITING_NAME "rate_limiting"


int main()
{

    ReverseProxy reverse_proxy;
    reverse_proxy.startHandleRequests();

    //BlockIP::blockIP(SUS_IP);
    // ReverseProxy reverse_proxy;
    // reverse_proxy.startHandleRequests();




    // EBPF_Runner rate_limiter(EBPF_RATE_LIMITING_NAME);
    //
    // if(rate_limiter.compileAndRunEBPFProgram()) {
    //     ReverseProxy reverse_proxy;
    //     reverse_proxy.startHandleRequests();
    // }


    // EBPF_Runner bandwith_limiter(EBPF_BANDWIDTH_NAME);
    //
    //
    // if(bandwith_limiter.compileAndRunEBPFProgram()) {
    //     ReverseProxy reverse_proxy;
    //     reverse_proxy.startHandleRequests();
    // }

    //checkIP::CheckIP("43.131.241.36");

    // BlockIP::blockIP(0x2b83f124);




    return 0;
}



