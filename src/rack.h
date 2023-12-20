/**
 * rack.h
 *
 * This file contains the public data-structure and function prototype declarations
 * for the rack type, as well as enumeration definitions for it.
 *
 * Version: 0.1.1
 * Author(s): Richard Gale
 */

#ifndef rack_h
#define rack_h

#include <stdio.h>
#include <stdlib.h>

#include "stepper_motor.h"
#include "mycutils.h"
#include "ldr.h"

/**
 * These are the directions in which the rack can rotate.
 */
enum RotationDirection { NOT_ROTATING, CLOCKWISE, ANTICLOCKWISE };

/**
 * These are the commands that can be sent to the rack.
 */
enum RackCommand {
    NO_RACK_COMMAND,
    X_CLOCKWISE,
    X_ANTICLOCKWISE,
    Z_CLOCKWISE,
    Z_ANTICLOCKWISE,
    LIGHT_SEARCH
};

/**
 * This is the data-structure of the rack type.
 */
typedef struct rack_data* rack;

/**
 * This function initialises the rack provided to it.
 */
void rack_init(rack* rp);

/**
 * This function terminates the rack provided to it.
 */
void rack_term(rack* rp);

/**
 * This function updates the rack provided to it.
 */
void rack_update(rack* rp, enum RackCommand rack_command);

#endif
