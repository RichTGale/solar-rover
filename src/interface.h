/**
 * interface.h
 *
 * This file contains the public data-structure and function prototype declarations
 * for the interface type, as well as enumeration definitions for it.
 *
 * Version: 0.2.1
 * Author(s): Richard Gale
 */

#ifndef interface_h
#define interface_h

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "drive.h"
#include "rack.h"
#include "mycutils.h"
#include "rpiutils.h"

/**
 * These are the commands that can be sent to the interface.
 */
enum InterfaceCommand {
    NO_INTERFACE_COMMAND,
    TERMINATE,
    START_SCREEN_ON,
    DRIVE_SCREEN_ON,
    RACK_SCREEN_ON
};

/**
 * This is the data structure of the commands type.
 */
typedef struct {
    enum InterfaceCommand interface_command;
    enum DriveCommand drive_command;
    enum RackCommand rack_command;
} commands; 

/**
 * This is the data-structure of the interface type.
 */
typedef struct interface_data* interface;

/**
 * This function initialises the interface provided to it.
 */
void interface_init(interface* ip);

/**
 * This function terminates the interface provided to it.
 */
void interface_term(interface* ip);

/**
 * This function gets input from the user and returns it.
 */
char interface_get_user_in();

/**
 * This function builds a set of commands based on user input and the
 * current screen that is on.
 */
void interface_build_commands(interface* ip, commands* cmdsp, char user_in);

/**
 * This function updates the interface.
 */
void interface_update(interface* ip, enum InterfaceCommand interface_command);

/**
 * This function displays the interface.
 */
void interface_display(interface i, drive d, rack r);

#endif
