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
#define MAX_MESSAGES_PER_SECOND 20 //+3 //1000 MESSAGES_PER_SECOND for example, map allocate needed in the fture

long long messagesCounter = 0;
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

//10 packets == 8 messages


unsigned short get_packet_dest_port(struct xdp_md* ctx) {
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

    tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);
    if (tcp + 1 > data_end) {
        return 0;
    }

    unsigned short dest_port = bpf_ntohs(tcp->dest);
    return dest_port;
}



SEC("xdp")
int rate_limiting(struct xdp_md *ctx)
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
        messagesCounter = 0;
        total_time = 0; // Reset total_time after one second
        bpf_printk("SECOND PASSED!!!");
        return XDP_PASS;
    }
    if (messagesCounter >= MAX_MESSAGES_PER_SECOND) //basic "hello world", 4 more packets than actually sent (sent 10, got 14 packets, 20,24)
    {
        bpf_printk("DROPPING!!!");
        return XDP_DROP;
    }

    messagesCounter++;
    bpf_printk("%lld", messagesCounter);

    bpf_printk("GOT MESSAGE TO SERVER");

    return XDP_PASS;

}


char RATE_LIMITING_LICENSE[] SEC("license") = "Dual BSD/GPL";
