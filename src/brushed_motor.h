/*
 * brushed_motor.h
 *
 * This file contains the public data-structure and public function prototype
 * declarations for the brushed_motor type;
 *
 * Version: 1.0.0
 * Author(s): Richard Gale
 */

#ifndef brushed_motor_h
#define brushed_motor_h

#include <stdlib.h>
#include <pi-gpio.h>

/**
 * This is the data-structure of the brushed_motor type.
 */
typedef struct brushed_motor_data* brushed_motor;

/**
 * This function initialises the brushed motor provided to it.
 */
void brushed_motor_init(brushed_motor* bmp, int duty_cycle_max, int en_pin,
                                            int in1_pin, int in2_pin);

/**
 * This function terminates the brushed motor provided to it.
 */
void brushed_motor_term(brushed_motor* bmp);

/**
 * This function returns the duty cycle of the brushed_motor provided to it. 
 */
int brushed_motor_get_duty_cycle(brushed_motor bm);

/**
 * Alters the duty-cycle of the provided motor.
 */
void brushed_motor_change_duty_cycle( brushed_motor* bmp, int delta );

#endif
