#include <bpf/bpf_endian.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/types.h>



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

SEC("xdp")
int IpBlock(struct xdp_md *ctx) {
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

