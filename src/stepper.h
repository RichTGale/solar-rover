

#ifndef STEPPER_H
#define STEPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pi-gpio.h>

#include "mycutils.h"

typedef struct stepper_data* stepper;

void stepper_init(stepper* sp, int num_steps, int pin1, int pin2,
                                              int pin3, int pin4);

void stepper_free(stepper* sp);

void stepper_set_speed(stepper* sp, long speed);

void stepper_step(stepper* sp, int num_steps);


#endif // STEPPER_H
