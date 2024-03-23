#include <bpf/bpf_endian.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>


#define PROXY_PORT 9090

struct ip_block {
    __u32 ip;
    __u32 mask;
};

struct btf_ip_block_map_def {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u32);
    __type(value, struct ip_block);
};

struct btf_ip_block_map_def ip_block_map SEC(".maps");


void* data_end = NULL;
void* data = NULL;
struct ethhdr* eth = NULL;
struct iphdr* ip = NULL;
struct tcphdr* tcp = NULL;

unsigned short get_packet_dest_port(struct xdp_md* ctx) {
    data_end = (void *)(long)ctx->data_end;
    data = (void *)(long)ctx->data;

    eth = data;
    if (data + sizeof(*eth) > data_end) {
        return 0;
    }

    ip = (struct iphdr *)(data + sizeof(*eth));
    if (ip + 1 > (struct iphdr*)data_end) {
        return 0;
    }

    tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);
    if (tcp + 1 > (struct tcphdr*)data_end) {
        return 0;
    }


    unsigned short dest_port = bpf_ntohs(tcp->dest);
    return dest_port;
}



SEC("xdp")
int IpBlock(struct xdp_md *ctx) {


    long port = get_packet_dest_port(ctx);

    if(port != PROXY_PORT)
    {
        return XDP_PASS;
    }

    bpf_printk("GOT MESSAGE!!!");


    __u32 ip = bpf_get_smp_processor_id(); // Assuming you want to get the current CPU's ID
    struct ip_block *block;

    block = bpf_map_lookup_elem(&ip_block_map, &ip);
    if (block && (ip & block->mask) == block->ip) {
        return XDP_DROP;
    }

    return XDP_PASS;
}


// Specify the license for the BPF program
char IP_BLOCK_LICENSE[] SEC("license") = "Dual BSD/GPL";

