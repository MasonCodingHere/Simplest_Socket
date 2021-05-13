#!/bin/sh

set -x
pwd
mkdir build bin
cd build
cmake ..
make
