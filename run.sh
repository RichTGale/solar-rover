#!/bin/bash

DIRECTORY="output"

./compile.sh

cd ./build/bin

if [ ! -d "${DIRECTORY}" ]; then
    mkdir ./output
fi

./rover.run

cd ../
