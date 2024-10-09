
#include <iostream>
#include  "ReverseProxy.h"
#include "EBPF_Runner.h"
#include "Port_Scanning_Detector.h"
#include "IPTools/checkIP.h"
#include "IPTools/BlockIP.h"
#include "IPTools/IPTools.h"
#include <linux/bpf.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <fstream>

#define SUS_IP "127.0.0.1"
#define PROXY_PORT 9090
#define SERVER_PORT 8888

#define EBPF_BANDWIDTH_NAME "bandwidth_limit"
#define EBPF_RATE_LIMITING_NAME "rate_limiting"
#define EBPF_PORT_SCANNING_DETECTION_NAME "port_scan_dt"
#define EBPF_IP_BLOCK_NAME "IpBlock"


struct each_port {
    __u32 port;
};

//sudo bpftool map dump name const_nmap_port

bool add_ports_to_ebpf_array_map() {
    // Get the map ID
    int map_id = bpf_obj_get("/sys/fs/bpf/tc/globals/const_nmap_ports");
    if (map_id < 0) {
        std::cerr << "Error: Failed to get BPF map ID: " << std::strerror(errno) << std::endl;
        return false;
    }

    __u32 key = 0;

    // Open the ports file
    std::ifstream ports_file("nmapDefault.txt");
    if (!ports_file) {
        std::cerr << "Error: Could not open the ports file." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(ports_file, line)) {
        __u32 current_port = std::stoi(line);  // Convert string to integer
        //std::cout << "Adding port: " << current_port << std::endl;

        struct each_port port = {current_port};  // Initialize struct with current_port

        // Update the map with the current key
        int result = bpf_map_update_elem(map_id, &key, &port, BPF_ANY);
        if (result < 0) {
            std::cerr << "Error: Failed to update BPF map at key " << key
                      << ": " << std::strerror(errno) << std::endl;
            return false;
        }

        key++;
    }

    return true;
}





int main()
{






    EBPF_Runner port_scanner_detector(EBPF_PORT_SCANNING_DETECTION_NAME);
    if(port_scanner_detector.compileAndRunEBPFProgram()) {
        add_ports_to_ebpf_array_map();
     }




//    ReverseProxy reverse_proxy;
//    reverse_proxy.startHandleRequests();

    //BlockIP::blockIP(SUS_IP);
    // ReverseProxy reverse_proxy;
    // reverse_proxy.startHandleRequests();




//     EBPF_Runner rate_limiter(EBPF_RATE_LIMITING_NAME);
//     if(rate_limiter.compileAndRunEBPFProgram()) {
//         ReverseProxy reverse_proxy;
//         reverse_proxy.startHandleRequests();
//     }
//

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



