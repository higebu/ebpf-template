#!/bin/bash

KERNEL_VERSION=v4.19

root_dir=$(pwd)

rm -rf build
mkdir build

git clone --branch $KERNEL_VERSION --depth 1 https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git ./build/linux
#sudo apt-get install -y linux-source-$KERNEL_VERSION
#tar xf /usr/src/linux-source-$KERNEL_VERSION.tar.xz -C ./build
#mv ./build/linux-source-$KERNEL_VERSION ./build/linux
pushd ./build/linux
make defconfig
make headers_install
popd
mkdir out
