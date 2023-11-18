#pragma once
#ifndef EBPF_RUNNER_H
#define EBPF_RUNNER_H

#include <iostream>
#include <string>

class EBPF_Runner {
public:
    EBPF_Runner(const std::string& ebpfProgramPath); //"program.c.bpf" => "program"
    ~EBPF_Runner();

    bool compileAndRunEBPFProgram();
    bool clean();
    bool isAlreadyRunning();


private:
    std::string ebpfProgramPath;
    std::string interfaceName;

    int compileEBPFProgram();
    int loadEBPFProgram();


};

#endif // EBPF_RUNNER_H