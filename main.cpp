
#include <iostream>
#include <thread>
#include <chrono>
#include "EBPF_Runner.h"

int main() {

    std::string ebpfProgramPath = "bandwidth_limiting";

    EBPF_Runner ebpfRunner(ebpfProgramPath);

    if(ebpfRunner.compileAndRunEBPFProgram()) {
        ebpfRunner.printLogOfProgram();
    }


    return 0;


}