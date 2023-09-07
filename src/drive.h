/**
 * drive.h
 *
 * This file contains the public data-structure and function prototype
 * declarations for the drive type, as well as enumeration definitions for it.
 *
 * Version: 1.0.0
 * Author(s): Richard Gale
 */

#ifndef drive_h
#define drive_h

#include <stdlib.h>

#include "brushed_motor.h"

/**
 * These are the commands that can be sent to the drive.
 */
enum DriveCommand {
    NO_DRIVE_COMMAND,   /* Do nothing. */
    ACCELERATE,         /* Make both motors go faster. */
    DECELERATE,         /* Make both motors go slower. */
    TURN_LEFT,          /* Make the left motor go slower and the right faster. */
    TURN_RIGHT,         /* Make the right motor go slower and the left faster. */
    STOP_DRIVE          /* Stop the motors. */
};

/**
 * This is the data-structure of the drive type.
 */
typedef struct drive_data* drive;

/**
 * This function initialises the drive provided to it.
 */
void drive_init(drive* dp);

/**
 * This function terminates the drive provided to it.
 */
void drive_term(drive* dp);

/**
 * This function returns the duty cycle of the lmotor property of the 
 * drive provided to the function.
 */
int drive_get_lmotor_duty_cycle(drive d);

/**
 * This function returns the duty cycle of the rmotor property of the 
 * drive provided to the function.
 */
int drive_get_rmotor_duty_cycle(drive d);

/**
 * This function updates the drive provided to it.
 */
void drive_update(drive* dp, enum DriveCommand drive_command);

#endif
