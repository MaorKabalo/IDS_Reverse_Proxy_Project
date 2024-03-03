//
// Created by kali on 2/4/24.
//

#include "BlockIP.h"

struct ip_block {
    __uint32_t ip;
    __uint32_t mask;
};

bool BlockIP::blockIP(__uint32_t ip) {
    EBPF_Runner runner("IpBlock");

    if (!runner.compileAndRunEBPFProgram()) {
        std::cerr << "\n\nFailed to compile and run eBPF program." << std::endl;
        return !runner.clean();
    }

    struct ip_block block = {ip, 0xFFFFFFFF};

    int result = 0;
    std::vector<std::string> commands = {
        "sudo echo \"" + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "sudo echo \"insert 0 " + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "sudo cat /sys/fs/bpf/ip_block_map"
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
        "sudo echo \"" + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "sudo echo \"delete 0 " + std::to_string(ip) + "\" > /sys/fs/bpf/ip_block_map",
        "sudo cat /sys/fs/bpf/ip_block_map"
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

