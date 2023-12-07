//
// Created by kali on 12/7/23.
//

#include "EBPF_Runner.h"
#include <cstdlib>
#include <vector>
using namespace std;

EBPF_Runner::EBPF_Runner(const std::string& ebpfProgramPath) : ebpfProgramPath(ebpfProgramPath) {
    std::string command = "sudo bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h";
    system(command.c_str());
}

EBPF_Runner::~EBPF_Runner() = default;

bool EBPF_Runner::compileAndRunEBPFProgram() {

    int compileResult = compileEBPFProgram();

    if (compileResult == 0)
    {
        int loadResult = loadEBPFProgram();

        if(loadResult == 0)
        {
            return true;
        }
    }

    return false;

}

int EBPF_Runner::compileEBPFProgram() {
    // Build the command to compile the eBPF program
    std::string command = "clang -target bpf -D __BPF_TRACING__ -I/usr/include/$(uname -m)-linux-gnu -Wall -O2 -o " + ebpfProgramPath + ".bpf.o -c " + ebpfProgramPath + ".bpf.c";

    // Execute the command using system()
    int result = system(command.c_str());

    return result;
}

int EBPF_Runner::loadEBPFProgram() {
    // Build the command to load the eBPF program into the kernel
    int result;
    vector<std::string> commands = {
        "sudo rm -f /sys/fs/bpf/" + ebpfProgramPath,
        "sudo ip link set dev eth0 xdp off",
        "sudo ip link set dev eth0 xdp obj " + ebpfProgramPath + ".bpf.o section xdp",
        //"cat /sys/kernel/tracing/trace_pipe"
    };

    for (std::string& str : commands)
    {
        result = system(str.c_str());
        if(result == 1) {return 1;}
    }

    return result;
}

bool EBPF_Runner::clean()
{
    int result;
    vector<std::string> commands = {
        "sudo ip link set dev eth0 xdp off",
        "sudo rm -f /sys/fs/bpf/" + ebpfProgramPath,
        "sudo rm " + ebpfProgramPath + ".bpf.o",
        "sudo rm -f /sys/fs/bpf/ip",
        "sudo rm -rf /sys/fs/bpf/tc",
        "sudo rm -f /sys/fs/bpf/xdp",
        "sudo rm vmlinux.h",
        "sudo rm Project"
    };

    for (std::string& str : commands)
    {
        result = system(str.c_str());
        if(result == 1) {return 1;}
    }

    return result;

}

bool EBPF_Runner::isAlreadyRunning()
{
    // Construct the bpftool command
    std::string bpftoolCommand = "sudo bpftool prog show name " + ebpfProgramPath;

    // Open a pipe to execute the command and read its output
    FILE* pipe = popen(bpftoolCommand.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error: Unable to open pipe for bpftool command." << std::endl;
        return false;
    }

    // Read the command output into a string
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }

    // Close the pipe
    pclose(pipe);

    // Check if the output contains information about the specified program name
    return result.find(ebpfProgramPath) != std::string::npos;
}
