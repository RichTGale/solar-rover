/**
 * rover.c
 *
 * Data-structure and function definitions for a remote-control rover.
 *
 * Author: Richard Gale.
 * Version: 4th September, 2023.
 */

#include "rover.h"

/**
 * The data contained within the rover data structure.
 */
struct rover_data {
    motor rmotor;
    motor lmotor;
    stepper zstep;
    stepper xstep;
    enum MotorDirection motor_direction;
    enum StepperRotation zstep_rotation;
    enum StepperRotation xstep_rotation;
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
    motor_init( &(*rp)->lmotor, MOTOR_ENA, MOTOR_IN1, MOTOR_IN2, 100 );
    motor_init( &(*rp)->rmotor, MOTOR_ENB, MOTOR_IN3, MOTOR_IN4, 100 );
    (*rp)->acc = 25;
    (*rp)->motor_direction = STOP;

    /* Initalising the stepper motors. */
    stepper_init( &(*rp)->zstep, 2048, ZSTEP_IN1, ZSTEP_IN2, 
                                       ZSTEP_IN3, ZSTEP_IN4 );
    stepper_init( &(*rp)->xstep, 2048, XSTEP_IN1, XSTEP_IN2,
                                       XSTEP_IN3, XSTEP_IN4 );
    stepper_steps_per_sec( &(*rp)->zstep, 400 );
    stepper_steps_per_sec( &(*rp)->xstep, 400 );
    (*rp)->zstep_rotation = NO_ROTATE;
    (*rp)->xstep_rotation = NO_ROTATE;
}

/**
 * Destroys the rover.
 */
void rover_free( rover* rp )
{
    // Cleaning up the motors
    motor_free(&(*rp)->rmotor);
    motor_free(&(*rp)->lmotor);

    /* Cleaning up the stepper motors. */
    stepper_free(&(*rp)->zstep);
    stepper_free(&(*rp)->xstep);

    // Destroying the rover.
    free(*rp);
}

/**
 * This function returns the provided rover's motor direction property.
 */
enum MotorDirection rover_get_motor_direction( rover r )
{
    return r->motor_direction;
}

///**
// * This function returns the provided rover's velocity/speed.
// */
//vec2d rover_get_direction( rover r )
//{
//    vec2d direction;
//
//    direction.x = motor_get_dutycycle(r->lmotor);
//    direction.y = motor_get_dutycycle(r->rmotor);
//
//    return direction;
//}

/**
 * This function changes the direction vector in which the rover is travelling.
 */
void rover_change_direction( rover* rp, enum MotorDirection direction )
{
    (*rp)->motor_direction = direction;

    switch ( (*rp)->motor_direction )
    {
        case FORWARDS : 
            motor_change_dutycycle( &(*rp)->lmotor, (*rp)->acc );
            motor_change_dutycycle( &(*rp)->rmotor, (*rp)->acc );
            break;
        case BACKWARDS : motor_change_dutycycle( &(*rp)->lmotor, (*rp)->acc*-1 );
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

/**
 * This function rotates the z axis of the solar rack of the provided rover
 * in the direction provided by one degree.
 */
void rover_step_z_1degree(rover* rp, enum StepperRotation rotation)
{
    /* The number of teeth on the stepper gear is 60, the number on the
     * internal gear is 95.
     * Gear ratio = 95:60 = 1.58333:1.
     * 2048 steps * 1.58333 = ~3242 steps per internal gear revolution.
     * 3242 steps / 360 degress = ~9 steps per degree of the internal gear. */
    const int ONE_DEGREE = 9;

    /* Rotating by one degree. */
    if (rotation == CLOCKWISE)
        stepper_step(&(*rp)->zstep, ONE_DEGREE);
    else if (rotation == ANTICLOCK)
        stepper_step(&(*rp)->zstep, -ONE_DEGREE);
}

/**
 * This function rotates the x axis of the solar rack of the provided rover
 * in the direction provided by one degree.
 */
void rover_step_x_1degree( rover* rp, enum StepperRotation rotation )
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

    /* Rotating by one degree. */
    if (rotation == CLOCKWISE)
        stepper_step(&(*rp)->xstep, ONE_DEGREE);
    else if (rotation == ANTICLOCK)
        stepper_step(&(*rp)->xstep, -ONE_DEGREE);
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
