
#ifndef ROVER_H
#define ROVER_H

#include <pi-gpio.h>

#define ENA_PIN 12
#define ENB_PIN 13 
#define IN1_PIN 17
#define IN2_PIN 27
#define IN3_PIN 5
#define IN4_PIN 6

/**
 * Motor direction identities.
 */
enum MotorDirection { UNKNOWN, FORWARDS, BACKWARDS, STOPPED };

/**
 * Intialises the rover.
 */
void rover_init();

/**
 * Cleans up the rover.
 */
void rover_free();

/**
 * Returns the direction in which a motor is turning.
 */
enum MotorDirection motor_direction( const int BCM1, const int BCM2 );

/**
 * Moves the rover forward.
 */
void forward( const int DUTY_CYCLE );

/**
 * Moves the rover backwards.
 */
void backward( const int DUTY_CYCLE );

/** 
 * Rotates the rover left.s
 */
void left( const int DUTY_CYCLE );

/** 
 * Rotates the rover right.
 */
void right( const int DUTY_CYCLE );

/**
 * Stops the rover.
 */
void stop( const int DUTY_CYCLE );

#endif // ROVER_H
