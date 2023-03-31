

#include <pi-gpio.h>
#include <stdio.h>
#include <stdlib.h>

enum ServoType { STANDARD, CONTINUOUS };

typedef struct servo_data* servo;

void servo_init( servo* sp, int gpio_pin, enum ServoType type );

void servo_free( servo* sp );

void servo_rotate( servo* sp, int angle ); 
