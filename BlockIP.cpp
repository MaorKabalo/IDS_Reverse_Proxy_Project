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




bool BlockIP::blockIP(const char* ip) {
    EBPF_Runner runner("IpBlock");

    map_id = bpf_map_create(BPF_MAP_TYPE_HASH, "my_map", sizeof(__u32), sizeof(struct ip_block), 1024, nullptr);
    if (map_id < 0) {
        std::cerr << "Failed to create BPF map: " << std::strerror(errno) << std::endl;
        return true;
    }
    /*std::vector<std::string> mapCommands ={"sudo gcc -c -o ip_block_map.o ip_block_map.h",
    "sudo bpftool map create /sys/fs/bpf/ip_block_map type hash key 32/nat max_entries 1024 value struct ip_block"};

    for (std::string& str : mapCommands)
    {
        int result = system(str.c_str());
        if(result == 1) {return false;}
    }*/

    const __uint32_t ipInt = ReverseIP(IPv4StringToInt(ip));

    if (!runner.compileAndRunEBPFProgram()) {
        std::cerr << "Failed to compile and run eBPF program." << std::endl;
        return false;
    }

    struct ip_block block = {ipInt, 0xFFFFFFFF};

    int result = bpf_map_update_elem(map_id, &ipInt, &block, BPF_ANY);
    if (result < 0) {
        std::cerr << "Error: Failed to update BPF map: " << std::strerror(errno) << std::endl;
        return false;
    }

    //runner.printLogOfProgram();

    // int result = 0;
    // std::vector<std::string> commands = {
    //     "echo \"" + std::to_string(ip) + "\" > /sys/fs/bpf/my_map",
    //     "echo \"insert 0 " + std::to_string(ip) + "\" > /sys/fs/bpf/my_map",
    //     "cat /sys/fs/bpf/my_map"
    // };
    //
    // for (std::string& str : commands)
    // {
    //     FILE* pipe = popen(str.c_str(), "w");
    //     if (!pipe) {
    //         std::cerr << "Error: Unable to open pipe for command." << std::endl;
    //         return false;
    //     }
    //
    //     if (fwrite(&block, sizeof(struct ip_block), 1, pipe) != 1) {
    //         std::cerr << "Error: Failed to write to pipe." << std::endl;
    //         return false;
    //     }
    //
    //     result = pclose(pipe);
    //     if(result == 1) {
    //         std::cerr << "Error: Failed to execute command." << std::endl;
    //         return false;
    //     }
    // }


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

