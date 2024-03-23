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
#define MAX_MESSAGES_PER_SECOND 27

long long messagesCounter = 0;
long long lastSecond = 0;
__u64 SECOND = 1000000000; // second in nano second


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
int rate_limiting(struct xdp_md *ctx)
{

    long port = get_packet_dest_port(ctx);


    if(port != PROXY_PORT) //Check traffic for proxy port only
    {
        return XDP_PASS;
    }

    long long currentTime = bpf_ktime_get_ns();
    if (currentTime >= lastSecond + SECOND) {
        messagesCounter = 0;
        lastSecond = currentTime;
    }

    if (messagesCounter >= MAX_MESSAGES_PER_SECOND) {
        bpf_printk("DROPPING!!!");
        return XDP_DROP;
    }

    messagesCounter++;
    bpf_printk("%lld", messagesCounter);

    bpf_printk("GOT MESSAGE TO SERVER");

    return XDP_PASS;

}

char RATE_LIMITING_LICENSE[] SEC("license") = "Dual BSD/GPL";