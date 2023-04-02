
#include "motor.h"

struct motor_data {
    int en_pin;
    int in1_pin;
    int in2_pin;
    int vel_max;
    int vel;
};

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

void motor_free( motor* mp )
{
    // Stopping the PWM instance
    pwm_stop( (*mp)->en_pin );

    free( *mp );
}

void motor_forwards( const int BCM1, const int BCM2 )
{
    // Setting the motor into a state of forwards rotation
    output_gpio( BCM1, HIGH );
    output_gpio( BCM2, LOW );
}

/**
 * Ensure the motor pertaining to the provided gpio pins in in
 * a state of backwards rotation.
 */ 
void motor_backwards( const int BCM1, const int BCM2 )
{
    // Setting the motor into a state of backwards rotation
    output_gpio( BCM1, LOW );
    output_gpio( BCM2, HIGH );
}

/**
 * Ensures the motor pertaining to the providedd gpio pins is in
 * a state of stopping.
 */
void motor_stop( const int BCM1, const int BCM2 )
{
    // Setting the motor into a state of stopping
    output_gpio( BCM1, LOW );
    output_gpio( BCM2, LOW );
}

void motor_change_vel( motor* mp, int delta )
{
    int dutycycle;

    (*mp)->vel+=delta;

    // Setting the velocity of the motor.
    if ( (*mp)->vel > (*mp)->vel_max )
        (*mp)->vel=(*mp)->vel_max;

    // Setting the velocity of the motor.
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

    if ( (*mp)->vel<0 )
    {
        dutycycle=(*mp)->vel*-1;
    }
    else
    {
        dutycycle=(*mp)->vel;
    }

    if ( (*mp)->en_pin==12 )
    {
        dutycycle-=0;
    }

    // Setting the duty cycle of the motor.
    pwm_set_duty_cycle( (*mp)->en_pin, dutycycle );
}

void motor_print( motor m ) 
{
    fprintf( stdout, "vel: %d", m->vel );
}
