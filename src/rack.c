/**
 * rack.c
 *
 * This file contains the internal data-structure and function definitions
 * for the rack type.
 *
 * Version: 0.1.1
 * Author(s): Richard Gale
 */
#include "rack.h"

struct rack_data {
    stepper_motor zmotor;
    stepper_motor xmotor;
};

/**
 * This function initialises the rack provided to it.
 */
void rack_init(rack* rp)
{
    /* Allocate memory to the rack. */
    *rp = (rack) malloc(sizeof(struct rack_data));

    /* Initialise motors. */
    stepper_motor_init(&(*rp)->zmotor, 2048, 26, 20, 19, 16);
    stepper_motor_steps_per_sec(&(*rp)->zmotor, 400);
    stepper_motor_init(&(*rp)->xmotor, 2048, 22, 10, 24, 9);
    stepper_motor_steps_per_sec(&(*rp)->xmotor, 400);
}

/**
 * This function terminates the rack provided to it.
 */
void rack_term(rack* rp)
{
    /* Terminating the stepper_motors. */
    stepper_motor_term(&(*rp)->zmotor);
    stepper_motor_term(&(*rp)->xmotor);

    /* De-allocate memory from the rack. */
    free(*rp);
}

/**
 * This function rotates the rack provided to it by one degree on its z axis.
 */
void rotate_z_1degree(rack* rp, enum RotationDirection direction)
{
    /* The number of teeth on the stepper gear is 60, the number on the
     * internal gear is 95.
     * Gear ratio = 95:60 = 1.58333:1.
     * 2048 steps * 1.58333 = ~3242 steps per internal gear revolution.
     * 3242 steps / 360 degress = ~9 steps per degree of the internal gear. */
    const int ONE_DEGREE = 9;

    /* Rotate by one degree. */
    if (direction == CLOCKWISE)
        stepper_motor_step(&(*rp)->zmotor, ONE_DEGREE);
    else if (direction == ANTICLOCKWISE)
        stepper_motor_step(&(*rp)->zmotor, -ONE_DEGREE);
}

/**
 * This function rotates the rack provided to by one degree it on its x axis.
 */
void rotate_x_1degree(rack* rp, enum RotationDirection direction )
{
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
    const int ONE_DEGREE = 568;

    /* Rotate by one degree. */
    if (direction == CLOCKWISE)
        stepper_motor_step(&(*rp)->xmotor, ONE_DEGREE);
    else if (direction == ANTICLOCKWISE)
        stepper_motor_step(&(*rp)->xmotor, -ONE_DEGREE);
}

/**
 * This function updates the rack provided to it.
 */
void rack_update(rack* rp, enum RackCommand rack_command)
{
    switch (rack_command)
    {
        case X_CLOCKWISE :
            rotate_x_1degree(rp, CLOCKWISE);
            break;
        case X_ANTICLOCKWISE :
            rotate_x_1degree(rp, ANTICLOCKWISE);
            break;
        case Z_CLOCKWISE :
            rotate_z_1degree(rp, CLOCKWISE);
            break;
        case Z_ANTICLOCKWISE :
            rotate_z_1degree(rp, ANTICLOCKWISE);
            break;
        case NO_RACK_COMMAND :
            NULL;
            break;
    }
}
