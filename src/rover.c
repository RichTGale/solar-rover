/**
 * rover.c
 *
 * This file contains the internal data-structure and function definitions
 * for the rover type.
 *
 * Version: 0.1.1
 * Author: Richard Gale
 */

#include "rover.h"

/**
 * This is the internal data of the rover data-type.
 */
struct rover_data {
    interface i;            /* Allows a user to control the rover. */
    drive d;                /* Controls the movement of the driving motors. */
    rack r;                 /* Controls the movement of the rack motors. */
    bool is_running;        /* Whether the rover is running. */
};

/**
 * This function initialises the rover supplied to it.
 */
void rover_init(rover* rp)
{
    /* Allocate memory to the rover. */
    fprintf(stdout, " - Allocating memory...\n");
    *rp = (rover) malloc(sizeof(struct rover_data));

    /* Set up pi-gpio. */
    fprintf(stdout, " - Setting up pi-gpio...\n");
    setup();

    /* Initialise rover properties. */
    fprintf(stdout, " - Setting up the interface...\n");
    interface_init(&(*rp)->i);
    fprintf(stdout, " - Setting up the drive...\n");
    drive_init(&(*rp)->d);
    fprintf(stdout, " - Setting up the rack...\n");
    rack_init(&(*rp)->r);
    (*rp)->is_running = true;
}

/**
 * This function terminates the rover supplied to it.
 */
void rover_term(rover* rp)
{
    /* Terminate the rover properties. */
    fprintf(stdout, " - Terminating the rack...\n");
    rack_term(&(*rp)->r);
    fprintf(stdout, " - Terminating the drive...\n");
    drive_term(&(*rp)->d);
    fprintf(stdout, " - Terminating the interface...\n");
    interface_term(&(*rp)->i);

    /* De-allocate memory from the rover. */
    fprintf(stdout, " - De-allocating memory...\n");
    free(*rp);
}

/**
 * This function updates the rover.
 */
void update(rover* rp)
{
    commands cmds;  /* Commands for the rover to execute. */
    char user_in;   /* The user input. */
    
    /* Get user input. */
    user_in = interface_get_user_in();
    
    /* Build a set of commands. */
    interface_build_commands(&(*rp)->i, &cmds, user_in);

    /* Update the interface. */
    fprintf(stdout, " - Updating the interface...\n");
    interface_update(&(*rp)->i, cmds.interface_command);

    /* Update the drive. */
    fprintf(stdout, " - Updating the drive...\n");
    drive_update(&(*rp)->d, cmds.drive_command);

    /* Update the rack. */
    fprintf(stdout, " - Updating the rack...\n");
    rack_update(&(*rp)->r, cmds.rack_command);

    switch (cmds.interface_command)
    {
        case TERMINATE :
            (*rp)->is_running = false;
            break;
    }
}

void display(rover r)
{
    /* Display the interface. */
    fprintf(stdout, " - Drawing the interface...\n");
    interface_display(r->i, r->d, r->r);
}

/**
 * This function runs the rover supplied to it.
 */
void rover_exec(rover* rp)
{
    struct timespec end_last_frame; /* The time at the end of the last frame. */

    /* Because no frames have run yet, initialise the time at the end of the
     * last frame to be now. */
    start_timer(&end_last_frame);

    /* Draw the interface in its initial state. */
    fprintf(stdout, " - Drawing the interface in its initial state...\n");
    interface_display((*rp)->i, (*rp)->d, (*rp)->r);

    /* Check if the rover is still running. */
    while((*rp)->is_running)
    {
        /* Check if it's time to run a frame. */
        if (check_timer(end_last_frame, NANOS_PER_FRAME))
        {
            /* Update the rover. */
            update(rp);

            /* Display the rover. */
            display(*rp);
        }
    }
}

