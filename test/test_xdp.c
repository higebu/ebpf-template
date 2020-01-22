#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#include "bpf_endian.h"

#define MAGIC_BYTES 123

/* ipv4 test vector */
struct ipv4_packet {
	struct ethhdr eth;
	struct iphdr iph;
	struct tcphdr tcp;
} __packed;
extern struct ipv4_packet pkt_v4;

struct ipv4_packet pkt_v4 = {
	.eth.h_proto = __bpf_constant_htons(ETH_P_IP),
	.iph.ihl = 5,
	.iph.protocol = IPPROTO_TCP,
	.iph.tot_len = __bpf_constant_htons(MAGIC_BYTES),
	.tcp.urg_ptr = 123,
	.tcp.doff = 5,
};

int main(void) {
	const char *file = "./src/example.o";
	struct bpf_object *obj;
	char buf[128];
	int err, prog_fd;
	__u32 duration, retval, size;
	err = bpf_prog_load(file, BPF_PROG_TYPE_XDP, &obj, &prog_fd);
	if (err) {
		fprintf(stderr, "ERR: loading eBPF object file (%d): %s\n", err, strerror(-err));
		return -1;
	};
	err = bpf_prog_test_run(prog_fd, 1, &pkt_v4, sizeof(pkt_v4),
				buf, &size, &retval, &duration);
	if (err) {
		fprintf(stderr, "ERR: bpf_prog_test_run (%d): %s\n", err, strerror(-err));
		return -1;
	};
	if (retval == XDP_PASS) {
		fprintf(stdout, "OK: test_xdp: retval=%d duration=%d\n", retval, duration);
	} else {
		fprintf(stdout, "FAIL: test_xdp: retval=%d duration=%d\n", retval, duration);
	};
	bpf_object__close(obj);
	return 0;
}
