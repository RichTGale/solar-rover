/**
 * drive.c
 *
 * This file contains the internal data-structure and function definitions
 * for the drive type.
 *
 * Version: 1.0.0
 * Author(s): Richard Gale
 */

#include "drive.h"

/**
 * This is the internal data of the drive type.
 */
struct drive_data {
    brushed_motor lmotor;   /* The drive's left motor. */
    brushed_motor rmotor;   /* The drive's right motor. */
    int acceleration_rate;  /* The drive's rate of acceleration. */
};

/**
 * This function initialises the drive provided to it.
 */
void drive_init(drive* dp)
{
    /* Allocate memory to the drive. */
    *dp = (drive) malloc(sizeof(struct drive_data));

    /* Initialise motors. */
    brushed_motor_init(&(*dp)->lmotor, 100, 12, 17, 27);
    brushed_motor_init(&(*dp)->rmotor, 100, 13, 5, 6);
    (*dp)->acceleration_rate = 25;
}

/**
 * This function terminates the drive provided to it.
 */
void drive_term(drive* dp)
{
    /* Terminating the motors. */
    brushed_motor_term(&(*dp)->lmotor);
    brushed_motor_term(&(*dp)->rmotor);

    /* De-allocating memory from the drive. */
    free(*dp);
}

/**
 * This function returns the duty cycle of the lmotor property of the 
 * drive provided to the function.
 */
int drive_get_lmotor_duty_cycle(drive d)
{
    return brushed_motor_get_duty_cycle(d->lmotor);
}

/**
 * This function returns the duty cycle of the rmotor property of the 
 * drive provided to the function.
 */
int drive_get_rmotor_duty_cycle(drive d)
{
    return brushed_motor_get_duty_cycle(d->rmotor);
}

/**
 * This function updates the drive provided to it.
 */
void drive_update(drive* dp, enum DriveCommand drive_command)
{
    /* Checking the drive command. */
    switch (drive_command)
    {
        /* Make both motors go faster. */
        case ACCELERATE :
            brushed_motor_change_duty_cycle(&(*dp)->lmotor,
                                            (*dp)->acceleration_rate);
            brushed_motor_change_duty_cycle(&(*dp)->rmotor,
                                            (*dp)->acceleration_rate);
            break;
        
        /* Make both motors go slower. */
        case DECELERATE :
            brushed_motor_change_duty_cycle(&(*dp)->lmotor,
                                            -(*dp)->acceleration_rate);
            brushed_motor_change_duty_cycle(&(*dp)->rmotor,
                                            -(*dp)->acceleration_rate);
            break;

        /* Make the left motor go slower and the right faster. */
        case TURN_LEFT :
            brushed_motor_change_duty_cycle(&(*dp)->lmotor,
                                            -(*dp)->acceleration_rate);
            brushed_motor_change_duty_cycle(&(*dp)->rmotor,
                                            (*dp)->acceleration_rate);
            break;

        /* Make the right motor go slower and the left faster. */
        case TURN_RIGHT :
            brushed_motor_change_duty_cycle(&(*dp)->lmotor,
                                            (*dp)->acceleration_rate);
            brushed_motor_change_duty_cycle(&(*dp)->rmotor,
                                            -(*dp)->acceleration_rate);
            break;

        /* Make the motors stop. */
        case STOP_DRIVE :
            brushed_motor_change_duty_cycle(&(*dp)->lmotor, 0);
            brushed_motor_change_duty_cycle(&(*dp)->rmotor, 0);
            break;

        /* Do nothing. */
        case NO_DRIVE_COMMAND :
            NULL;
            break;
    }
}
