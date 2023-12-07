
#include <iostream>
#include <thread>
#include <chrono>
#include "EBPF_Runner.h"

int main() {


    std::string ebpfProgramPath = "hello";

    EBPF_Runner ebpfRunner(ebpfProgramPath);


    if(ebpfRunner.isAlreadyRunning())
    {
        std::cout << "ALLREADY RUNNING!!!, DELETING PROGRAM!" << std::endl;
        ebpfRunner.clean();
    }
    else
    {


        if(ebpfRunner.compileAndRunEBPFProgram())
        {
            std::cout << "RUNNING!!!" << std::endl;
        }
        else
        {
            std::cout << "NOT RUNNING!!!" << std::endl;
        }
    }

    return 0;


}