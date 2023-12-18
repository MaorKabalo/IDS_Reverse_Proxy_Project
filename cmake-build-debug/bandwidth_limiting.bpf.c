//#include "vmlinux.h"
#include <bpf/bpf_endian.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void* data_end = NULL;
void* data = NULL;
struct ethhdr* eth = NULL;
struct iphdr* ip = NULL;
struct tcphdr* tcp = NULL;


unsigned short get_packet_source_port(struct xdp_md* ctx)
{
    data_end = NULL;
    data = NULL;
    eth = NULL;
    ip = NULL;


    data_end = (void *)(long)ctx->data_end;
    data = (void *)(long)ctx->data;

    eth = data;
    if (data + sizeof(*eth) > data_end) {
        return 0;
    }

    ip = (struct iphdr *)(data + sizeof(*eth));
    if (ip + 1 > data_end) {
        return 0;
    }

    tcp = NULL;
    if (data + sizeof(*eth) > data_end) {
        return 0;
    }

    tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(*tcp) > data_end) {
        return 0;
    }

    unsigned short src_port = bpf_ntohs(tcp->dest);

    return src_port;
}




SEC("xdp")
int bandwidth_limiting(struct xdp_md *ctx) {


    unsigned short port = get_packet_source_port(ctx);

    if (port == 8888) // TCP
    {
        bpf_printk("HELLO REVERSE PROXY!!! %ld", port);
    }
    else
    {
        //bpf_printk("Hello Other: %ld", port);
    }

    return XDP_PASS;
}

char BANDWIDTH_LIMITING_LICENSE[] SEC("license") = "Dual BSD/GPL";