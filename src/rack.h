/**
 * rack.h
 *
 * This file contains the public data-structure and function prototype declarations
 * for the rack type, as well as enumeration definitions for it.
 *
 * Version: 0.1.2
 * Author(s): Richard Gale
 */

#ifndef rack_h
#define rack_h

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "stepper_motor.h"
#include "mycutils.h"
#include "ldr.h"
#include "button.h"

/* Judging from the 3d models simulations in blender, 7.5 revolutions
 * of the worm gear equals 1 revolution of the spur gear.
 * Gear ratio = 7.5:1. 
 * 2048 steps * 7.5 = 15360 steps per revolution of the spur gear. 
 * There are 200 teeth on the internal gear (if it was whole) and there
 * are 15 teeth on the spur gear. 
 * Gear ratio = 200:15 = 13.33333:1.
 * 15360 steps * 13.33333 = ~204,800 steps per internal gear revolution.
 * 204800 steps / 360 degress = ~568 steps per degree of the internal
 * gear. */
#define ONE_DEGREE_X 568

/* The number of teeth on the stepper gear is 62, the number on the
 * internal gear is 95.
 * Gear ratio = 95:62 = 1.532258065:1.
 * 2048 steps * 1.532258065 = ~3138 steps per internal gear revolution.
 * 3138 steps / 360 degress = ~9 steps per degree of the internal gear. */
#define ONE_DEGREE_Z 9

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
