/**
 * brushed_motor.c
 *
 * This file contains the internal data-structure and function definitions
 * for the brushed_motor type.
 *
 * Version: 1.0.0
 * Author(s): Richard Gale
 */

#include "brushed_motor.h"

/**
 * This is the internal data-structure of the brushed_motor type.
 */
struct brushed_motor_data {
    /* The rpi gpio pins corresponding to the motor driver enable pin. */
    int en_pin; 
    int in1_pin;            
    int in2_pin;

    int duty_cycle;     /* The duty cycle of the motor. */
    int duty_cycle_max; /* The maximum possible duty cycle of the motor. */
};

/**
 * This function initialises the brushed motor provided to it.
 */
void brushed_motor_init(brushed_motor* bmp, int duty_cycle_max, int en_pin,
                                            int in1_pin, int in2_pin)
{
    /* Initialise properties. */
    *bmp = (brushed_motor) malloc(sizeof(struct brushed_motor_data));
    (*bmp)->duty_cycle_max = duty_cycle_max;
    (*bmp)->duty_cycle = 0;
    (*bmp)->en_pin = en_pin;
    (*bmp)->in1_pin = in1_pin;
    (*bmp)->in2_pin = in2_pin;

    /* Configure rpi pins so they can communicate with this motor's driver. */
    setup_gpio((*bmp)->en_pin, OUTPUT, 0);
    setup_gpio((*bmp)->in1_pin, OUTPUT, 0);
    setup_gpio((*bmp)->in2_pin, OUTPUT, 0);

    /* Initialise PWM. */
    pwm_set_duty_cycle((*bmp)->en_pin, (*bmp)->duty_cycle);
    pwm_set_frequency((*bmp)->en_pin, 480);
    pwm_start((*bmp)->en_pin);
}

/**
 * This function terminates the brushed motor provided to it.
 */
void brushed_motor_term(brushed_motor* bmp)
{
    /* Stop the PWM instance. */
    pwm_stop((*bmp)->en_pin);

    /* De-allocate memory from the brushed_motor. */
    free(*bmp);
}

/**
 * This function returns the duty cycle of the brushed_motor provided to it. 
 */
int brushed_motor_get_duty_cycle(brushed_motor bm)
{
    return bm->duty_cycle;
}

/**
 * This function switches the flow of current through the motor connected to
 * the pins provided to the function so that it spins in a forward rotation.
 */
void forwards(int bcm1, int bcm2)
{
    /* Putting the motor into a state of forwards rotation. */
    output_gpio(bcm1, HIGH);
    output_gpio(bcm2, LOW);
}

/**
 * This function switches the flow of current through the motor connected to
 * the pins provided to the function so that it spins in a backwards rotation.
 */
void backwards(int bcm1, const int bcm2)
{
    /* Putting the motor into a state of backwards rotation. */
    output_gpio(bcm1, LOW);
    output_gpio(bcm2, HIGH);
}

/**
 * This function stops the flow of current through the motor so that it stops 
 * rotating.
 */
void stop(int bcm1, int bcm2)
{
    /* Putting the motor into a stopped state. */
    output_gpio(bcm1, LOW);
    output_gpio(bcm2, LOW);
}

/**
 * This function changes the duty cycle of the provided motor.
 */
void brushed_motor_change_duty_cycle( brushed_motor* bmp, int delta )
{
    int duty_cycle;  /* The duty-cycle of the motor. */

    /* Calculating the new duty-cycle. */
    (*bmp)->duty_cycle += delta;

    /* Ensuring the duty-cycle is limited by the maximum possible value. */
    if ((*bmp)->duty_cycle > (*bmp)->duty_cycle_max) 
        (*bmp)->duty_cycle = (*bmp)->duty_cycle_max;
    else if ((*bmp)->duty_cycle < -(*bmp)->duty_cycle_max) 
        (*bmp)->duty_cycle = -(*bmp)->duty_cycle_max;

    /* Ensuring that the motor is spinning in the correct direction. */
    if (delta > 0 && (*bmp)->duty_cycle >= 0)
    {
        forwards((*bmp)->in1_pin, (*bmp)->in2_pin);
    }
    else if (delta < 0 && (*bmp)->duty_cycle <= 0)
    {
        backwards((*bmp)->in1_pin, (*bmp)->in2_pin);
    }
    else if (delta == 0)
    {
        stop((*bmp)->in1_pin, (*bmp)->in2_pin);
        (*bmp)->duty_cycle = delta;
    }

    /* Applying the duty-cycle to the motor. */
    if ((*bmp)->duty_cycle < 0)
        duty_cycle = -(*bmp)->duty_cycle;
    else
        duty_cycle = (*bmp)->duty_cycle;
    pwm_set_duty_cycle((*bmp)->en_pin, duty_cycle);
}
