/**
 * interface.c
 *
 * This file contains the internal data-structure and function definitions
 * for the interface type.
 *
 * Version: 0.1.0
 * Author(s): Richard Gale
 */

#include "interface.h"

/**
 * This is the internal data of the interface type.
 */
struct interface_data {
    bool start_screen_on;   /* Whether the start screen is on. */
    bool drive_screen_on;   /* Whether the drive screen is on. */
    bool rack_screen_on;    /* Whether the rack screen is on. */
};

/**
 * This is the data-structure of the interface type.
 */
void interface_init(interface* ip)
{
    /* Allocate memory to the interface. */
    *ip = (interface) malloc(sizeof(struct interface_data));

    /* Start with the start-screen on. */
    (*ip)->start_screen_on = true;
    (*ip)->drive_screen_on = false;
    (*ip)->rack_screen_on = false;
}

/**
 * This function terminates the interface provided to it.
 */
void interface_term(interface* ip)
{
    /* De-allocate memory from the interface. */
    free(*ip);
}

/**
 * This function returns commands based on user input.
 */
commands interface_input_command(interface* ip)
{
    commands cmds;  /* The commands we will return. */
    char user_in;   /* The user input. */

    /* Initialise commands. */
    cmds = (commands) {
        .interface_command = NO_INTERFACE_COMMAND,
        .drive_command = NO_DRIVE_COMMAND,
        .rack_command = NO_RACK_COMMAND
    };

    /* Get user input. */
    user_in = scanc_nowait();

    /* Check if the start-screen is on. */
    if ((*ip)->start_screen_on)
    {
        /* Check what the user input. */
        switch (user_in)
        {
            /* Terminate the app. */
	        case 'q' :
                cmds.interface_command = TERMINATE;
                break;

            /* Show the drive screen. */
            case 'd'  :
                cmds.interface_command = DRIVE_SCREEN_ON;
                break;
            
            /* Show the rack screen. */
            case 'r'  :
                cmds.interface_command = RACK_SCREEN_ON;
                break;
        }
    }

    /* Checking if the drive screen is on. */
    else if ((*ip)->drive_screen_on)
    {
        /* Checking what the user input. */
        switch (user_in)
        {
            /* Accelerate the rover. */
            case 'w' :
                cmds.drive_command = ACCELERATE; 
                break;

            /* Turn the rover left. */
            case 'a' :
                cmds.drive_command = TURN_LEFT;
                break;

            /* Decellerate the rover. */
            case 's' :
                cmds.drive_command = DECELERATE;
                break;

            /* Turn the rover right. */
            case 'd' :
                cmds.drive_command = TURN_RIGHT;
                break;

            /* Stop the rover. */
            case 'x' :
                cmds.drive_command = STOP_DRIVE;
                break;
            
            /* Turn on the start screen. */
            case 'q' :
                cmds.interface_command = START_SCREEN_ON;
                cmds.drive_command = STOP_DRIVE;
                break;
        }
    }

    /* Checking if the rack screen is on. */
    else if ((*ip)->rack_screen_on)
    {
        /* Checking what the user input. */
        switch (user_in)
        {
            /* Rotate the rack clockwise on its x axis. */
            case 'w' :
                cmds.rack_command = X_CLOCKWISE; 
                break;

            /* Rotate the rack anticlockwise on its z axis. */
            case 'a' :
                cmds.rack_command = Z_ANTICLOCKWISE;
                break;

            /* Rotate the rack anticlockwise on its x axis. */
            case 's' :
                cmds.rack_command = X_ANTICLOCKWISE;
                break;

            /* Rotate the rack clockwise on its z axis. */
            case 'd' :
                cmds.rack_command = Z_CLOCKWISE;
                break;

            /* Turn on the start screen. */
            case 'q' :
                cmds.interface_command = START_SCREEN_ON;
                break;
        }
    }
    return cmds;
}

/**
 * This function updates the interface.
 */
void interface_update(interface* ip, enum InterfaceCommand interface_command)
{
    switch (interface_command)
    {
        case TERMINATE :
            (*ip)->start_screen_on = false;
            (*ip)->drive_screen_on = false;
            (*ip)->rack_screen_on = false;
            break;
        case START_SCREEN_ON :
            (*ip)->start_screen_on = true;
            (*ip)->drive_screen_on = false;
            (*ip)->rack_screen_on = false;
            break;
        case DRIVE_SCREEN_ON :
            (*ip)->start_screen_on = false;
            (*ip)->drive_screen_on = true;
            (*ip)->rack_screen_on = false;
            break;
        case RACK_SCREEN_ON :
            (*ip)->start_screen_on = false;
            (*ip)->drive_screen_on = false;
            (*ip)->rack_screen_on = true;
            break;
        case NO_INTERFACE_COMMAND:
            NULL;
            break;
    }
}

/**
 * This function displays the start screen.
 */
void display_start_screen()
{
    rpi_info info;  /* Information about the raspberry pi. */
    vec2d origin;   /* The cursor position. */
    vec2d bounds;   /* The size of the terminal. */
    char* message;  /* A message. */

    /* Start with the cursor in the top left corner. */
    origin.x = 1;
    origin.y = 1;

    /* Get the bounds of the terminal. */
    bounds = termres();

    /* Create the message. */
    strfmt(&message, "'d': Drive | 'r': Rack | 'q': Quit");

    /* Draw the program title. */
    textmode(NORMAL);
    termprintfs("../../art/title.txt", &origin, WHITE, NORMAL);

    /* Print information about the raspberry pi this program is running on. */
    cursput(origin.x, origin.y);
    get_rpi_info(&info);
    print_rpi_info(info);

    /* Printing the message. */
    origin.x = bounds.x / 2 - (strlen(message) / 2);
    origin.y = bounds.y - 1;
    textmode(BOLD);
    termprint(message, origin);
    textmode(NORMAL);

    /* De-allocating memory. */
    free(message);
}

/** 
 * This function visualises the duty cycle of a motor. 
 */
void display_drive_bar(char* label, int duty_cycle, vec2d location )
{
    vec2d label_location;   /* The location of the label. */
    vec2d outline_location; /* The location of the outline. */
    char* bar;              /* The bar. */
 
    /* Create the bar. */
    strfmt(&bar, "     ");

    /* Display the label. */
    label_location = (vec2d) { location.x - strlen(label) - 1, location.y };
    termprint(label, label_location);

    /* Set the location of the bar outline and display it. */
    outline_location = (vec2d) { location.x - 1, location.y - 5 };
    termprintfs("../../art/drive_bar.txt", &outline_location, WHITE, NORMAL);

    /* Set the bar colour to red. */ 
    curscolb(RED);

    /* Display the bar. */
    for (int i = 0; i < abs(duty_cycle) / 25; i++)
    {
        if (duty_cycle > 0 )
            termprint(bar, (vec2d) { location.x, location.y - 1 - i } );
        else if (duty_cycle < 0)
            termprint(bar, (vec2d) { location.x, location.y + 1 + i } );
    }

    /* Revert text changes. */
    textmode(NORMAL);

    /* De-allocating memory. */
    free(bar);
}

/**
 * This function displays the drive screen.
 */
void display_drive_screen(drive d)
{
    vec2d bounds;               /* The size of the terminal. */
    vec2d lmotor_location;      /* The location of the left motor bar. */
    vec2d rmotor_location;      /* The location to place the right motor bar. */
    vec2d controls_location;    /* The location of the control instructions. */
    char* title;                /* The name of the screen. */
    char* controls;             /* The control instructions. */
    
    /* Get the size of the terminal. */
    bounds = termres();

    /* Create the screen title. */
    strfmt(&title, "Drive Screen");

    /* Display the screen title. */
    termprint(title, (vec2d) { bounds.x / 2 - strlen(title), 1 });

    /* Set the location of the drive bars. */
    lmotor_location = (vec2d) { bounds.x / 4, 10 };
    rmotor_location = (vec2d) { bounds.x / 4 * 3, 10 };
    
    /* Display the drive bars. */ 
    display_drive_bar("Motor 1", drive_get_lmotor_duty_cycle(d), lmotor_location);
    display_drive_bar("Motor 2", drive_get_rmotor_duty_cycle(d), rmotor_location);

    /* Create the control instructions. */
    strfmt(&controls, 
            "'w': Accelerate | 'a': Left | 's': Decelerate | 'd': Right | "
            "'q': Start Screen");

    /* Set the control instruction location. */
    controls_location.x = bounds.x / 2 - strlen(controls) / 2;
    controls_location.y = bounds.y - 1;

    /* Print the control instructions. */
    termprint(controls, controls_location);

    /* De-allocating memory. */
    free(title);
    free(controls);
}

/**
 * This function displays the rack screen.
 */
void display_rack_screen()
{

}

/**
 * This function displays the interface.
 */
void interface_display(interface i, drive d, rack r)
{
    /* Clear the terminal. */
    termclear();

    /* Check which screen to display and display it. */
    if (i->start_screen_on) display_start_screen();
    else if (i->drive_screen_on) display_drive_screen(d);
    else if (i->rack_screen_on) display_rack_screen();
}

