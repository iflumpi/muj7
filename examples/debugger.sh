#!/usr/bin/bash

cd .. &&
make &&
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/build-debug ./build-debug/muj7debugger.o ./examples/program_2.bin
