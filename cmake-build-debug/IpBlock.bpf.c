#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

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

SEC("prog")
int bpf_prog(struct xdp_md *ctx) {
    __u32 ip = ctx->data;
    struct ip_block *block;

    block = bpf_map_lookup_elem(&ip_block_map, &ip);
    if (block && (ip & block->mask) == block->ip) {
        return XDP_DROP;
    }

    return XDP_PASS;
}

SEC("xdp")
int a_xdp(){
return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
