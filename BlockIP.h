//
// Created by kali on 2/4/24.
//

#ifndef BLOCKIP_H
#define BLOCKIP_H
#include "EBPF_Runner.h"
#include <iostream>
#include <string>
#include <vector>


class BlockIP {
public:
    static bool blockIP(__uint32_t ip);
    static bool unblockIP(__uint32_t ip);
};



#endif //BLOCKIP_H
