#!/bin/bash

EXEC_DIR=${PWD}
INSTALL_DIR="${home}/Programs"

if [ i! -d "$INSTALL_DIR" ]; then
    mkdir -p INSTALL_DIR
fi

cd ${INSTALL_DIR}
git clone https://github.com/Milliways2/pi-gpio.git
cd pi-gpio
./build

cd ${EXEC_DIR}
