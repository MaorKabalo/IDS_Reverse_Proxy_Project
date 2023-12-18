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

#define HTTP_PORT 80

 //Recevies incoming http response!!!

#define MAX_HTTP_REQUEST_LEN 1024

void* data_end = NULL;
void* data = NULL;
struct ethhdr* eth = NULL;
struct iphdr* ip = NULL;
struct tcphdr* tcp = NULL;

char http_request[MAX_HTTP_REQUEST_LEN] = {0};


static void extract_http_request(void *payload, void *data_end, char *http_request) {
    char *cursor = http_request;
    int remaining_space = MAX_HTTP_REQUEST_LEN;

    // Loop through the TCP payload and copy bytes into the http_request buffer
    for (int i = 0; i < remaining_space && payload + i < data_end; i++) {
        http_request[i] = ((char *)payload)[i];
        cursor++;
        remaining_space--;
    }

    // Null-terminate the string
    http_request[MAX_HTTP_REQUEST_LEN - remaining_space] = '\0';
}

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

int is_packet_http(struct xdp_md *ctx)
{
    tcp = NULL;
    if (data + sizeof(*eth) > data_end) {
        return 0;
    }

    tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr);
    if (data + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(*tcp) > data_end) {
        return 0;
    }

    __u64 tcp_offset = (void *)tcp - data;
    if (tcp_offset + (tcp->doff << 2) > data_end) {
        return 0;
    }

    void* payload = data + tcp_offset + (tcp->doff << 2);

    extract_http_request(payload, data_end, http_request);

    if(http_request[0] == 'H' &&
       http_request[1] == 'T' &&
       http_request[2] == 'T' &&
       http_request[3] == 'P')
    {
        bpf_printk("%s", http_request);
        return 1;
    }



    return 0;

}


SEC("xdp")
int http_filter(struct xdp_md *ctx) {

    long protocol = get_packet_protocol(ctx);


    if (protocol == IPPROTO_TCP) // TCP
    {
        if(is_packet_http(ctx))
        {
            bpf_printk("HELLO HTTP!!!");
        }

    }
    else
    {
        //bpf_printk("Hello Other: %ld\n", protocol);
    }

    return XDP_PASS;
}

char HTTP_FILTER_LICENSE[] SEC("license") = "Dual BSD/GPL";