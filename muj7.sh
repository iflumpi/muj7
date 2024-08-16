#!/usr/bin/bash

usage() {
    echo "Usage: $0 [-a source_file] [-o output_file] [-d binary_file] [-m binary_file]"
}

if [[ $# -le 0 ]] then
    usage
    exit 1
fi

while getopts "a:o:d:m:" opt; do
    case ${opt} in
        a)
            source_asm=$OPTARG 
            ;;
        o)
            output=$OPTARG
            ;;
        d)
            debugger_bin=$OPTARG 
            ;;
        m)
            machine_bin=$OPTARG 
            ;;
        :)
            echo "Option -$OPTARG requires an argument" >&2
            usage
            ;;
    esac
done

if [[ -n "$source_asm" ]] then
    if [[ -z "$output" ]] then
        output=output.bin
    fi
    build/muj7asm.out <$source_asm >$output
    exit 0
fi

if [[ -n "$debugger_bin" ]] then
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/build ./build/muj7debugger.out $debugger_bin
    exit 0
fi

if [[ -n "$machine_bin" ]] then
    LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PWD/build ./build/machine.out $machine_bin
    exit 0
fi
