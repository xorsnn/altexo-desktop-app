#!/bin/bash
cd "${0%/*}"

mkdir -p libs
cd libs
git clone https://github.com/whoshuu/cpr.git
mkdir -p build
cd build
cmake ../cpr/ -DBUILD_CPR_TESTS=OFF -DUSE_SYSTEM_CURL=ON
make
