//
// Created by kali on 12/7/23.
//

#include "EBPF_Runner.h"
#include <cstdlib>
#include <utility>
#include <vector>
using namespace std;

EBPF_Runner::EBPF_Runner(std::string  ebpfProgramPath) : ebpfProgramPath(std::move(ebpfProgramPath)) {
    std::string command = "sudo bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h";
    system(command.c_str());
}

EBPF_Runner::~EBPF_Runner() = default;

bool EBPF_Runner::compileAndRunEBPFProgram() const {

    if(isAlreadyRunning()) {
        std::cout << "\"" << ebpfProgramPath << "\"" << " is already a running eBPF program! deleting ebpf for safety!" << std::endl;
        return !clean();
    }

    return compileEBPFProgram() && loadEBPFProgram();
}

int EBPF_Runner::compileEBPFProgram() const{
    // Build the command to compile the eBPF program
    std::string command = "clang -target bpf -D __BPF_TRACING__ -I/usr/include/$(uname -m)-linux-gnu -Wall -O2 -o " + ebpfProgramPath + ".bpf.o -c " + ebpfProgramPath + ".bpf.c";

    // Execute the command using system()
    int result = system(command.c_str());

    return !result;
}

int EBPF_Runner::loadEBPFProgram() const{
    // Build the command to load the eBPF program into the kernel
    int result = 0;

    vector<std::string> commands = {
        "sudo rm -f /sys/fs/bpf/" + ebpfProgramPath,
        "sudo ip link set dev eth0 xdp off",
        "sudo ip link set dev eth0 xdp obj " + ebpfProgramPath + ".bpf.o section xdp",
    };

    for (std::string& str : commands)
    {
        result = system(str.c_str());
        if(result == 1) {return 0;}
    }

    return !result;
}

bool EBPF_Runner::clean() const
{
    int result = 0;
    vector<std::string> commands = {
        "sudo ip link set dev eth0 xdp off",
        "sudo rm -f /sys/fs/bpf/" + ebpfProgramPath,
        "sudo rm " + ebpfProgramPath + ".bpf.o",
        "sudo rm -f /sys/fs/bpf/ip",
        "sudo rm -rf /sys/fs/bpf/tc",
        "sudo rm -f /sys/fs/bpf/xdp",
        "sudo rm vmlinux.h",
        "sudo rm " + std::string(PROJECT_NAME)
    };

    for (std::string& str : commands)
    {
        result = system(str.c_str());
        if(result == 1) {return 0;}
    }

    return !result;

}

bool EBPF_Runner::isAlreadyRunning() const
{

    std::string bpftoolEbpfName = ebpfProgramPath.substr(0,15); //bpftool saves name for 15 characters max

    // Construct the bpftool command
    std::string bpftoolCommand = "sudo bpftool prog show name " + bpftoolEbpfName;

    // Open a pipe to execute the command and read its output
    FILE* pipe = popen(bpftoolCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Unable to open pipe for bpftool command." << std::endl;
        return false;
    }

    // Read the command output into a string
    char buffer[128];
    std::string result;
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    // Close the pipe
    pclose(pipe);

    // Check if the output contains information about the specified program name
    return result.find(bpftoolEbpfName) != std::string::npos;
}

bool EBPF_Runner::printLogOfProgram() const {
    std::string command = "cat /sys/kernel/tracing/trace_pipe";
    return !(system(command.c_str()));
}

