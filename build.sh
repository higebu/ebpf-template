#!/bin/bash

prog_dir=./build/bpfprog
rm -rf $prog_dir
mkdir -p ${prog_dir}/include
cp ./Makefile-bpf $prog_dir/Makefile
cp -r ./src $prog_dir
cp ./lib/linux_includes/* "${prog_dir}/include/"
pushd $prog_dir
make EXTRA_CFLAGS="$*"
popd
mkdir -p ./out
cp $prog_dir/src/example.o ./out/
