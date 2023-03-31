
#include "rover.h"

/**
 * Initialises the rover
 */
void rover_init()
{
    // Setting up the motor driver
    setup_gpio( ENA_PIN, OUTPUT, 0 );
    setup_gpio( ENB_PIN, OUTPUT, 0 );
    setup_gpio( IN1_PIN, OUTPUT, 0 );
    setup_gpio( IN2_PIN, OUTPUT, 0 );
    setup_gpio( IN3_PIN, OUTPUT, 0 );
    setup_gpio( IN4_PIN, OUTPUT, 0 );

    // Setting the motor PWMs
    pwm_set_duty_cycle( ENA_PIN, 0 );
    pwm_set_duty_cycle( ENB_PIN, 0 );
    pwm_set_frequency( ENA_PIN, 480 );
    pwm_set_frequency( ENB_PIN, 480 );
    
    // Starting the PWMs
    pwm_start( ENA_PIN );
    pwm_start( ENB_PIN );
}

/**
 * Cleans up the rover.
 */
void rover_free()
{
    // Stopping the pwm instances
    pwm_stop( ENA_PIN );
    pwm_stop( ENB_PIN );
}

/**
 * Returns the direction in which a motor is turning.
 */
enum MotorDirection motor_direction( const int BCM1, const int BCM2 )
{
    // The direction the motor is spinning
    enum MotorDirection md = UNKNOWN;
    
    // Determining which direction in which the motor is spinning
    if ( BCM1==HIGH && BCM2==LOW ) 
    {
        md = FORWARDS;
    }
    else if ( BCM1==LOW && BCM2==HIGH )
    {
        md = BACKWARDS;
    }
    else if ( BCM1==LOW && BCM2==LOW )
    {
        md = STOPPED;
    }
    
    return md;
}

/**
 * Ensures the motor pertaining to the provided gpio pins is in 
 * a state of forwards rotation.
 */
void motor_forward( const int BCM1, const int BCM2 )
{
    // Determining if the motor is in a state of forwards rotation
    if ( ( motor_direction( BCM1, BCM2 ) != FORWARDS ) )
    {
        // Setting the motor into a state of forwards rotation
        output_gpio( BCM1, HIGH );
        output_gpio( BCM2, LOW );
    }
}

/**
 * Ensure the motor pertaining to the provided gpio pins in in
 * a state of backwards rotation.
 */ 
void motor_backward( const int BCM1, const int BCM2 )
{
    // Determining if the motor is in a state of backwards rotation
    if ( ( motor_direction( BCM1, BCM2 ) != BACKWARDS ) )
    {
        // Setting the motor into a state of backwards rotation
        output_gpio( BCM1, LOW );
        output_gpio( BCM2, HIGH );
    }
}

/**
 * Ensures the motor pertaining to the providedd gpio pins is in
 * a state of stopping.
 */
void motor_stop( const int BCM1, const int BCM2 )
{
    // Determining if the motor is in a state of stopping
    if ( ( motor_direction( BCM1, BCM2 ) != STOPPED ) )
    {
        // Setting the motor into a state of stopping
        output_gpio( BCM1, LOW );
        output_gpio( BCM2, LOW );
    } 
}

/**
 * Moves the rover forward.
 */
void forward( const int DUTY_CYCLE )
{
    // Turning motor 1 forwards
    motor_forward( IN1_PIN, IN2_PIN );
    pwm_set_duty_cycle( ENA_PIN, DUTY_CYCLE );
    
    // Turning motor 2 forwards
    motor_forward( IN3_PIN, IN4_PIN );
    pwm_set_duty_cycle( ENB_PIN, DUTY_CYCLE );
}

/**
 * Moves the rover backwards.
 */
void backward( const int DUTY_CYCLE )
{
    // Turning motor 1 backwards
    motor_backward( IN1_PIN, IN2_PIN );
    pwm_set_duty_cycle( ENA_PIN, DUTY_CYCLE );
    
    // Turning motor 2 backwards
    motor_backward( IN3_PIN, IN4_PIN );
    pwm_set_duty_cycle( ENB_PIN, DUTY_CYCLE );
}

/** 
 * Rotates the rover left.s
 */
void left( const int DUTY_CYCLE )
{
    motor_backward( IN1_PIN, IN2_PIN );
    pwm_set_duty_cycle( ENA_PIN, DUTY_CYCLE );
    motor_forward( IN3_PIN, IN4_PIN );
    pwm_set_duty_cycle( ENB_PIN, DUTY_CYCLE );
}

/** 
 * Rotates the rover right.
 */
void right( const int DUTY_CYCLE )
{
    motor_forward( IN1_PIN, IN2_PIN );
    pwm_set_duty_cycle( ENA_PIN, DUTY_CYCLE );
    motor_backward( IN3_PIN, IN4_PIN );
    pwm_set_duty_cycle( ENB_PIN, DUTY_CYCLE );
}

/**
 * Stops the rover.
 */
void stop( const int DUTY_CYCLE )
{
    // Stopping motor 1
    motor_stop( IN1_PIN, IN2_PIN );
    pwm_set_duty_cycle( ENA_PIN, DUTY_CYCLE );
    
    // Stopping motor 2
    motor_stop( IN3_PIN, IN4_PIN );
    pwm_set_duty_cycle( ENB_PIN, DUTY_CYCLE );
}


