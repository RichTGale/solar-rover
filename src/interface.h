/**
 * interface.h
 *
 * This file contains the public data-structure and function prototype declarations
 * for the interface type, as well as enumeration definitions for it.
 *
 * Version: 0.1.0
 * Author: Richard Gale
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
 * This function creates and returns a commands based on user input.
 */
commands interface_input_command(interface* ip);

/**
 * This function updates the interface.
 */
void interface_update(interface* ip, enum InterfaceCommand interface_command);

/**
 * This function displays the interface.
 */
void interface_display(interface i, drive d, rack r);

#endif
