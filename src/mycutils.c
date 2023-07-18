/**
 * mycutils.c
 *
 * This file contains the definitions of various utility functions.
 *
 * Author: Richard Gale
 * Version: 16th July, 2023
 */

#include "mycutils.h"

/********************************* Time **************************************/

/**
 * This function returns true if a number of nano-seconds equal to or greater
 * than wait_time has elapsed since start.
 */
bool check_timer(struct timespec start, uint64_t wait_time)
{
    struct timespec current;    // The current time
    struct timespec elapsed;    // The time elapsed since start
    bool has_elapsed = false;   // Whether the time has elapsed

    /* Obtaining the current time. */
    clock_gettime(CLOCK_REALTIME, &current);

    /* Calculating the elapsed time. */
    elapsed.tv_sec = current.tv_sec - start.tv_sec;
    elapsed.tv_nsec = current.tv_nsec - start.tv_nsec;

    /* Checking whether the time has elapsed. */
    if ((elapsed.tv_sec * NANOS_PER_SEC) + elapsed.tv_nsec >= wait_time)
    {
        has_elapsed = true; // The designated time has elapsed.
    }

    /* Returning whether the designated time has elapsed. */
    return has_elapsed;
}

/**
 * This function obtains the current time and stores it in the timespec
 * that was provided to it.
 */
void start_timer(struct timespec* ts)
{
    /* Obtaining the current time.*/
    if ((clock_gettime(CLOCK_REALTIME, ts)) == -1)
    {
        /* An error occured so we are printing it to stderr. */
        fprintf(stderr, 
                "[ %s ] ERROR: in function start_timer(): %s\n",
                timestamp(), strerror(errno));
    }
}

/**
 * This function returns a string that represent the current time.
 */
char* timestamp()
{
    time_t current_time;    // The current time
    char* stamp;            // The time stamp

    /* Obtaining the current time. */
    if ((current_time = time(NULL)) == ((time_t) - 1))
    {
        /* There was an error obtaining the time so we're printing 
         * a message to stderr and exiting the program. */
        fprintf(stderr, 
                "ERROR: In function timestamp(): "
                "Calender time is not available\n");
        exit(EXIT_FAILURE);
    }

    /* Converting time to local time format. */
    if ((stamp = ctime(&current_time)) == NULL)
    {
        /* There was an error converting the time to a string so we're
         * printing a message to stderr and exiting the program. */
        fprintf(stderr, 
                "ERROR: In function timestamp(): "
                "Failure to convert the current time to a string.\n");
        exit(EXIT_FAILURE);
    }

    /* Removing the newline character that was added by ctime(). */
    stringrmc(&stamp, '\n');

    /* Returning the time stamp. */
    return stamp;
}

/******************************** In/Out *************************************/


/**
 * This function asks the user to input a char in response to a prompt supplied
 * to it, then stores it in the supplied char pointer. 
 */
void read_userc(char* cp, char* prompt)
{
    /* Printing the prompt. */
    fprintf(stdout, "%s", prompt);

    /* Getting the user's input. */
    scanf("%c", cp);
}

/**
 * This function returns a char that was input by the user. It doesn't wait
 * for the user to press enter. (Not my code)
 */
char read_userc_nowait() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

/**
 * This function closes the file stream provided tp it. If there is an error,
 * it is printed on stderr and the program will exit.
 */
void close_file(FILE* fp)
{
    /* Closing the file stream. */
    if (fclose(fp) != 0)
    {
        /* There was an error closing the file stream so we are printing it
         * on stderr and exiting the program. */
        fprintf(stderr,
                "[ %s ] ERROR: In function close_file: %s\n", 
                timestamp(), strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * This function opens a file that has a name that matches fname. It opens the
 * file in the mode specified by mode.
 * If there is an error it will be printed on stderr and the program 
 * is exited. If the file is successfully opened, this function
 * will return a pointer to the file stream.
 */
FILE* open_file(char* fname, char* mode)
{
    FILE* fp;   // The pointer to the file stream.

    /* Opening the file. */
    if ((fp = fopen(fname, mode)) == NULL)
    {
        /* There was an error opening the file so wea re printing the error to
         * stderr and exiting the program. */
        fprintf(stderr, 
                "[ %s ] ERROR: In function open_file(): "
                "Could not open file %s: %s\n",
                timestamp(), fname, strerror(errno));
        exit(EXIT_FAILURE);
    }

    /* Returning the pointer to the file stream. */
    return fp;
}

/**
 * This function assigns the next char in the file stream provided to it to
 * the buffer provided to it. It returns true on success or false if EOF is
 * reached. It will exit the program if an error occurs.
 */
bool read_filec(FILE* fstreamp, char* buf)
{
    const bool SUCCESS = 1;     // Return value if success
    const bool END_OF_FILE = 0; // Return value if EOF
    bool status;                // Whether the char was read successfully

    /* Presuming the character was read successfully. */
    status = SUCCESS;

    /* Reading the char. */
    if ((*buf = fgetc(fstreamp)) == EOF)
    {
        /* Checking if an error occured. */
        if (ferror(fstreamp))
        {
            /* Printing an error message and exiting the program. */
            fprintf(stderr,
                    "[ %s ] ERROR: In function read_filec(): %s\n",
                    timestamp(), strerror(errno));
            exit(EXIT_FAILURE);
        }
        /* EOF was reached. */
        status = END_OF_FILE;
    }

    /* Returning whether the char was read successfully. */
    return status;
}


/**
 * This function assigns the next line in the file stream provided to it to
 * the string provided to it. It returns true if the line was read successfully
 * or false if EOF was reached. If an error occurs the program will exit.
 * Make sure to free() the buffer when you're finished with it.
 */
bool read_fileln(FILE* fstreamp, char** buf)
{
    const bool SUCCESS = 1;     // Return value if success
    const bool END_OF_FILE = 0; // Return value if EOF
    bool status;                // Whether the line was read successfully
    size_t n;                   // Allocated size of the buffer

    /* Initialising how big the buffer is. */
    n = 0;

    /* Presuming the character was read successfully. */
    status = SUCCESS;
    
    /* Reading the next line from the file. */
    if (getline(buf, &n, fstreamp) == -1)
    {
        /* Checking if an error occured. */
        if (ferror(fstreamp))
        {
            /* Printing an error message and exiting the program. */
            fprintf(stdout,
                    "[ %s ] ERROR: In function read_fileln: %s\n",
                    timestamp(), strerror(errno));
            exit(EXIT_FAILURE);
        }
        /* EOF was reached. */
        status = END_OF_FILE;
    }

    /* Returning whether the line was read successfully. */
    return status;
}

/**
 * This function writes the char provided to it to the file stream provided to
 * it.
 */
void write_filec(FILE* fstreamp, char ch)
{
    /* Writing the char to the file stream. */
    fprintf(fstreamp, "%c", ch); 
}

/**
 * This function writes the string provided to it to the file steam provided
 * to it.
 */
void write_str(FILE* fstreamp, char* str)
{
    int c;  // Index of the current char in the string

    /* Writing the string to the file stream. */
    for (c = 0; c < strlen(str); c++)
        write_filec(fstreamp, str[c]);
}

/******************************** Strings ************************************/

/**
 * This function dynamically allocates only the needed amount of memory to a
 * string based on the argument list, then concatenates the argument list into 
 * the supplied format and stores it in the supplied string pointer.
 */
void stringf(char** sptr, char *fmt, ...)
{
    va_list lptr;       // Pointer to the list of arguments
    va_list lptr_cpy;   // A Copy of the list of arguments
    size_t bytes;       // The number of bytes the string needs

    /* Pointing to the first argument. */
    va_start(lptr, fmt);

    /* Copying the argument list. */
    va_copy(lptr_cpy, lptr);

    /* Getting the number of bytes the string will need. Adding
     * 1 for the null byte. */
    bytes = vsnprintf(NULL, 0, fmt, lptr_cpy) + 1;

    /* Assuring a clean finish to the copy. */
    va_end(lptr_cpy);

    /* Allocating memory to the string. */
    *sptr = (char*) malloc(bytes);

    /* Creating the string. */
    vsnprintf(*sptr, bytes, fmt, lptr);

    /* Assuring a clean finish to the argument list. */
    va_end(lptr);
}

/**
 * This function removes all cases of the provided char from the string at the
 * provided pointer.
 */
void stringrmc(char** str, char remove)
{
    int len = strlen(*str);   // The original length of the string.
    int total_chars = strlen(*str);   // The current length of the string.
    int i; // An indexer
    char* src; // The address of where to start moving the memory.
    char* dst; // The address of where to move the memory to.

    /* Overwriting the unwanted character. */
    for (i = 0; i < len; i++)
    {
        if ((*str)[i] == remove)
        {
            /* Setting the source and destinations points for moving. */
            src = &((*str)[i + 1]);
            dst = &((*str)[i]);

            /* Overwriting an unwanted character. */
            memmove(dst, src, 
                    (sizeof(char) * strlen(*str)) - (sizeof(char) * i));

            /* Decrementing the index so we will check the replacement 
             * character. */
            i--;

            /* Recording the new length of the string. */
            total_chars--;
        }
    }

    /* Designating the end of the string. */
    (*str)[total_chars] = '\0';
}






