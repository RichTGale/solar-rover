

#include "stepper.h"

struct stepper_data {
    int direction;
    uint64_t step_delay;
    int num_steps;
    int step_num;

    int pin1;
    int pin2;
    int pin3;
    int pin4;

    struct timespec last_step_time;
};

void stepper_init(stepper* sp, int num_steps, int pin1, int pin2,
                                              int pin3, int pin4)
{
    setup();

    *sp = (stepper) malloc(sizeof(struct stepper_data));
    (*sp)->step_num = 0;
    (*sp)->direction = 0;
    start_timer(&(*sp)->last_step_time);
    (*sp)->num_steps = num_steps;

    (*sp)->pin1 = pin1;
    (*sp)->pin2 = pin2;
    (*sp)->pin3 = pin3;
    (*sp)->pin4 = pin4;

    setup_gpio((*sp)->pin1, OUTPUT, 0);
    setup_gpio((*sp)->pin2, OUTPUT, 0);
    setup_gpio((*sp)->pin3, OUTPUT, 0);
    setup_gpio((*sp)->pin4, OUTPUT, 0);
}

void stepper_free(stepper* sp)
{
    free(*sp);
    cleanup();
}

void stepper_steps_per_sec(stepper* sp, unsigned int steps_per_sec)
{
    (*sp)->step_delay = NANOS_PER_SEC / steps_per_sec;
}

void step_motor(stepper* sp, int this_step)
{
    switch (this_step) {
        case 0:  // 1010
            output_gpio((*sp)->pin1, HIGH);
            output_gpio((*sp)->pin2, LOW);
            output_gpio((*sp)->pin3, HIGH);
            output_gpio((*sp)->pin4, LOW);
            break;
        case 1:  // 0110
            output_gpio((*sp)->pin1, LOW);
            output_gpio((*sp)->pin2, HIGH);
            output_gpio((*sp)->pin3, HIGH);
            output_gpio((*sp)->pin4, LOW);
            break;
        case 2:  //0101
            output_gpio((*sp)->pin1, LOW);
            output_gpio((*sp)->pin2, HIGH);
            output_gpio((*sp)->pin3, LOW);
            output_gpio((*sp)->pin4, HIGH);
            break;
        case 3:  //1001
            output_gpio((*sp)->pin1, HIGH);
            output_gpio((*sp)->pin2, LOW);
            output_gpio((*sp)->pin3, LOW);
            output_gpio((*sp)->pin4, HIGH);
            break;
    }
}

void stepper_step(stepper* sp, int num_steps)
{
    int steps_left = abs(num_steps);

    if (num_steps > 0) (*sp)->direction = 1;
    if (num_steps < 0) (*sp)->direction = 0;

    while (steps_left > 0)
    {
        if (check_timer((*sp)->last_step_time, (*sp)->step_delay))
        {
            start_timer(&(*sp)->last_step_time);

            if ((*sp)->direction == 1)
            {
                (*sp)->step_num++;
                if ((*sp)->step_num == (*sp)->num_steps)
                    (*sp)->step_num = 1;
            }
            else
            {
                printf("%d\n", (*sp)->step_num);
                if ((*sp)->step_num == 0)
                    (*sp)->step_num = (*sp)->num_steps;

                (*sp)->step_num--;
            }
            steps_left--;
            step_motor(sp, (*sp)->step_num % 4);
        } 
    }
}
