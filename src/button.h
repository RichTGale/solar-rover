
#ifndef BUTTON_H
#define BUTTON_H

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <pi-gpio.h>

#include "mycutils.h"

/**
 * This is the data -structure of the button type.
 */
typedef struct button_data* button;

/**
 * This function initialises the button provided to it.
 */
void button_init(button* bp, int pin, int mode);

/**
 * This function terminates the button provided to it.
 */
void button_term(button* bp);

/**
 * This function sets the amount of time to wait before debouncing the button
 * provided to it.
 */
void button_set_debounce_time(button* bp, uint64_t time);

/**
 * This function returns the last recorded state the button was in.
 */
int button_get_state(button b);

/**
 * This function returns the current state of the button.
 */
int button_get_state_raw(button b);

/**
 * This function returns true if the button is currently pressed.
 */
bool button_is_pressed(button b);

/**
 * This function returns true if the button is currently not pressed.
 */
bool button_is_released(button b);

/**
 * This function updates the button.
 */
void button_update(button* bp);

#endif
