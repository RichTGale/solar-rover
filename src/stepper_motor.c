/**
 * stepper_motor.c
 *
 * This file contains the internal data-structure and function definitions
 * for the stepper_motor type.
 *
 * Version: 0.1.0
 * Author(s): Richard Gale
 */

#include "stepper_motor.h"

struct stepper_motor_data {
    struct timespec last_step_time; /* The time of the last step. */
    uint64_t step_delay;    /* The amount of time to wait between steps. */
    int direction;          /* The direction the motor is rotating. */
    int num_steps;          /* The number of steps per full revolution. */
    int step_num;           /* The current step number. */

    /* The rpi gpio pins this motor's driver is connected to. */
    int in1_pin;
    int in2_pin;
    int in3_pin;
    int in4_pin;
};

/**
 * This function initialises the  stepper_motor provided to it.
 */
void stepper_motor_init(stepper_motor* smp, int num_steps, 
                                            int in1_pin, int in2_pin,
                                            int in3_pin, int in4_pin)
{
    /* Allocate memory to the stepper motor. */
    *smp = (stepper_motor) malloc(sizeof(struct stepper_motor_data));

    /* Initialise properties. */
    start_timer(&(*smp)->last_step_time);
    (*smp)->direction = 0;
    (*smp)->num_steps = num_steps;
    (*smp)->step_num = 0;
    (*smp)->in1_pin = in1_pin;
    (*smp)->in2_pin = in2_pin;
    (*smp)->in3_pin = in3_pin;
    (*smp)->in4_pin = in4_pin;

    /* Configure rpi ins so they can communicate with this motor's driver. */
    setup_gpio((*smp)->in1_pin, OUTPUT, 0);
    setup_gpio((*smp)->in2_pin, OUTPUT, 0);
    setup_gpio((*smp)->in3_pin, OUTPUT, 0);
    setup_gpio((*smp)->in4_pin, OUTPUT, 0);
}

void stepper_motor_term(stepper_motor* smp)
{
    /* De-allocate memory from the stepper motor. */
    free(*smp);
}

/**
 * This function terminates the stepper_motor provided to it.
 */
void stepper_motor_steps_per_sec(stepper_motor* smp, unsigned int steps_per_sec)
{
    /* Set the amount of time to wait between steps. */
    (*smp)->step_delay = NANOS_PER_SEC / steps_per_sec;
}

/**
 * This function activates one of the four phases of the stepper_motor
 * provided to it.
 */
void step_motor(stepper_motor* smp, int this_step)
{
    /* Check which phase the motor should be in and activate it. */
    switch (this_step) 
    {
        case 0:
            output_gpio((*smp)->in1_pin, HIGH);
            output_gpio((*smp)->in2_pin, HIGH);
            output_gpio((*smp)->in3_pin, LOW);
            output_gpio((*smp)->in4_pin, LOW);
            break;
        case 1:
            output_gpio((*smp)->in1_pin, LOW);
            output_gpio((*smp)->in2_pin, HIGH);
            output_gpio((*smp)->in3_pin, HIGH);
            output_gpio((*smp)->in4_pin, LOW);
            break;
        case 2:
            output_gpio((*smp)->in1_pin, LOW);
            output_gpio((*smp)->in2_pin, LOW);
            output_gpio((*smp)->in3_pin, HIGH);
            output_gpio((*smp)->in4_pin, HIGH);
            break;
        case 3:
            output_gpio((*smp)->in1_pin, HIGH);
            output_gpio((*smp)->in2_pin, LOW);
            output_gpio((*smp)->in3_pin, LOW);
            output_gpio((*smp)->in4_pin, HIGH);
            break;
    }
}

/**
 * This function rotates the stepper motor provided to it.
 */
void stepper_motor_step(stepper_motor* smp, int num_steps)
{
    int steps_left = abs(num_steps);    /* The number of steps left. */

    /* Set the direction the motor should rotate. */
    if (num_steps > 0) (*smp)->direction = 1;
    if (num_steps < 0) (*smp)->direction = 0;

    /* Rotate the motor. */
    while (steps_left > 0)
    {
        /* Check if enough time has passed between steps. */
        if (check_timer((*smp)->last_step_time, (*smp)->step_delay))
        {
            /* Record the time of this step. */
            start_timer(&(*smp)->last_step_time);

            /* Check which direction the motor should rotate. */
            if ((*smp)->direction == 1)
            {
                /* Calculate and record the step number. */
                (*smp)->step_num++;
                if ((*smp)->step_num == (*smp)->num_steps)
                    (*smp)->step_num = 1;
            }
            else
            {
                /* Calculate and record the step number. */
                if ((*smp)->step_num == 0)
                    (*smp)->step_num = (*smp)->num_steps;
                (*smp)->step_num--;
            }
            /* Calculate and record how many steps are left. */
            steps_left--;

            /* Activate the appropriate phase of the stepper_motor. */
            step_motor(smp, (*smp)->step_num % 4);
        } 
    }
}
