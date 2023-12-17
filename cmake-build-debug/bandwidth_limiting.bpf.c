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



 //Recevies incoming http response!!!


void* data_end = NULL;
void* data = NULL;
struct ethhdr* eth = NULL;
struct iphdr* ip = NULL;
struct tcphdr* tcp = NULL;


long get_packet_protocol(struct xdp_md *ctx) {

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

    // Extract the IP protocol field from the IP header
    // 1 = ICMP
    // 6 = TCP
    // 17 = UDP
    __u64 protocol = ip->protocol;

    return protocol;
}

long get_packet_dest_port(struct xdp_md* ctx)
{
    tcp = NULL;
    if (data + sizeof(*eth) > data_end) {
        return 0;
    }

    tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(*tcp) > data_end) {
        return 0;
    }

    __u16 dest_port = tcp->dest;

    return dest_port;
}




SEC("xdp")
int bandwidth_limiting(struct xdp_md *ctx) {

    long protocol = get_packet_protocol(ctx);
    long port = get_packet_dest_port(ctx);

    if (protocol == IPPROTO_TCP) // TCP
    {
         bpf_printk("HELLO HTTP!!! %ld", port);
    }
    else
    {
        //bpf_printk("Hello Other: %ld\n", protocol);
    }

    return XDP_PASS;
}

char BANDWIDTH_LIMITING_LICENSE[] SEC("license") = "Dual BSD/GPL";