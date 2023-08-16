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
    motor_init( &(*rp)->lmotor, ENA_PIN, IN1_PIN, IN2_PIN, 100 );
    motor_init( &(*rp)->rmotor, ENB_PIN, IN3_PIN, IN4_PIN, 100 );    

    // Setting the rover's acceleration rate.
    (*rp)->acc=25;
}

/**
 * Destroys the rover.
 */
void rover_free( rover* rp )
{
    // Cleaning up the motors
    motor_free( &(*rp)->rmotor );
    motor_free( &(*rp)->lmotor );

    // Destroying the rover.
    free( *rp );
}

/**
 * Alters the rover's velocity.
 */
void rover_change_vel(rover* rp, enum MotorDirection direction)
{
    switch ( direction )
    {
        case FORWARDS : 
            motor_change_vel( &(*rp)->lmotor, (*rp)->acc );
            motor_change_vel( &(*rp)->rmotor, (*rp)->acc );
            break;
        case BACKWARDS :
	        motor_change_vel( &(*rp)->lmotor, (*rp)->acc*-1 );
            motor_change_vel( &(*rp)->rmotor, (*rp)->acc*-1 );
            break;
        case LEFT :
	        motor_change_vel( &(*rp)->lmotor, (*rp)->acc*-1 );
            motor_change_vel( &(*rp)->rmotor, (*rp)->acc );
            break; 
        case RIGHT :
	        motor_change_vel( &(*rp)->lmotor, (*rp)->acc );
            motor_change_vel( &(*rp)->rmotor, (*rp)->acc*-1 );
            break;
        case STOP :
            motor_change_vel( &(*rp)->lmotor, 0 );
            motor_change_vel( &(*rp)->rmotor, 0 );
            break;
    }
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
