
#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"
#include "Port_Scanning_Detector.h"


#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define EBPF_BANDWIDTH_NAME "bandwidth_limiting"


int main()
{

    ReverseProxy reverse_proxy;
    reverse_proxy.startHandleRequests();
    //std::thread reverseProxyThread(&ReverseProxy::startHandleRequests, &reverse_proxy);


    // Create an instance of Port_Scanning_Detector
    //std::thread detectorThread(&Port_Scanning_Detector::ListenForSYNScanAttack, &detector);


    //reverseProxyThread.detach();
    //detectorThread.detach();


    return 0;
}



