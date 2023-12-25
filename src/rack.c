/**
 * rack.c
 *
 * This file contains the internal data-structure and function definitions
 * for the rack type.
 *
 * Version: 0.1.2
 * Author(s): Richard Gale
 */

#include "rack.h"

typedef struct {
    int x;
    int z;
    bool visited;
} position;

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

    position positions[7];
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

    (*rp)->positions[0].x =  (*rp)->max_x;
    (*rp)->positions[0].z =  (*rp)->max_z;
    (*rp)->positions[0].visited = false;

    (*rp)->positions[1].x =  (*rp)->max_x;
    (*rp)->positions[1].z =  0;
    (*rp)->positions[1].visited = false;

    (*rp)->positions[2].x =  (*rp)->max_x;
    (*rp)->positions[2].z = -(*rp)->max_z;
    (*rp)->positions[2].visited = false;

    (*rp)->positions[3].x =  0;
    (*rp)->positions[3].z =  0;
    (*rp)->positions[3].visited = false;

    (*rp)->positions[4].x = -(*rp)->max_x;
    (*rp)->positions[4].z = -(*rp)->max_z;
    (*rp)->positions[4].visited = false;

    (*rp)->positions[5].x = -(*rp)->max_x;
    (*rp)->positions[5].z =  0;
    (*rp)->positions[5].visited = false;

    (*rp)->positions[6].x = -(*rp)->max_x;
    (*rp)->positions[6].z = (*rp)->max_z;
    (*rp)->positions[6].visited = false;
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
        /* Rotate the z axis. */
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
 * This function returns the position of the rack that is closest to the
 * postion passed to the function.
 */
position get_closest_position(rack* rp, position current)
{
    /* This is the closest position to the current position. */
    position closest;

    /* The x axis takes a lot longer to move than the z axis. The x axis' motor
     * takes 568 steps per degree, compared to the z axis' motor which only
     * takes 9.
     * This variable maps the number of degrees the x axis needs to rotate to
     * the number the z axis needs to rotate. */
    int mapped_x;

    /* This is the minimum distance in degrees that both axes summed need to
     * travel to reach another position that has not yet been reached */
    int min;

    /* This is the index of the position being checked. */
    int p;

    /* This is the sum of the movement of both axes. */
    int distance;

    /* This is the index of the position that was closest to the current
     * position passed to this function. */
    int closest_index;

    /* Calculate the mapped maximum distance the x axis will have to move. */
    mapped_x = 
        map((*rp)->max_x * 2 * 1.0, 0, (*rp)->max_x * 2, 0, (*rp)->max_z * 2);

    /* Calculate the maximum distance both axes will have to move combined.
     * This is the sum of both axes maximum potential movement. */
    min = mapped_x + ((*rp)->max_z * 2);

    /* Work out the next closest position. */
    for (p = 0; p < 7; p++)
    {
        /* Don't check previously visited positions. */
        if (!(*rp)->positions[p].visited)
        {
            /* Map the x axis for the current position. */
            mapped_x = abs((*rp)->positions[p].x - current.x);
            mapped_x = map(mapped_x * 1.0, 0, (*rp)->max_x, 0, (*rp)->max_z);

            /* Calculate the sum of the movement of both axes in degrees. */
            distance = mapped_x + abs((*rp)->positions[p].z - current.z);
            
            /* Check if the sum of movement in degrees of both axes is less
             * or equal to the previously calculated position that was closest
             * to the current position. */
            if (distance <= min)
            {
                /* This is the position that is the closest to the current
                 * position so record everything. */
                min = distance;
	            closest.x = (*rp)->positions[p].x;
	            closest.z = (*rp)->positions[p].z;
                closest_index = p;
            }
        }
    }

    /* Mark the closest postion as checked. */
    (*rp)->positions[closest_index].visited = true;

    /* Return the closest position. */
    return closest;
}


/**
 * This function moves the rack so its solar panels are pointing in the
 * direction of the brightest light.
 */
void light_search(rack* rp)
{
    /* This is the current position of the rack. */
    position current;

    /* This is the position of the rack that is pointed towards the brightest
     * bit of light. */
    position brightest;

    /* Get the current position of the rack. */ 
    current.x = get_degree_of_rotation("../../cur_x.txt");
    current.z = get_degree_of_rotation("../../cur_z.txt");


    /* Work out the brightest position. */
    for (int i = 0; i < 7; i++)
    {
        /* Get the next closest position to the current position. */
        current = get_closest_position(rp, current);

        /* Move to the next position. */
        rotate_axis(rp, 'x', current.x);
        rotate_axis(rp, 'z', current.z);

        
        /* Read the light sensor. */
        if (ldr_read((*rp)->l))
        {
            /* Record the brightest reading. */
            brightest.x = current.x;
            brightest.z = current.z;
        }
    }
    
    /* Move to the position of the brightest position. */
    rotate_axis(rp, 'x', brightest.x);
    rotate_axis(rp, 'z', brightest.z);

    /* Write the brightest position to disk. */
    store_degree_of_rotation("../../cur_x.txt", brightest.x);
    store_degree_of_rotation("../../cur_z.txt", brightest.z);

    /* Reset all positions to unvisited in preparation for the next search. */
    for (int p = 0; p < 7; p++)
    {
        (*rp)->positions[p].visited = false;
    }
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
