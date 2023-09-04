/**
 * rover.h
 *
 * Data structure and function declarations for a remote-control rover.
 *
 * Author: Richard Gale.
 * Version: 4th September, 2023.
 */

#ifndef ROVER_H
#define ROVER_H

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "stepper.h"

/* Raspberry pi gpio pin numbers for the dc motor driver. */
#define MOTOR_ENA 12
#define MOTOR_ENB 13
#define MOTOR_IN1 17
#define MOTOR_IN2 27
#define MOTOR_IN3 5
#define MOTOR_IN4 6

/* Raspberry pi gpio pin number for the stepper motor drivers. */
#define XSTEP_IN1 22
#define XSTEP_IN2 10
#define XSTEP_IN3 24
#define XSTEP_IN4 9
#define ZSTEP_IN1 26
#define ZSTEP_IN2 20
#define ZSTEP_IN3 19
#define ZSTEP_IN4 16

/**
 * These are thge different ways that the dc motors can move.
 */
enum MotorDirection { FORWARDS, BACKWARDS, STOP, LEFT, RIGHT };

/**
 * These are the different ways that the stepper motors can move.
 */
enum StepperRotation { CLOCKWISE, ANTICLOCK, NO_ROTATE };

/**
 * The Rover data structure.
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
 * This function returns the provided rover's motor direction property.
 */
enum MotorDirection rover_get_motor_direction( rover r );

/**
 * This function returns a vector that represents the duty-cycles of
 * its two driving motors.
 */
//vec2d rover_get_direction( rover r );

/**
 * This function changes the duty-cycles of the provided rover's two
 * driving motors based on the provided direction.
 */
void rover_change_direction( rover* rp, enum MotorDirection direction );

/**
 * This function rotates the z axis of the solar rack of the provided rover
 * in the direction provided by one degree.
 */
void rover_step_z_1degree( rover* rp, enum StepperRotation rotation );

/**
 * This function rotates the x axis of the solar rack of the provided rover
 * in the direction provided by one degree.
 */
void rover_step_x_1degree( rover* rp, enum StepperRotation rotation );

/**
 * Prints information about the rover.
 */
void rover_print( rover r );

#endif // ROVER_H
