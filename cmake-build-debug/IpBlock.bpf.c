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
    __uint(pinning, LIBBPF_PIN_BY_NAME);
    __type(key, __u32);
    __type(value, struct ip_block);
} ip_block_map SEC(".maps");

//struct btf_ip_block_map_def ip_block_map SEC(".maps");


SEC("xdp")
int IpBlock(struct xdp_md *ctx) {
    __u32 ip = 0;

    struct ip_block *block;


    void* data_end = (void *)(long)ctx->data_end;

    void* data = (void *)(long)ctx->data;


    struct ethhdr *eth = (struct ethhdr *)data;

    if (data + sizeof(struct ethhdr) > data_end) {

        return XDP_DROP;

    }


    if (eth->h_proto != bpf_htons(ETH_P_IP)) {

        return XDP_PASS;

    }


    struct iphdr *iph = (struct iphdr *)(data + sizeof(struct ethhdr));

    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) > data_end) {

        return XDP_DROP;

    }

    ip = iph->saddr;



    block = bpf_map_lookup_elem(&ip_block_map, &ip);

    if(block)
    {
        bpf_printk("%u", block->ip);
    }


    if (block && (ip & block->mask) == block->ip) {

        return XDP_DROP;
    }

    return XDP_PASS;
}


// Specify the license for the BPF program
char IP_BLOCK_LICENSE[] SEC("license") = "Dual BSD/GPL";