#include "ldr.h"

struct ldr_data {
    int send_pin;
    int read_pin1;
    int read_pin2;
};

void ldr_init(ldr* lp, int send_pin, int read_pin1, int read_pin2)
{
    *lp = (ldr) malloc(sizeof(struct ldr_data));
    (*lp)->send_pin = send_pin;
    (*lp)->read_pin1 = read_pin1;
    (*lp)->read_pin2 = read_pin2;

    setup_gpio((*lp)->send_pin, OUTPUT, 0);
    setup_gpio((*lp)->read_pin1, INPUT, 0);
    setup_gpio((*lp)->read_pin2, INPUT, 0);

    output_gpio((*lp)->send_pin, LOW);
    output_gpio((*lp)->read_pin1, LOW);
    output_gpio((*lp)->read_pin2, LOW);
}

void ldr_term(ldr* lp)
{
    free(*lp);
}

bool ldr_read(ldr l)
{
    bool highest = false;

    do
    {
        /* Send HIGH to arduino (pin 14). This will tell the arduino that we're
         * ready for it to make a reading. */
        output_gpio(l->send_pin, HIGH);
   
    /* Wait for HIGH from arduino (pin 15). This tells us that the arduino made a reading. */
    } while (input_gpio(l->read_pin1) == LOW);
  
    output_gpio(l->send_pin, LOW);

    do
    {
        /* Wait for HIGH from arduino (pin 18). This tells us that the reading
         * detected the most light out of any reading so far. */
        if (input_gpio(l->read_pin2) == HIGH)
        {
            highest = true;
        }
    
    /* Wait for LOW from arduino (pin 15). This tells us that the highest reading was checked. */
    } while (input_gpio(l->read_pin1) == HIGH);

    return highest;
}
