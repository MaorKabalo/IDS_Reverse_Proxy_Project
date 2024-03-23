//
// Created by kali on 2/4/24.
//

#ifndef BLOCKIP_H
#define BLOCKIP_H
#include "EBPF_Runner.h"
//#include "ip_block_map.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <linux/bpf.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <cstdint>

#include "IPTools.h"

class BlockIP : public IPTools{
public:
    static bool blockIP(const char* ip);
    static bool unblockIP(const char* ip);
private:
    static int map_id;
};



#endif //BLOCKIP_H
