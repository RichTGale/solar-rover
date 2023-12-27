/**
 * rack.c
 *
 * This file contains the internal data-structure and function definitions
 * for the rack type.
 *
 * Version: 0.3.0
 * Author(s): Richard Gale
 */

#include "rack.h"

/**
 * This is the data-structure of a rack position.
 */
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

    /* The limit switch. */
    button limit_switch;
    
    /* This is an array of positions. */
    position* positions;

    /* This is the amount of positions the rack can be in. */
    int num_positions;

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
    int p, x, z;

    /* Allocate memory to the rack. */
    *rp = (rack) malloc(sizeof(struct rack_data));

    /* Initialise motors. */
    stepper_motor_init(&(*rp)->zmotor, 2048, 26, 20, 19, 16);
    stepper_motor_steps_per_sec(&(*rp)->zmotor, 400);
    stepper_motor_init(&(*rp)->xmotor, 2048, 22, 10, 24, 9);
    stepper_motor_steps_per_sec(&(*rp)->xmotor, 400);

    /* Initialise light the light dependant resistor. */
    ldr_init(&(*rp)->l, 14, 15, 18);

    /* Initialise the limit switch. */
    button_init(&(*rp)->limit_switch, 21, 2);
    button_set_debounce_time(&(*rp)->limit_switch, 50000000);

    /* Initialise the maximum degress of rotation. */
    (*rp)->max_x = 25;
    (*rp)->max_z = 90;

    /* Initialise the current angle of the x axis. */
    (*rp)->cur_x = get_degree_of_rotation("../../cur_x.txt");

    /* Initialise the current angle of the z axis. */
    (*rp)->cur_z = get_degree_of_rotation("../../cur_z.txt");

    /* Allocate memory to the array of positions. */
    (*rp)->num_positions = 7;
    (*rp)->positions = 
        (position*) malloc(sizeof(position) * (*rp)->num_positions);

    /* Initialise the positions. */
    p = 0;
    for (x = -(*rp)->max_x; x <= (*rp)->max_x; x += (*rp)->max_x)
    {
        for (z = -(*rp)->max_z; z <= (*rp)->max_z; z += (*rp)->max_z)
        {
            if (x == 0 && z != 0)
            {
                /* Don't use these positions. */
            }
            else
            {
                (*rp)->positions[p].x = x;
                (*rp)->positions[p].z = z;
                (*rp)->positions[p].visited = false;
                p++;
            }
        }
    }
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

    /* Terminate the limit switch. */
    button_term(&(*rp)->limit_switch);

    /* De-allocate memory from the rack. */
    free(*rp);
}

/**
 * Forward declaration.
 */
void rotate_z_1degree(rack* rp, enum RotationDirection direction);

/**
 * This function uses a limit switch to rotate the z axis to -90 degrees.
 */
void reset_z(rack* rp)
{
    /* Rotate the z axis to -90 degrees. */
    while (button_get_state((*rp)->limit_switch) == HIGH)
    {
        button_update(&(*rp)->limit_switch);
        rotate_z_1degree(rp, ANTICLOCKWISE);
    }
    button_update(&(*rp)->limit_switch);
    (*rp)->cur_z = -(*rp)->max_z;
    
    /* Write the z axis' position to disk. */
    store_degree_of_rotation("../../cur_z.txt", (*rp)->cur_z);
}

/**
 * This function rotates the rack provided to it by one degree on its z axis.
 */
void rotate_z_1degree(rack* rp, enum RotationDirection direction)
{
    /* Rotate by one degree. */
    if (direction == CLOCKWISE)
    {
        stepper_motor_step(&(*rp)->zmotor, ONE_DEGREE_Z);
        (*rp)->cur_z++;
    }
    else if (direction == ANTICLOCKWISE)
    {
        if (button_get_state((*rp)->limit_switch) == HIGH)
        {
            stepper_motor_step(&(*rp)->zmotor, -ONE_DEGREE_Z);
            (*rp)->cur_z--;
        }
        else
        {
            reset_z(rp);
        }
    }
}

/**
 * This function rotates the rack provided to by one degree it on its x axis.
 */
void rotate_x_1degree(rack* rp, enum RotationDirection direction )
{
    /* Rotate by one degree. */
    if (direction == CLOCKWISE)
    {
        stepper_motor_step(&(*rp)->xmotor, ONE_DEGREE_X);
        (*rp)->cur_x++;
    }
    else if (direction == ANTICLOCKWISE)
    {
        stepper_motor_step(&(*rp)->xmotor, -ONE_DEGREE_X);
        (*rp)->cur_x--;
    }
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

    /* This is the name of the file that stores the current rotation
     * of the axis. */
    char* fname;
    
    /* Get references to the correct axis. */
    if (axis == 'x')
        dcurrent = &(*rp)->cur_x;
    else
        dcurrent = &(*rp)->cur_z;
    
    /* Calculate the number of degrees the axis needs to rotate. */
    dtotal = abs(*dcurrent - target);

    /* Determine which direction to rotate. */
    if (*dcurrent < target)
        dir = CLOCKWISE;
    else
        dir = ANTICLOCKWISE;

    /* Rotate the axis. */
    for (dcount = 0; dcount < dtotal; dcount++)
    {
        if (axis =='x')
            rotate_x_1degree(rp, dir);
        else
            rotate_z_1degree(rp, dir);
    }

    /* Create the file name. */
    strfmt(&fname, "../../cur_%c.txt", axis);

    /* Write the current degree of rotation to a file. */
    store_degree_of_rotation(fname, *dcurrent);

    /* De-allocate memory from the file name. */
    free(fname);
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
        map((*rp)->max_x * 2 * 1.0, 
            0, (*rp)->max_x * 2, 
            0, (*rp)->max_z * 2 * ONE_DEGREE_X);

    /* Calculate the maximum distance both axes will have to move combined.
     * This is the sum of both axes maximum potential movement. */
    min = mapped_x + ((*rp)->max_z * 2 * ONE_DEGREE_Z);

    /* Work out the next closest position. */
    for (p = 0; p < 7; p++)
    {
        /* Don't check previously visited positions. */
        if (!(*rp)->positions[p].visited)
        {
            /* Map the x axis for the current position. */
            mapped_x = abs((*rp)->positions[p].x - current.x);
            mapped_x = 
                map(mapped_x * 1.0,
                    0, (*rp)->max_x,
                    0, (*rp)->max_z * ONE_DEGREE_X);

            /* Calculate the sum of the movement of both axes in degrees. */
            distance = mapped_x 
                + (abs((*rp)->positions[p].z - current.z) * ONE_DEGREE_Z);
            
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

    reset_z(rp);

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
        printf("%d of 7: ", i + 1);

        /* Read the light sensor. */
        if (ldr_read((*rp)->l))
        {
            /* Record the brightest reading. */
            brightest.x = current.x;
            brightest.z = current.z;
            printf("brightest recorded so far\n");
        }
        else
        {
            printf("dimmer\n");
        }
    }

    printf("moving to the brightest postion\n");
    
    /* Move to the brightest position. */
    rotate_axis(rp, 'x', brightest.x);
    rotate_axis(rp, 'z', brightest.z);

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
