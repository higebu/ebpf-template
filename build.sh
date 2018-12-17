#!/bin/bash

prog_dir=./build/linux/bpfprog
rm -rf $prog_dir
mkdir -p $prog_dir
cp ./Makefile-bpf $prog_dir/Makefile
cp -r ./src $prog_dir
pushd $prog_dir
make EXTRA_CFLAGS="$*"
popd
cp $prog_dir/src/example.o ./out/
