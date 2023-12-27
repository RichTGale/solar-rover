#include "button.h"

/**
 * This is the internal data-structure of the button type.
 */
struct button_data {

    /* This is the raspberry pi gpio pin corresponding with the button. */
    int pin;

    /* This is the amount of time it takes to bebounce the button. */
    uint64_t debounce_time;
    
    /* This is the previous steady state from the input pin. It is used to
     * detect pressed and released events. */
	int previous_steady_state;  

    /* This is the last steady state from the input pin. */
	int last_steady_state;      

    /* This is the last flickerable state from the input pin. */
    int last_flickerable_state; 
	
    /* This is the last time the output pin was toggled. */
	struct timespec last_debounce_time; 
    
};

/**
 * This function initialises the button provided to it.
 */
void button_init(button* bp, int pin, int mode)
{
    /* Allocate memory to the button. */
    *bp = (button) malloc(sizeof(struct button_data));

    /* Set up the raspberry pi gpio pin that corresponds with the button. */
    setup_gpio((*bp)->pin, INPUT, mode);

    /* Initialise the buttons internal data. */
    (*bp)->pin = pin;
    (*bp)->debounce_time = 0;
	(*bp)->previous_steady_state = input_gpio(pin);
	(*bp)->last_steady_state = (*bp)->previous_steady_state;
	(*bp)->last_flickerable_state = (*bp)->previous_steady_state;
	start_timer(&(*bp)->last_debounce_time);
}

/**
 * This function terminates the button provided to it.
 */
void button_term(button* bp)
{
    /* De-allocate memory from the button. */
    free(*bp);
}

/**
 * This function sets the amount of time to wait before debouncing the button
 * provided to it.
 */
void button_set_debounce_time(button* bp, uint64_t time)
{
	(*bp)->debounce_time = time;
}

/**
 * This function returns the last recorded state the button was in.
 */
int button_get_state(button b)
{
	return b->last_steady_state;
}

/**
 * This function returns the current state of the button.
 */
int button_get_state_raw(button b)
{
	return input_gpio(b->pin);
}

/**
 * This function returns true if the button is currently pressed.
 */
bool button_is_pressed(button b)
{
    bool is_pressed;

	if(b->previous_steady_state == HIGH && b->last_steady_state == LOW)
		is_pressed = true;
	else
		is_pressed = false;

    return is_pressed;
}

/**
 * This function returns true if the button is currently not pressed.
 */
bool button_is_released(button b)
{
    bool is_pressed;

	if(b->previous_steady_state == LOW && b->last_steady_state == HIGH)
		is_pressed = true;
	else
		is_pressed = false;

    return is_pressed;
}

/**
 * This function updates the button.
 */
void button_update(button* bp)
{	
	int current_state;  /* The current state of the button. */

    /* Get the current state of the button. */ 
	current_state = button_get_state_raw(*bp);

	/* Check whether the button has changed states. */
	if (current_state != (*bp)->last_flickerable_state) 
    {
		/* Reset the debouncing timer. */
        start_timer(&(*bp)->last_debounce_time);

		/* Record the the last flickerable state. */
		(*bp)->last_flickerable_state = current_state;
	}

    /* Check whether the debounce time has elapsed. */
	if (check_timer((*bp)->last_debounce_time, (*bp)->debounce_time))
    {
		/* Whatever the reading is at, it's been there for longer than the
         * debounce delay, so take it as the actual current state and record
         * it. */
		(*bp)->previous_steady_state = (*bp)->last_steady_state;
		(*bp)->last_steady_state = current_state;
	}
}
