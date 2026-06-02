#!/bin/bash
set -e
export PICO_SDK_PATH=~/pico/pico-sdk
rm -rf build
mkdir build
cd build
cmake ..
make -j4
echo "Build complete!"
ls -lh src/Advent.*
