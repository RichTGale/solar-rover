#!/bin/bash

DIRECTORY="./build"

#if [ ! -d "$DIRECTORY" ]; then
#    printf "Creating directory ${DIRECTORY}.\n"
#    mkdir ./build
#fi

rm -rf ${DIRECTORY}
mkdir ${DIRECTORY}

cmake -H. -B ./build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

make --directory=./build
