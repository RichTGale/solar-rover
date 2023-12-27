#include "button.h"

struct button_data {
	int pin;
	uint64_t debounce_time;
	unsigned long count;
	int count_mode;
	
	int previous_steady_state;  // the previous steady state from the input pin, used to detect pressed and released event
	int last_steady_state;      // the last steady state from the input pin
    int last_flickerable_state; // the last flickerable state from the input pin
	
	struct timespec last_debounce_time; // the last time the output pin was toggled
    
};

void button_init(button* bp, int pin, int mode)
{
    *bp = (button) malloc(sizeof(struct button_data));

    (*bp)->pin = pin;
    (*bp)->debounce_time = 0;
    (*bp)->count = 0;
    (*bp)->count_mode = COUNT_FALLING;

    setup_gpio((*bp)->pin, INPUT, mode);

	(*bp)->previous_steady_state = input_gpio((*bp)->pin);
	(*bp)->last_steady_state = (*bp)->previous_steady_state;
	(*bp)->last_flickerable_state = (*bp)->previous_steady_state;

	start_timer(&(*bp)->last_debounce_time);
}

void button_term(button* bp)
{
    free(*bp);
}

void set_debounce_time(button* bp, uint64_t time)
{
	(*bp)->debounce_time = time;
}

int get_state(button b)
{
	return b->last_steady_state;
}

int get_state_raw(button b)
{
	return input_gpio(b->pin);
}

bool is_pressed(button b)
{
	if(b->previous_steady_state == HIGH && b->last_steady_state == LOW)
		return true;
	else
		return false;
}

bool is_released(button b)
{
	if(b->previous_steady_state == LOW && b->last_steady_state == HIGH)
		return true;
	else
		return false;
}

void set_count_mode(button* bp, int mode)
{
	(*bp)->count_mode = mode;
}

unsigned long get_count(button b)
{
	return b->count;
}

void reset_count(button* bp)
{
	(*bp)->count = 0;
}

void loop(button* bp)
{	
    // read the state of the switch/button:
	int current_state = input_gpio((*bp)->pin);

	struct timespec current_time;

    start_timer(&current_time);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch/button changed, due to noise or pressing:
	if (current_state != (*bp)->last_flickerable_state) 
    {
		// reset the debouncing timer
		(*bp)->last_debounce_time = current_time;
		// save the the last flickerable state
		(*bp)->last_flickerable_state = current_state;
	}

	if (check_timer((*bp)->last_debounce_time, (*bp)->debounce_time))
    {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// save the the steady state
		(*bp)->previous_steady_state = (*bp)->last_steady_state;
		(*bp)->last_steady_state = current_state;
	}

	if((*bp)->previous_steady_state != (*bp)->last_steady_state){
		if((*bp)->count_mode == COUNT_BOTH)
			(*bp)->count++;
		else if((*bp)->count_mode == COUNT_FALLING)
        {
			if((*bp)->previous_steady_state == HIGH && (*bp)->last_steady_state == LOW)
				(*bp)->count++;
		}
		else if((*bp)->count_mode == COUNT_RISING)
        {
			if((*bp)->previous_steady_state == LOW && (*bp)->last_steady_state == HIGH)
				(*bp)->count++;
		}
	}
}
