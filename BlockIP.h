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


class BlockIP {
public:
    static bool blockIP(__uint32_t ip);
    static bool unblockIP(__uint32_t ip);
private:
    static int map_id;
};



#endif //BLOCKIP_H
