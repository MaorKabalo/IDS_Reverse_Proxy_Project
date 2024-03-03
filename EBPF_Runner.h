//
// Created by kali on 12/7/23.
//

#ifndef EBPF_RUNNER_H
#define EBPF_RUNNER_H

#include <iostream>
#include <string>
#include <bpf/libbpf.h>


class EBPF_Runner {
public:
    explicit EBPF_Runner(std::string ebpfProgramPath); //"program.c.bpf" => "program"
    ~EBPF_Runner();

    bool compileAndRunEBPFProgram() const;
    bool clean() const;
    bool isAlreadyRunning() const;
    bool printLogOfProgram() const;


private:
    std::string ebpfProgramPath_;
    std::string interfaceName_;

    int compileEBPFProgram() const;
    int loadEBPFProgram() const;


};




#endif //EBPF_RUNNER_H
