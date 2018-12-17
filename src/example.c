#define KBUILD_MODNAME "foo"
#include <linux/if_ether.h>
#include <uapi/linux/bpf.h>

#include "bpf_helpers.h"

struct bpf_map_def SEC("maps") rxcnt = {
    .type = BPF_MAP_TYPE_PERCPU_ARRAY,
    .key_size = sizeof(u32),
    .value_size = sizeof(long),
    .max_entries = 256,
};

SEC("xdp_prog")
int prog(struct xdp_md *ctx) {
  void *data_end = (void *)(long)ctx->data_end;
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;
  int rc = XDP_DROP;
  long *value;
  u16 h_proto;
  u64 nh_off;

  nh_off = sizeof(*eth);
  if (data + nh_off > data_end)
    return rc;

  h_proto = eth->h_proto;

  value = bpf_map_lookup_elem(&rxcnt, &h_proto);
  if (value)
    *value += 1;

  return rc;
}

char _license[] SEC("license") = "GPL";
