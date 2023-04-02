
#ifndef MOTOR_H
#define MOTOR_H

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct motor_data* motor;

void motor_init( 
                motor* mp, 
                const int EN_PIN,
                const int IN1_PIN,
                const int IN2_PIN,
                const int VEL_MAX 
                );

void motor_free( motor* mp );

/**
 * Returns the direction in which a motor is turning.
 */
enum MotorDirection motor_direction( const int BCM1, const int BCM2 );

void motor_change_vel( motor* mp, int delta );

void motor_print( motor m );

#endif // MOTOR_H
