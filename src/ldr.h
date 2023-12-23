/**
 * ldr.h
 *
 * This file contains the publicly available data-structure and function
 * prototype declarations for the ldr type.
 *
 * The ldr type communicates with an arduino which has a light dependant
 * resistor attached to it.
 *
 * Version 1.0.0
 * Author: Richard Gale
 */


#ifndef LDR_H
#define LDR_H

#include <stdlib.h>
#include <stdbool.h>
#include <pi-gpio.h>

/**
 * This is the data structure of the ldr type.
 */
typedef struct ldr_data* ldr;

/**
 * This function initialises the ldr supplied to it.
 */
void ldr_init(ldr* lp, int send_pin, int read_pin1, int read_pin2);

/**
 * This function terminates the ldr supplied to it.
 */
void ldr_term(ldr* lp);

/**
 * This function returns true if the light dependant resistor on the arduino
 * made a reading that was the brightest reading in a series.
 */
bool ldr_read(ldr l);

#endif // LDR_H
