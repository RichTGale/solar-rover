/*
 * ldr.c
 *
 * This file contains the internal data-structure and function definitions
 * for the ldr type.
 *
 * The ldr type communicates with an arduino which has a light dependant
 * resistor attached to it.
 *
 * Version: 1.0.0
 * Author: Richard Gale
 */

#include "ldr.h"

/**
 * This is the internal data-structure of the ldr type.
 */
struct ldr_data {
    int send_pin;
    int read_pin1;
    int read_pin2;
};

/**
 * This function initialises the ldr supplied to it.
 */
void ldr_init(ldr* lp, int send_pin, int read_pin1, int read_pin2)
{
    /* Allocate memory. */
    *lp = (ldr) malloc(sizeof(struct ldr_data));

    /* Initialise internal data. */
    (*lp)->send_pin = send_pin;
    (*lp)->read_pin1 = read_pin1;
    (*lp)->read_pin2 = read_pin2;

    /* Create inputs and outputs. */
    setup_gpio((*lp)->send_pin, OUTPUT, 0);
    setup_gpio((*lp)->read_pin1, INPUT, 0);
    setup_gpio((*lp)->read_pin2, INPUT, 0);

    /* Initialise outputs. */
    output_gpio((*lp)->send_pin, LOW);
}

/**
 * This function terminates the ldr supplied to it.
 */
void ldr_term(ldr* lp)
{
    /* De-allocate memory. */
    free(*lp);
}

/**
 * This function returns true if the light dependant resistor on the arduino
 * made a reading that was the brightest reading in a series.
 */
bool ldr_read(ldr l)
{
    bool highest = false;

    do
    {
        /* Tell the arduino that we're ready for it to make a reading. */
        output_gpio(l->send_pin, HIGH);
   
    /* Wait for the arduino made a reading. */
    } while (input_gpio(l->read_pin1) == LOW);

    /* Stop telling the arduino to make a reading. */
    output_gpio(l->send_pin, LOW);

    /* Return whether the reading was the brightest out of any reading
     * so far. */
    return input_gpio(l->read_pin2) == HIGH;
}
