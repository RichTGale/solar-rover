/**
 * rack.c
 *
 * This file contains the internal data-structure and function definitions
 * for the rack type.
 *
 * Version: 0.1.1
 * Author(s): Richard Gale
 */

#include "rack.h"

/**
 * This is the internal data-structure of the Rack type.
 */
struct rack_data {
    
    /* This is the stepper motor that controls the rack's z axis. */
    stepper_motor zmotor;

    /* This is the stepper motor that controls the rack's x axis. */
    stepper_motor xmotor;

    /* The light dependant resistor. */
    ldr l;

    /* This is the maximum angle the x axis can rotate to. */
    int max_x;

    /* This is the maximum angle the z axis can rotate to. */
    int max_z;

    /* This is the current angle of the x axis. */
    int cur_x;

    /* This is the current angle of the z axis. */
    int cur_z;
};

/**
 * This function returns the degree of rotation of the rack's axis that
 * corresponds to the file passed to the function.
 */
int get_degree_of_rotation(char* fname)
{
    /* This is the file stream. */
    FILE* fs;

    /* This stores the string read from the file. */
    char* buf;

    /* This is the number of degrees the axis is rotated. */
    int degree;
    
    /* Read the file and connvert the number it stores to an integer. */
    fs = openfs(fname, "r");
    readfsl(fs, &buf);
    degree = atoi(buf);

    /* De-allocate memory and close the file. */
    free(buf);
    closefs(fs);

    /* Return the degree of rotation the axis is rotated to. */
    return degree;
}

/**
 * This function writes the degree of rotation of the axis corresponding to
 * the file past to the funciton.
 */
void store_degree_of_rotation(char* fname, int degree)
{
    /* This is the file stream. */
    FILE* fs;

    /* This stores the degree value as a string. */
    char* buf;
    
    /* Convert the degree to a string and write it to the file. */
    strfmt(&buf, "%d", degree);
    fs = openfs(fname, "w");
    writefss(fs, buf);

    /* Close the file and de-allocate memory. */
    closefs(fs);
    free(buf);
}

/**
 * This function initialises the rack provided to it.
 */
void rack_init(rack* rp)
{
    /* Allocate memory to the rack. */
    *rp = (rack) malloc(sizeof(struct rack_data));

    /* Initialise motors. */
    stepper_motor_init(&(*rp)->zmotor, 2048, 26, 20, 19, 16);
    stepper_motor_steps_per_sec(&(*rp)->zmotor, 400);
    stepper_motor_init(&(*rp)->xmotor, 2048, 22, 10, 24, 9);
    stepper_motor_steps_per_sec(&(*rp)->xmotor, 400);

    /* Initialise light the light dependant resistor. */
    ldr_init(&(*rp)->l, 14, 15, 18);

    /* Initialise the maximum degress of rotation. */
    (*rp)->max_x = 25;
    (*rp)->max_z = 90;

    /* Initialise the current angle of the x axis. */
    (*rp)->cur_x = get_degree_of_rotation("../../cur_x.txt");

    /* Initialise the current angle of the z axis. */
    (*rp)->cur_z = get_degree_of_rotation("../../cur_z.txt");
}

/**
 * This function terminates the rack provided to it.
 */
void rack_term(rack* rp)
{
    /* Terminate the stepper_motors. */
    stepper_motor_term(&(*rp)->zmotor);
    stepper_motor_term(&(*rp)->xmotor);

    /* Terminate the light dependant resistor. */
    ldr_term(&(*rp)->l);

    /* De-allocate memory from the rack. */
    free(*rp);
}

/**
 * This function rotates the rack provided to it by one degree on its z axis.
 */
void rotate_z_1degree(rack* rp, enum RotationDirection direction)
{
    /* The number of teeth on the stepper gear is 62, the number on the
     * internal gear is 95.
     * Gear ratio = 95:62 = 1.532258065:1.
     * 2048 steps * 1.532258065 = ~3138 steps per internal gear revolution.
     * 3138 steps / 360 degress = ~9 steps per degree of the internal gear. */
    const int ONE_DEGREE = 9;

    /* Rotate by one degree. */
    if (direction == CLOCKWISE)
        stepper_motor_step(&(*rp)->zmotor, ONE_DEGREE);
    else if (direction == ANTICLOCKWISE)
        stepper_motor_step(&(*rp)->zmotor, -ONE_DEGREE);
}

/**
 * This function rotates the rack provided to by one degree it on its x axis.
 */
void rotate_x_1degree(rack* rp, enum RotationDirection direction )
{
    /* Judging from the 3d models simulations in blender, 7.5 revolutions
     * of the worm gear equals 1 revolution of the spur gear.
     * Gear ratio = 7.5:1. 
     * 2048 steps * 7.5 = 15360 steps per revolution of the spur gear. 
     * There are 200 teeth on the internal gear (if it was whole) and there
     * are 15 teeth on the spur gear. 
     * Gear ratio = 200:15 = 13.33333:1.
     * 15360 steps * 13.33333 = ~204,800 steps per internal gear revolution.
     * 204800 steps / 360 degress = ~568 steps per degree of the internal
     * gear. */
    const int ONE_DEGREE = 568;

    /* Rotate by one degree. */
    if (direction == CLOCKWISE)
        stepper_motor_step(&(*rp)->xmotor, ONE_DEGREE);
    else if (direction == ANTICLOCKWISE)
        stepper_motor_step(&(*rp)->xmotor, -ONE_DEGREE);
}

/**
 * This function rotates the axis corresponding to the char passed to
 * the function to the target angle that is also passed to the function.
 */
void rotate_axis(rack* rp, char axis, int target)
{
    /* This is the direction of rotation. */
    enum RotationDirection dir;

    /* This is the total number of degrees to rotate. */
    int dtotal;

    /* This keeps track of the number of degrees the axis has rotated. */
    int dcount;

    /* This is the current angle. */
    int* dcurrent;

    /* This is the maximum degree of rotation. */
    int* dmax;
    
    /* Get references to the correct axis. */
    if (axis == 'x')
    {
        dcurrent = &(*rp)->cur_x;
        dmax = &(*rp)->max_x;
    }
    else
    {
        dcurrent = &(*rp)->cur_z;
        dmax = &(*rp)->max_z;
    }
    
    /* Calculate the number of degrees the axis needs to rotate. */
    dtotal = abs(*dcurrent - target);

    /* Determine which direction to rotate. */
    if (*dcurrent < target)
        dir = CLOCKWISE;
    else
        dir = ANTICLOCKWISE;

    /* Check which axis to rotate. */
    if (axis == 'x')
    {
        /* Rotate the x axis. */
        for (dcount = 0; dcount < dtotal; dcount++)
        {
            rotate_x_1degree(rp, dir);
            if (dir == CLOCKWISE)
                (*dcurrent)++;
            else
                (*dcurrent)--;
        }
        
        /* Write the current degree of rotation to a file. */
        store_degree_of_rotation("../../cur_x.txt", *dcurrent);
    }
    else
    {
        /* Rotate the y axis. */
        for (dcount = 0; dcount < dtotal; dcount++)
        {
            rotate_z_1degree(rp, dir);
            if (dir == CLOCKWISE)
                (*dcurrent)++;
            else
                (*dcurrent)--;
        }
        
        /* Write the current degree of rotation to a file. */
        store_degree_of_rotation("../../cur_z.txt", *dcurrent);
    }
}

/**
 * This function moves the rack so its solar panels are pointing in the
 * direction of the most light.
 */
void light_search(rack* rp)
{
    vec2d highest;
    int x, z;
    bool skip;

	for (x = -(*rp)->max_x; x <= (*rp)->max_x; x += (*rp)->max_x)
    {
        skip = false;
        rotate_axis(rp, 'x', x);
	    for (z = -(*rp)->max_z; z <= (*rp)->max_z && !skip; z += (*rp)->max_z)
        {
            /* Only do one reading when the x axis id flat. */
            if (x == 0)
            {
                rotate_axis(rp, 'z', 0);
                z = 0;
                skip = true;
            }

            /* Do three readings when x axis is angled. */
            else
            {
                rotate_axis(rp, 'z', z);
            }

            if (ldr_read((*rp)->l))
            {
                highest.x = x;
                highest.y = z;
            }
        }
    }

    /* Go to the position of the highest reading. */
    rotate_axis(rp, 'x', highest.x);
    rotate_axis(rp, 'z', highest.y);
}

/**
 * This function updates the rack provided to it.
 */
void rack_update(rack* rp, enum RackCommand rack_command)
{
    switch (rack_command)
    {
        case X_CLOCKWISE :
            rotate_x_1degree(rp, CLOCKWISE);
            break;
        case X_ANTICLOCKWISE :
            rotate_x_1degree(rp, ANTICLOCKWISE);
            break;
        case Z_CLOCKWISE :
            rotate_z_1degree(rp, CLOCKWISE);
            break;
        case Z_ANTICLOCKWISE :
            rotate_z_1degree(rp, ANTICLOCKWISE);
            break;
        case LIGHT_SEARCH:
            light_search(rp);
            break;
        case NO_RACK_COMMAND :
            NULL;
            break;
    }
}
