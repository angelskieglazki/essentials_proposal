#!/bin/bash

cmake -DCMAKE_TOOLCHAIN_FILE=cmake-toolchain-files/x86-linux-clang.cmake -S ./ -B build
# cmake -S ./ -B build
cmake --build build/