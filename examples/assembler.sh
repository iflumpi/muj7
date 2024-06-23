#!/usr/bin/bash

cd ..
make assembler
build-debug/muj7asm.o <examples/program_2.asm >examples/program_2.bin
