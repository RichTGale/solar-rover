/**
 * rover.h
 *
 * Data structure and function declarations for a remote-control rover.
 *
 * Author: Richard Gale.
 * Version: 2nd April, 2023.
 */

#ifndef ROVER_H
#define ROVER_H

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>

#include "motor.h"
#include "stepper.h"

// Raspberry pi gpio pin numbers for the motor driver.
#define ENA_PIN 12
#define ENB_PIN 13 
#define IN1_PIN 17
#define IN2_PIN 27
#define IN3_PIN 5
#define IN4_PIN 6

/**
 * These are thge different ways that the dc motors can move.
 */
enum MotorDirection { FORWARDS, BACKWARDS, STOP, LEFT, RIGHT };

/**
 * These are the different ways that the stepper motors can move.
 */
enum StepRotation { CLOCKWISE, ANTICLOCKWISE, NOT_ROTATING };

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
 * This function returns a vector that represents the duty-cycles of
 * its two driving motors.
 */
vec2d rover_get_direction( rover r );

/**
 * This function changes the duty-cycles of the provided rover's two
 * driving motors based on the provided direction.
 */
void rover_change_direction( rover* rp, enum MotorDirection direction );

/**
 * This function rotates the provided stepper motor in the direction
 * provided.
 */
void rover_rotate_zaxis(rover* rp, enum StepRotation rotation);

/**
 * Prints information about the rover.
 */
void rover_print( rover r );

#endif // ROVER_H
