
#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"
#include "Port_Scanning_Detector.h"


#define IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define EBPF_BANDWIDTH_NAME "bandwidth_limiting"
#define INTERFACE_FOR_PORT_SCANNING "lo"

int main()
{

    const Port_Scanning_Detector detector(INTERFACE_FOR_PORT_SCANNING);



    detector.ListenForSYNScanAttack();
    //detector.startListening();


    std::cout << "HELLO" << std::endl;

    return 0;
}



