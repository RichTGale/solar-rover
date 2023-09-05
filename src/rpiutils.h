/**
 * rpiutils.h
 *
 * This file contains function prototype declarations for various utility
 * functions for the raspberry pi computer.
 *
 * Version: 1.0.0
 * Author(s): Richard Gale
 */

#ifndef RPIUTILS_H
#define RPIUTILS_H

#include <stdio.h>
#include <pi-gpio.h>

#include "mycutils.h"

/**
 * Prints information about the Raspberry Pi this program is running on.
 */
void print_rpi_info( rpi_info info );

#endif // RPIUTILS_H
