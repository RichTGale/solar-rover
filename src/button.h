
#ifndef BUTTON_H
#define BUTTON_H

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdint.h>
#include <pi-gpio.h>

#include "mycutils.h"

#define COUNT_FALLING 0
#define COUNT_RISING  1
#define COUNT_BOTH    2

typedef struct button_data* button;

void button_init(button* bp, int pin, int mode);

void button_term(button* bp);

void set_debounce_time(button* bp, uint64_t time);

int get_state(button b);

int get_state_raw(button b);

bool is_pressed(button b);

bool is_released(button b);

void set_count_mode(button* bp, int mode);

unsigned long get_count(button b);

void reset_count(button* bp);

void loop(button* bp);

#endif
