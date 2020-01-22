#define KBUILD_MODNAME "foo"
#include <linux/if_ether.h>

#include "bpf.h"
#include "bpf_helpers.h"

struct bpf_map_def SEC("maps") rxcnt = {
    .type = BPF_MAP_TYPE_PERCPU_ARRAY,
    .key_size = sizeof(__u32),
    .value_size = sizeof(long),
    .max_entries = 256,
};

SEC("xdp_prog")
int prog(struct xdp_md *ctx) {
  void *data_end = (void *)(long)ctx->data_end;
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;
  long *value;
  __u32 h_proto;
  __u64 nh_off;

  nh_off = sizeof(*eth);
  if (data + nh_off > data_end)
    return XDP_DROP;

  h_proto = (__u32)eth->h_proto;

  value = bpf_map_lookup_elem(&rxcnt, &h_proto);
  if (value) {
    *value += 1;
  };

  return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
