//
// Created by kali on 12/7/23.
//

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

int counter = 0;

SEC("xdp")
int hello(struct xdp_md *ctx) {
    bpf_printk("Hello World %d", counter);
    counter++;
    return XDP_PASS;
}

char HELLO_LICENSE[] SEC("license") = "Dual BSD/GPL";
