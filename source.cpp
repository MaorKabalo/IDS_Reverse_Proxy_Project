
#include <iostream>
#include "EBPF_Runner.cpp"
#include <thread>
#include <chrono>

int main() {

    std::string ebpfProgramPath = "hello";
    
    EBPF_Runner ebpfRunner(ebpfProgramPath);

    ebpfRunner.clean();


    //TODO: clean the program if it allready running in background

    // if(ebpfRunner.isAlreadyRunning())
    // {
    //     std::cout << "ALLREADY RUNNING!!!" << std::endl;
    //     ebpfRunner.clean();
    // }



    if(ebpfRunner.compileAndRunEBPFProgram())
    {
        std::cout << "RUNNING!!!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        ebpfRunner.clean();
    }
    else
    {
        std::cout << "NOT RUNNING!!!" << std::endl;
    }

    return 0;
}