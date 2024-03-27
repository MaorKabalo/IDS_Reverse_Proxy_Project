//
// Created by kali on 2/4/24.
//

#include "BlockIP.h"

// Define the map structure
struct ip_block {
    __u32 ip;
    __u32 mask;
};

int BlockIP::map_id;
bool BlockIP::isActive = false;




bool BlockIP::blockIP(const char* ip) {

    const __uint32_t ipInt = ReverseIP(IPv4StringToInt(ip));

    if(!isActive) {
        EBPF_Runner runner("IpBlock");
        isActive = true;

        // map_id = bpf_map_create(BPF_MAP_TYPE_HASH, "my_map", sizeof(__u32), sizeof(struct ip_block), 1024, nullptr);
        // if (map_id < 0) {
        //     std::cerr << "Failed to create BPF map: " << std::strerror(errno) << std::endl;
        //     return false;
        // }


        if (!runner.compileAndRunEBPFProgram()) {
            std::cerr << "Failed to compile and run eBPF program." << std::endl;
            return false;
        }
    }

    map_id = bpf_obj_get("/sys/fs/bpf/tc/globals/ip_block_map");

    struct ip_block block = {ipInt, 0xFFFFFFFF};

    int result = bpf_map_update_elem(map_id, &ipInt, &block, BPF_ANY);
    if (result < 0) {
        std::cerr << "Error: Failed to update BPF map: " << std::strerror(errno) << std::endl;
        return false;
    }

    std::cout << "IP address " << ipInt << " is blocked." << std::endl;

    return true;
}

#include <linux/bpf.h> // Include BPF headers

bool BlockIP::unblockIP(const char* ip) {
    // Assuming 'map_fd' contains the file descriptor of the BPF map


    const __uint32_t ipInt = ReverseIP(IPv4StringToInt(ip));
    // Prepare the key to delete from the map
    __u32 key = ipInt;

    // Delete the key from the BPF map
    int result = bpf_map_delete_elem(map_id, &key);
    if (result < 0) {
        std::cerr << "Error: Failed to delete IP address from the BPF map: " << std::strerror(errno) << std::endl;
        return false;
    }

    std::cout << "IP address " << ipInt << " is unblocked." << std::endl;

    return true;
}

