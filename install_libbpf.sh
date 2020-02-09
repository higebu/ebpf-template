#!/bin/bash

git clone https://github.com/libbpf/libbpf.git
cd libbpf/src
make
sudo make install
