

#include "servo.h"

struct servo_data {
    enum ServoType type;
    int gpio_pin;
    int min_angle;
    int max_angle;
};

void servo_init( servo* sp, int gpio_pin, enum ServoType type )
{
    *sp=( servo ) malloc( sizeof( struct servo_data ) );
    ( *sp )->gpio_pin=gpio_pin;
    ( *sp )->type=type;
    if ( ( *sp )->type==STANDARD )
    {
        ( *sp )->min_angle=0;
        ( *sp )->max_angle=180;
    }
    setup_gpio( ( *sp )->gpio_pin, OUTPUT, 0 );
    pwm_set_duty_cycle( ( *sp )->gpio_pin, 0 );
    pwm_set_frequency( ( *sp )->gpio_pin, 50 );
    pwm_start( ( *sp )->gpio_pin );
}

void servo_free( servo* sp )
{
    pwm_stop( ( *sp )->gpio_pin );
    free( *sp );
}

float map( float x, float in_min, float in_max, float out_min, float out_max )
{
    float out;
    
    out = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    printf( "%f\n", out );
    return out;
}

void servo_rotate( servo* sp, int angle )
{
    float in_min=( *sp )->min_angle*1.0;
    float in_max=( *sp )->max_angle*1.0;

    pwm_set_duty_cycle( 
            ( *sp )->gpio_pin, 
            map( angle*1.0, in_min, in_max, 2.0, 12.0 ) 
        );
}
