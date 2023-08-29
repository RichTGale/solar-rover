/**
 * rover.c
 *
 * Data-structure and function definitions for a remote-control rover.
 *
 * Author: Richard Gale.
 * Version: 2nd April, 2023.
 */

#include "rover.h"

/**
 * The data contained within the rover data structure.
 */
struct rover_data {
    motor rmotor;
    motor lmotor;
    stepper rotator;
    enum MotorDirection motor_direction;
    enum StepRotation step_rotation;
    int acc;
};

/**
 * Initialises the rover
 */
void rover_init( rover* rp )
{
    // Allocating memory for the rover.
    *rp=(rover) malloc( sizeof( struct rover_data ) );

    // Initialising the motors.
    motor_init(&(*rp)->lmotor, ENA_PIN, IN1_PIN, IN2_PIN, 100);
    motor_init(&(*rp)->rmotor, ENB_PIN, IN3_PIN, IN4_PIN, 100);

    /* Initalising the stepper motor. */
    stepper_init(&(*rp)->rotator, 2048, 22, 10, 24, 9);
    stepper_steps_per_sec(&(*rp)->rotator, 400);

    // Setting the rover's acceleration rate.
    (*rp)->acc=25;
}

/**
 * Destroys the rover.
 */
void rover_free( rover* rp )
{
    // Cleaning up the motors
    motor_free(&(*rp)->rmotor);
    motor_free(&(*rp)->lmotor);
    stepper_free(&(*rp)->rotator);

    // Destroying the rover.
    free(*rp);
}

/**
 * This function returns the provided rover's velocity/speed.
 */
vec2d rover_get_direction( rover r )
{
    vec2d direction;

    direction.x = motor_get_dutycycle(r->lmotor);
    direction.y = motor_get_dutycycle(r->rmotor);

    return direction;
}

/**
 * Alters the rover's velocity.
 */
void rover_change_direction(rover* rp, enum MotorDirection direction)
{
    switch ( direction )
    {
        case FORWARDS : 
            motor_change_dutycycle( &(*rp)->lmotor, (*rp)->acc );
            motor_change_dutycycle( &(*rp)->rmotor, (*rp)->acc );
            break;
        case BACKWARDS :
	        motor_change_dutycycle( &(*rp)->lmotor, (*rp)->acc*-1 );
            motor_change_dutycycle( &(*rp)->rmotor, (*rp)->acc*-1 );
            break;
        case LEFT :
	        motor_change_dutycycle( &(*rp)->lmotor, (*rp)->acc*-1 );
            motor_change_dutycycle( &(*rp)->rmotor, (*rp)->acc );
            break; 
        case RIGHT :
	        motor_change_dutycycle( &(*rp)->lmotor, (*rp)->acc );
            motor_change_dutycycle( &(*rp)->rmotor, (*rp)->acc*-1 );
            break;
        case STOP :
            motor_change_dutycycle( &(*rp)->lmotor, 0 );
            motor_change_dutycycle( &(*rp)->rmotor, 0 );
            break;
    }
}

void rover_rotate_zaxis(rover* rp, enum StepRotation rotation)
{
    /* The number of teeth on the stepper gear is 60, the number on the
     * internal gear is 95. Gear ratio = 95:60 = 1.58333 : 1.
     * 2048 steps * 1.58333 = ~3242 steps per internal gear revolution. */
    if (rotation == CLOCKWISE) stepper_step(&(*rp)->rotator, 3242);
    if (rotation == ANTICLOCKWISE) stepper_step(&(*rp)->rotator, -3242);
}

/**
 * Prints information about the rover.
 */
void rover_print( rover r )
{
    // Printing information about the rover's motors.
    motor_print( r->lmotor );
    fprintf( stdout, " " );
    motor_print( r->rmotor );
}
