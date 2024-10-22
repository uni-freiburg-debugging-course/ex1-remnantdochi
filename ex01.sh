#!/bin/bash

# ex) ./ex01.sh input.smt2
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <input_file.smt2>"
    exit 1
fi

input_file=$1  # first argument is the input file
main_output_file="main_output.txt"
z3_output_file="z3_output.txt"

echo "compiling fuzzer & main"

g++ -std=c++17 -o fuzzer fuzzer.cpp
if [ $? -ne 0 ]; then
    echo "ERR: Failed to compile fuzzer."
    exit 1
fi

g++ -std=c++17 -o main main.cpp
if [ $? -ne 0 ]; then
    echo "ERR: Failed to compile main."
    exit 1
fi

./fuzzer "$input_file"
if [ $? -ne 0 ]; then
    echo "ERR: Failed to generate input file."
    exit 1
fi

./main "$input_file" "$main_output_file"
if [ $? -ne 0 ]; then
    echo "ERR: main program failed to process the input file."
    exit 1
fi

z3 "$input_file" > "$z3_output_file"
if [ $? -ne 0 ]; then
    echo "ERR: z3 failed to process the input file."
    exit 1
fi

cmp "$main_output_file" "$z3_output_file"
if [ $? -eq 0 ]; then
    echo "SUCCESS: The output of main and z3 are identical."
else
    echo "FAIL: The output of main and z3 differ."
fi