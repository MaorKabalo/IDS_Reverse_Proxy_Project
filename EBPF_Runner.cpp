// ebpf_runner.cpp
#include "EBPF_Runner.h"
#include <cstdlib>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
using namespace std;

EBPF_Runner::EBPF_Runner(const std::string& ebpfProgramPath) : ebpfProgramPath(ebpfProgramPath) {
    std::string command = "sudo bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h";
    system(command.c_str());
}

EBPF_Runner::~EBPF_Runner() {}

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
    std::string command = "sudo clang -target bpf -D __BPF_TRACING__ -I/usr/include/$(uname -m)-linux-gnu -Wall -O2 -o " + ebpfProgramPath + ".bpf.o -c " + ebpfProgramPath + ".bpf.c";

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
        "rm source"
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
    const std::string folderPath = "/sys/fs/bpf";
    DIR* dir = opendir(folderPath.c_str());

    if (dir == nullptr) {
        perror("opendir");
        return false;  // Unable to open the directory
    }

    struct dirent* entry = readdir(dir);  // Read the first entry

    bool isEmpty = (entry == nullptr);

    closedir(dir);

    return isEmpty;

}