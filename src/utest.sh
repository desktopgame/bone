#!/bin/sh
cmake -D CMAKE_BUILD_TYPE=Debug -D BONE_RUN=1 -D USE_CUNIT=OFF .
make