#!/bin/bash

DIRECTORY="./build"

if [ ! -d "$DIRECTORY" ]; then
    printf"Creating diretory ${DIRECTORY}."
    mkdir ./build
fi

cmake -H. -B ./build -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=YES

make --directory=./build
