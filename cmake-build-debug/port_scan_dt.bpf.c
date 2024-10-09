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
#define NMAP_PORTS 1000


void* data_end = NULL;
void* data = NULL;
struct ethhdr* eth = NULL;
struct iphdr* ip = NULL;
struct tcphdr* tcp = NULL;


struct each_port {
    __u32 port;
};

struct btf_const_nmap_ports_def {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(max_entries, 1000); // Maximum entries
    __uint(pinning, LIBBPF_PIN_BY_NAME);
    __type(key, __u32);       // Key type
    __type(value, struct each_port); // Value type (your struct)
} const_nmap_ports SEC(".maps");


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
int port_scan_dt(struct xdp_md *ctx)
{

//    long port = get_packet_dest_port(ctx);
//
//    if(port != PROXY_PORT) //Check traffic for proxy port only
//    {
//        return XDP_PASS;
//    }
//
//
//
//
//
//    bpf_printk("%lld", MAX_BITS);

    return XDP_PASS;

}


char PORT_SCAN_DT_LICENSE[] SEC("license") = "Dual BSD/GPL";