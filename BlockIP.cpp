//
// Created by kali on 2/4/24.
//

#include "BlockIP.h"

// Define the map structure
struct ip_block {
    __u32 ip;
    __u32 mask;
};

bool BlockIP::blockIP(__uint32_t ip) {
    EBPF_Runner runner("IpBlock");

    int map_fd = bpf_map_create(BPF_MAP_TYPE_HASH, "my_map", sizeof(__u32), sizeof(struct ip_block), 1024, nullptr);
    if (map_fd < 0) {
        std::cerr << "Failed to create BPF map: " << std::strerror(errno) << std::endl;
        return 1;
    }
    /*std::vector<std::string> mapCommands ={"sudo gcc -c -o ip_block_map.o ip_block_map.h",
    "sudo bpftool map create /sys/fs/bpf/ip_block_map type hash key 32/nat max_entries 1024 value struct ip_block"};

    for (std::string& str : mapCommands)
    {
        int result = system(str.c_str());
        if(result == 1) {return false;}
    }*/

    if (!runner.compileAndRunEBPFProgram()) {
        std::cerr << "Failed to compile and run eBPF program." << std::endl;
        return false;
    }

    struct ip_block block = {ip, 0xFFFFFFFF};

    int result = 0;
    std::vector<std::string> commands = {
        "echo \"" + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "echo \"insert 0 " + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "cat /sys/fs/bpf/ip_block_map"
    };

    for (std::string& str : commands)
    {
        FILE* pipe = popen(str.c_str(), "w");
        if (!pipe) {
            std::cerr << "Error: Unable to open pipe for command." << std::endl;
            return false;
        }

        if (fwrite(&block, sizeof(struct ip_block), 1, pipe) != 1) {
            std::cerr << "Error: Failed to write to pipe." << std::endl;
            return false;
        }

        result = pclose(pipe);
        if(result == 1) {
            std::cerr << "Error: Failed to execute command." << std::endl;
            return false;
        }
    }


    std::cout << "IP address " << ip << " is blocked." << std::endl;

    return true;
}

bool BlockIP::unblockIP(__uint32_t ip) {
    EBPF_Runner runner("IpBlock");

    if (!runner.compileAndRunEBPFProgram()) {
        std::cerr << "Failed to compile and run eBPF program." << std::endl;
        return false;
    }

    int result = 0;
    std::vector<std::string> commands = {
        "echo \"" + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "echo \"delete 0 " + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "cat /sys/fs/bpf/ip_block_map"
    };

    for (std::string& str : commands)
    {
        FILE* pipe = popen(str.c_str(), "w");
        if (!pipe) {
            std::cerr << "Error: Unable to open pipe for command." << std::endl;
            return false;
        }

        if (fwrite(&ip, sizeof(__uint32_t), 1, pipe) != 1) {
            std::cerr << "Error: Failed to write to pipe." << std::endl;
            return false;
        }

        result = pclose(pipe);
        if(result == 1) {
            std::cerr << "Error: Failed to execute command." << std::endl;
            return false;
        }
    }

    std::cout << "IP address " << ip << " is unblocked." << std::endl;

    return true;
}

