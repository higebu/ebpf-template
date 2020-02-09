#!/bin/bash -x

set -eu
set -o pipefail

if [[ "${1:-}" = "--in-vm" ]]; then
  shift

  mount -t bpf bpf /sys/fs/bpf

  echo Running tests...
  sudo ./test/test_xdp.o
  touch "$1/success"
  exit 0
fi

readonly kernel_version="${1:-}"
if [[ -z "${kernel_version}" ]]; then
  echo "Expecting kernel version as first argument"
  exit 1
fi

readonly kernel="linux-${kernel_version}.bz"
readonly tmp_dir="$(mktemp -d)"

test -e "${tmp_dir}/${kernel}" || {
  echo Fetching ${kernel}
  curl --fail -L "https://github.com/newtools/ci-kernels/blob/master/${kernel}?raw=true" -o "${tmp_dir}/${kernel}"
}

script=$(realpath "$0")
script_dir=$(dirname $script)
virtme_run="${VIRTME_RUN:-virtme-run}"

echo Testing on ${kernel_version}
sudo $virtme_run --kimg "${tmp_dir}/${kernel}" --show-boot-console --memory 256M --pwd --rw --script-sh "$script --in-vm $script_dir"

sudo rm -rf $tmp_dir

if [[ ! -f "$script_dir/success" ]]; then
  exit 1
fi
