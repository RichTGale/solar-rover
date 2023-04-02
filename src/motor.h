/**
 * motor.h
 *
 * Data-structure and function definitions for a remote-control rover's motor.
 *
 * Author: Richard Gale.
 * Version: 2nd April, 2023.
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * The motor data structure.
 */
typedef struct motor_data* motor;

/**
 * Constructs the motor.
 */
void motor_init( 
                motor* mp, 
                const int EN_PIN,
                const int IN1_PIN,
                const int IN2_PIN,
                const int VEL_MAX 
                );

/**
 * Deconstructs the motor.
 */
void motor_free( motor* mp );

/**
 * Alters the duty-cycle of the provided motor.
 */
void motor_change_vel( motor* mp, int delta );

/**
 * Prints information about the provided motor.
 */
void motor_print( motor m );

#endif // MOTOR_H
