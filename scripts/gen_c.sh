#!/bin/bash

# !!! all scripts executed from project root: ./scripts/gen_c.sh --design-module Memory --design-file  memory.sv 

# FILEPATH: /home/gzy/projects/mvm/scripts/gen_c.sh

if [ "$#" -lt 4 ]; then
    echo "Usage: --design-module <design_module> --design-file <design_file> [--debug <debug>]"
    exit 1
fi

debug="true"
design_base="src/MCVPack/BareDut/"
include_base="include/MCVPack/BareDut/"

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    --design-module)
    design_module="$2"
    shift # past argument
    shift # past value
    ;;
    --design-file)
    design_file="$2"
    shift # past argument
    shift # past value
    ;;
    --debug)
    debug="$2"
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    echo "Unknown option: $key"
    exit 1
    ;;
esac
done

echo "Design module: ${design_module}"
echo "Design file: ${design_file}"
echo "Debug: ${debug}"

mkdir -p tmp/design

# Generate C++ code using Verilator
if [ "$debug" = "true" ]; then
    verilator --cc -O0 --debug --Mdir tmp/design ${design_base}/${design_module}/${design_file}
else
    verilator --cc -O3 --Mdir tmp/design ${design_base}/${design_module}/${design_file}
fi

design_file_no_suffix=${design_file%.*}
echo "Design file without suffix: ${design_file_no_suffix}"

cd tmp/design && make -f V${design_file_no_suffix}.mk && cd ../..

# Move the generated *.c file to the design module directory
mv tmp/design/V*.a ${design_base}/${design_module}/

# Move the generated *.h file to the target include path
mv tmp/design/V*.h ${include_base}/${design_module}/

rm -r tmp/design