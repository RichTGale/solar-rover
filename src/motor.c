/**
 * motor.c
 *
 * Data structure and function definitions for a remote-control rover's motor.
 *
 * Author: Richard Gale.
 * Version: 2nd April, 2023.
 */

#include "motor.h"

/**
 * Data contained within a motor.
 */
struct motor_data {
    int en_pin;
    int in1_pin;
    int in2_pin;
    int vel_max;
    int vel;
};

/**
 * Contructs the motor.
 */
void motor_init( 
                motor* mp, 
                const int EN_PIN,
                const int IN1_PIN,
                const int IN2_PIN,
                const int VEL_MAX 
                )
{
    *mp=(motor) malloc( sizeof( struct motor_data ) );
    (*mp)->en_pin=EN_PIN;
    (*mp)->in1_pin=IN1_PIN;
    (*mp)->in2_pin=IN2_PIN;
    (*mp)->vel_max=VEL_MAX;
    (*mp)->vel=0;

    // Configuring the rpi gpio pins so they can communicate with the 
    // motor driver.
    setup_gpio( EN_PIN, OUTPUT, 0 );
    setup_gpio( IN1_PIN, OUTPUT, 0 );
    setup_gpio( IN2_PIN, OUTPUT, 0 );

    // Setting the motor PWM.
    pwm_set_duty_cycle( EN_PIN, 0 );
    pwm_set_frequency( EN_PIN, 480 );

    // Starting the PWM.
    pwm_start( EN_PIN );
}

/**
 * Deconstructs the motor.
 */
void motor_free( motor* mp )
{
    // Stopping the PWM instance.
    pwm_stop( (*mp)->en_pin );

    // Unallocating memory for the motor.
    free( *mp );
}

/**
 * Switches the flow of current through the motor so that it spins
 * in a forward direction relative to the rover.
 */
void motor_forwards( const int BCM1, const int BCM2 )
{
    // Setting the motor into a state of forwards rotation
    output_gpio( BCM1, HIGH );
    output_gpio( BCM2, LOW );
}

/**
 * Switches the flow of current through the motor so that it spins
 * in a backwards direction relative to the rover.
 */
void motor_backwards( const int BCM1, const int BCM2 )
{
    // Setting the motor into a state of backwards rotation
    output_gpio( BCM1, LOW );
    output_gpio( BCM2, HIGH );
}

/**
 * Switches the flow of current through the motor so that it stops spinning.
 */
void motor_stop( const int BCM1, const int BCM2 )
{
    // Setting the motor into a state of stopping
    output_gpio( BCM1, LOW );
    output_gpio( BCM2, LOW );
}

/**
 * Alters the duty cycle of the provided motor.
 */
void motor_change_vel( motor* mp, int delta )
{
    int dutycycle;  // The duty-cycle of the motor.

    // Setting the velocity of the motor.
    (*mp)->vel+=delta;
    if ( (*mp)->vel > (*mp)->vel_max )
        (*mp)->vel=(*mp)->vel_max;
    else if ( (*mp)->vel < (*mp)->vel_max*-1 )
        (*mp)->vel=(*mp)->vel_max*-1;


    // Ensuring that the motor is spinning in the correct direction.
    if ( delta>0  && (*mp)->vel>=0 )
    {
        motor_forwards( (*mp)->in1_pin, (*mp)->in2_pin );
    }
    else if ( delta<0 && (*mp)->vel<=0 )
    {
        motor_backwards( (*mp)->in1_pin, (*mp)->in2_pin );
    }
    else if ( delta==0 )
    {
        motor_stop( (*mp)->in1_pin, (*mp)->in2_pin );
        (*mp)->vel=delta;
    }

    // Setting the duty-cycle.
    if ( (*mp)->vel<0 )
        dutycycle=(*mp)->vel*-1;
    else
        dutycycle=(*mp)->vel;
    pwm_set_duty_cycle( (*mp)->en_pin, dutycycle );
}

/**
 * Prints information about the provided motor.
 */
void motor_print( motor m ) 
{
    // Printing the motor's velocity.
    fprintf( stdout, "vel: %d", m->vel );
}
