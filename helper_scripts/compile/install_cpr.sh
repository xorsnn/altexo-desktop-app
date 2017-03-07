#!/bin/bash
cd "${0%/*}"

mkdir -p libs
cd libs
git clone git@github.com:xorsnn/cpr.git
cd cpr
git checkout altexo-branch
cd ..
mkdir -p build
cd build
cmake ../cpr/ -DBUILD_CPR_TESTS=OFF -DUSE_SYSTEM_CURL=ON
make
