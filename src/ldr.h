


#ifndef LDR_H
#define LDR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pi-gpio.h>

typedef struct ldr_data* ldr;

void ldr_init(ldr* lp, int send_pin, int read_pin1, int read_pin2);

void ldr_term(ldr* lp);

bool ldr_read(ldr l);

#endif // LDR_H
