#!/bin/bash

mkdir -p build/outputs/

for input_file in TestData/inputs/1.input*
do
    echo $input_file
    input_filename=$(basename ${input_file})
    echo build/outputs/"${input_filename%.input}.output"
    ./build/src/WordFrequencyCounter $input_file build/outputs/"${input_filename%.input}.output"
done