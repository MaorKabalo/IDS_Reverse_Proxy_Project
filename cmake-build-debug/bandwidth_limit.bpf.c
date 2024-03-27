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
#define BITS 10000000 //10 Mbps for example

long long MAX_BITS = BITS;
__u64 SECOND = 1000000000; // second in nano second

__u64 start_time = 0;
__u64 end_time = 0;
__u64 total_time = 0;

int initial = 0;

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
int bandwidth_limit(struct xdp_md *ctx)
{

    long port = get_packet_dest_port(ctx);

    start_time = end_time;

    if(port != PROXY_PORT) //Check traffic for proxy port only
    {
        return XDP_PASS;
    }

    if (!initial)
    {
        start_time = end_time;
        initial = 1;
    }

    end_time = bpf_ktime_get_ns();
    total_time += end_time - start_time;


    if (total_time >= SECOND)
    {
        MAX_BITS = BITS;
        total_time = 0; // Reset total_time after one second
        return XDP_PASS;
    }
    if (MAX_BITS <= 0)
    {
        bpf_printk("DROPPING!!!");
        return XDP_DROP;
    }

    __u32 packet_length_bytes = ctx->data_end - ctx->data;
    __u64 packet_length_bits = (__u64)packet_length_bytes * 8; //calculate bits of packet
    MAX_BITS -= packet_length_bits;

    bpf_printk("%lld", MAX_BITS);

    return XDP_PASS;

}


char BANDWIDTH_LIMIT_LICENSE[] SEC("license") = "Dual BSD/GPL";