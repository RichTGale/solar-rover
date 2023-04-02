
#ifndef ROVER_H
#define ROVER_H

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"

#define ENA_PIN 12
#define ENB_PIN 13 
#define IN1_PIN 17
#define IN2_PIN 27
#define IN3_PIN 5
#define IN4_PIN 6

/**
 * Motor direction identities.
 */
enum MotorDirection { FORWARDS, BACKWARDS, STOP, LEFT, RIGHT };

/**
 * The Rover data structure
 */
typedef struct rover_data* rover;

/**
 * Intialises the rover.
 */
void rover_init( rover* rp );

/**
 * Cleans up the rover.
 */
void rover_free( rover* rp );

/**
 * Alters the rover's velocity.
 */
void rover_change_vel( rover* rp, enum MotorDirection direction );

void rover_print( rover r );

#endif // ROVER_H
