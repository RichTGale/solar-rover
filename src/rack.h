/**
 * rack.h
 *
 * This file contains the public data-structure and function prototype declarations
 * for the rack type, as well as enumeration definitions for it.
 *
 * Version: 0.1.0
 * Author(s): Richard Gale
 */

#ifndef rack_h
#define rack_h

#include <stdlib.h>

#include "stepper_motor.h"

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

///**
// * This function rotates the rack provided to it by one degree on its z axis.
// */
//void rack_rotate_z_1degree(rack* rp, enum RotationDirection direction);
//
///**
// * This function rotates the rack provided to by one degree it on its x axis.
// */
//void rack_rotate_x_1degree(rack* rp, enum RotationDirection direction );

/**
 * This function updates the rack provided to it.
 */
void rack_update(rack* rp, enum RackCommand rack_command);

#endif
