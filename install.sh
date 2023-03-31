#!/bin/bash

EXEC_DIR=${PWD}

if [ -d ${HOME} ]; then
    cd ${HOME}
    git clone https://github.com/Milliways2/pi-gpio.git
    cd pi-gpio
    ./build
    cd ${EXEC_DIR}
fi
