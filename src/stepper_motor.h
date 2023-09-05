/**
 * stepper_motor.h
 *
 * This file contains the public data-structure and function prototype declarations
 * for the stepper_motor type.
 *
 * Version: 0.1.0
 * Author(s): Richard Gale
 */

#ifndef stepper_motor_h
#define stepper_motor_h

#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pi-gpio.h>

#include "mycutils.h"

/**
 * This is the data-structure of the stepper_motor type.
 */
typedef struct stepper_motor_data* stepper_motor;

/**
 * This function initialises the  stepper_motor provided to it.
 */
void stepper_motor_init(stepper_motor* smp, int num_steps, 
                                            int in1_pin, int in2_pin,
                                            int in3_pin, int in4_pin);

/**
 * This function terminates the stepper_motor provided to it.
 */
void stepper_motor_term(stepper_motor* smp);

/**
 * This function sets the delay time between the steps of the stepper_motor
 * provided to it.
 */
void stepper_motor_steps_per_sec(stepper_motor* smp, unsigned int steps_per_sec);

/**
 * This function rotates the stepper motor provided to it.
 */
void stepper_motor_step(stepper_motor* smp, int num_steps);

#endif
