/**
 * interface.c
 *
 * This file contains the internal data-structure and function definitions
 * for the interface type.
 *
 * Version: 0.1.1
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
    int min_width;          /* The minimum width of the interface. */
    int min_height;         /* The minimum height of the interface. */
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

    /* Set the minimum width and height of the interface. */
    (*ip)->min_width = 100;
    (*ip)->min_height = 25;


    /* Check if terminal is large enough to display the interface. */
    if ((get_res()).x < (*ip)->min_width || (get_res()).y < (*ip)->min_height)
    {
        /* The terminal window is not large enough so print an error
         * message and exit the program. */
        fprintf(stdout, "The interface failed to initialise because "
                        "the terminal window is not large enough. "
                        "It needs to have a minimum size of 100x25.\n");
        exit(EXIT_FAILURE);
    }
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
 * This function gets input from the user and returns it.
 */
char interface_get_user_in()
{
    return scanc_nowait();
}

/**
 * This function builds a set of commands associated with the start screen.
 */
void build_start_commands(commands* cmdsp, char user_in)
{
    /* Check what the user input. */
    switch (user_in)
    {
        /* Terminate the app. */
	    case 'q' :
            (*cmdsp).interface_command = TERMINATE;
            break;

        /* Show the drive screen. */
        case 'd'  :
            (*cmdsp).interface_command = DRIVE_SCREEN_ON;
            break;

        /* Show the rack screen. */
        case 'r'  :
           (*cmdsp).interface_command = RACK_SCREEN_ON;
            break;
    }
}

/**
 * This function builds a set of commands associated with the drive screen.
 */
void build_drive_commands(commands* cmdsp, char user_in)
{
    /* Checking what the user input. */
    switch (user_in)
    {
        /* Accelerate the rover. */
        case 'w' :
            (*cmdsp).drive_command = ACCELERATE; 
            break;

        /* Turn the rover left. */
        case 'a' :
            (*cmdsp).drive_command = TURN_LEFT;
            break;

        /* Decellerate the rover. */
        case 's' :
            (*cmdsp).drive_command = DECELERATE;
            break;

        /* Turn the rover right. */
        case 'd' :
            (*cmdsp).drive_command = TURN_RIGHT;
            break;

        /* Stop the rover. */
        case 'x' :
            (*cmdsp).drive_command = STOP_DRIVE;
            break;

        /* Turn on the start screen. */
        case 'q' :
            (*cmdsp).interface_command = START_SCREEN_ON;
            (*cmdsp).drive_command = STOP_DRIVE;
            break;
    }
}

/**
 * This function builds a set of commands associated with the rack screen.
 */
void build_rack_commands(commands* cmdsp, char user_in)
{
    /* Checking what the user input. */
    switch (user_in)
    {
        /* Rotate the rack clockwise on its x axis. */
        case 'w' :
            (*cmdsp).rack_command = X_CLOCKWISE; 
            break;

        /* Rotate the rack anticlockwise on its z axis. */
        case 'a' :
            (*cmdsp).rack_command = Z_ANTICLOCKWISE;
            break;

        /* Rotate the rack anticlockwise on its x axis. */
        case 's' :
            (*cmdsp).rack_command = X_ANTICLOCKWISE;
            break;

        /* Rotate the rack clockwise on its z axis. */
        case 'd' :
            (*cmdsp).rack_command = Z_CLOCKWISE;
            break;

        /* Turn on the start screen. */
        case 'q' :
            (*cmdsp).interface_command = START_SCREEN_ON;
            break;
    }
}

/**
 * This function builds a set of commands based on user input and the
 * current screen that is on.
 */
void interface_build_commands(interface* ip, commands* cmdsp, char user_in)
{
    /* Initialise commands. */
    *cmdsp = (commands) {
        .interface_command = NO_INTERFACE_COMMAND,
        .drive_command = NO_DRIVE_COMMAND,
        .rack_command = NO_RACK_COMMAND
    };

    /* Check if the start-screen is on. */
    if ((*ip)->start_screen_on)
    {
        /* Store start screen commands. */
        build_start_commands(cmdsp, user_in);
    }

    /* Checking if the drive screen is on. */
    else if ((*ip)->drive_screen_on)
    {
        /* Store drive screen command. */
        build_drive_commands(cmdsp, user_in);
    }

    /* Checking if the rack screen is on. */
    else if ((*ip)->rack_screen_on)
    {
        build_rack_commands(cmdsp, user_in);
    }
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
 * This function displays the string provided to it at the top and center
 * of the terminal.
 */
void display_screen_title_str(char* title)
{
    vec2d pos;  /* The position of the title. */

    /* Set the position of the title. */
    pos.x = (get_res()).x / 2 - strlen(title) / 2;
    pos.y = 1;

    /* Display the title. */
    print_str_mod(title, pos, WHITE, BOLD);
}

/**
 * This function displays a string at the bottom and center of the terminal.
 */
void display_controls(interface i, char* controls)
{
    vec2d pos;      /* The position of the control instructions. */
    vec2d term_res; /* The size of the terminal. */

    term_res = get_res();

    /* Set the control instruction location. */
    pos.x = term_res.x / 2 - strlen(controls) / 2;
    pos.y = term_res.y - 1;
    if (term_res.y > i->min_height + 1)
        pos.y = i->min_height + 1;

    /* Print the control instructions. */
    print_str_mod(controls, pos, WHITE, BOLD);
}

/**
 * This function displays the start screen.
 */
void display_start_screen(interface i)
{
    const int TITLE_WIDTH = 45;     /* The width of the title. */
    const int TITLE_HEIGHT = 16;    /* The height of the title. */
    rpi_info info;                  /* Information about the raspberry pi. */
    vec2d title_pos;                /* The position of the program title. */
    vec2d info_pos;                 /* The position of the rpi information. */
    vec2d controls_pos;             /* The position of the control instructions. */
    vec2d term_res;                 /* The resolution of the terminal. */
    char* controls;                 /* The control instructions. */

    /* Get the bounds of the terminal. */
    term_res = get_res();

    /* Set the position of the title. */
    title_pos.x = term_res.x / 2 - TITLE_WIDTH / 2;
    title_pos.y = 1;

    /* Draw the program title. */
    print_fs_mod("../../art/title.txt", title_pos, WHITE, BOLD);

    /* Set the position of the rpi info. */
    info_pos.x = title_pos.x;
    info_pos.y = TITLE_HEIGHT + 1;

    /* Print information about the raspberry pi this program is running on. */
    get_rpi_info(&info);
    print_rpi_info(info, info_pos);

    /* Create the control instructions. */
    strfmt(&controls, "'d': Drive | 'r': Rack | 'q': Quit");

    /* Set the location of the control instructions. */
    controls_pos.x = term_res.x / 2 - strlen(controls) / 2;
    controls_pos.y = term_res.y - 1;
    if (term_res.y > i->min_height + 1)
        controls_pos.y = i->min_height + 1;

    /* Printing the message. */
    print_str_mod(controls, controls_pos, WHITE, BOLD);

    /* Place the cursor in the top, right hand corner. */
    put_cursor(term_res.x, 0);

    /* De-allocating memory. */
    free(controls);
}

/** 
 * This function visualises the duty cycle of a motor. 
 */
void display_drive_bar(char* label, int duty_cycle, vec2d pos )
{
    vec2d label_pos;    /* The position of the bar's label. */
    vec2d outline_pos;  /* The position of the bar's outline. */
    char* bar;          /* The bar. */
 
    /* Create the bar. */
    strfmt(&bar, "     ");
    
    /* Set the location of the bar outline and display it. */
    outline_pos.x = pos.x - strlen(bar) / 2;
    outline_pos.y = pos.y - 5;
    print_fs_mod("../../art/drive_bar.txt", outline_pos, WHITE, BOLD);

    /* Set the location of the label and display it. */
    label_pos.x = pos.x - strlen(label) - strlen(bar) - 1;
    label_pos.y = pos.y; 
    print_str_mod(label, label_pos, WHITE, NORMAL);

    /* Set the bar colour. */ 
    text_bcol(YELLOW);

    /* Display the bar. */
    for (int i = 0; i < abs(duty_cycle) / 25; i++)
    {
        if (duty_cycle > 0 )
            print_str(bar, (vec2d) { outline_pos.x + 1, pos.y - 1 - i });
        else if (duty_cycle < 0)
            print_str(bar, (vec2d) { outline_pos.x + 1, pos.y + 1 + i });
    }

    /* Revert text colour changes. */
    text_mode(NORMAL);

    /* De-allocating memory. */
    free(bar);
}

/**
 * This function displays the drive screen.
 */
void display_drive_screen(interface i, drive d)
{
    vec2d term_res;     /* The size of the terminal. */
    vec2d lmotor_pos;   /* The position of the left motor bar. */
    vec2d rmotor_pos;   /* The position to place the right motor bar. */
    vec2d controls_pos; /* The position of the control instructions. */

    /* Get the size of the terminal. */
    term_res = get_res();

    /* Display the screen title. */
    display_screen_title_str("Drive Screen");

    /* Set the location of the drive bars. */
    lmotor_pos = (vec2d) { term_res.x / 3, term_res.y / 3 };
    rmotor_pos = (vec2d) { term_res.x / 3 * 2, term_res.y / 3 };

    /* Display the drive bars. */ 
    display_drive_bar("Motor 1", drive_get_lmotor_duty_cycle(d), lmotor_pos);
    display_drive_bar("Motor 2", drive_get_rmotor_duty_cycle(d), rmotor_pos);
    
    display_controls(i, "'w': Accelerate | 'a': Left | 's': Decelerate | "
                        "'d': Right | 'x': Stop | 'q': Start Screen");

    /* Place the cursor in the top, right hand corner. */
    put_cursor(term_res.x, 0);
}

/**
 * This function displays the rack screen.
 */
void display_rack_screen()
{
    vec2d bounds;   /* The size of the terminal. */
    
    /* Display the title of the screen. */
    display_screen_title_str("Rack Screen");
    
    /* Get the terminal size. */
    bounds = get_res();

    /* Place the cursor in the top, right hand corner. */
    put_cursor(bounds.x, 0);
}

/**
 * This function displays the interface.
 */
void interface_display(interface i, drive d, rack r)
{
    /* Clear the terminal. */
    clear();

    /* Check which screen to display and display it. */
    if (i->start_screen_on) display_start_screen(i);
    else if (i->drive_screen_on) display_drive_screen(i, d);
    else if (i->rack_screen_on) display_rack_screen();
}

