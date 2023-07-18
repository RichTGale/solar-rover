

#include "term.h"



struct term_data {
    coord2D origin;
    coord2D bounds;
};

void term_init( term* tp )
{
    // Allocating memory.
    *tp = ( term ) malloc( sizeof( struct term_data ) );
    
    // Initialising the origin of the terminal window.
    ( *tp )->origin.x = 0;
    ( *tp )->origin.y = 0;
    
    // Initialising the boundaries of the terminal window.
    term_res( tp );
}

void term_free( term* tp )
{
    free( *tp );
}

coord2D term_getbounds( term t )
{
    return t->bounds;
}

coord2D term_getorigin( term t )
{
    return t->origin;
}

void term_clear()
{
    /* Clearing the terminal. */
    system( "tput clear" );
}

void placecursor( int x, int y )
{
    char* cmd;   // The command

    /* Creating the command. */
    stringf(&cmd, "tput cup %d %d", y, x);

    /* Setting the cursor position. */
    system(cmd);

    /* De-allocating memory. */
    free(cmd); 
}

/**
 * This function stores the number of rows and columns of the terminal
 * in the bounds property of the term provided to it.
 */
void term_res( term* tp )
{
    FILE* rfp;      /* File stream for the rows file. */
    FILE* cfp;      /* File stream for the columns file. */
    char rbuf[5];   /* The number of rows. */
    char cbuf[5];   /* The number of columns. */

    /* Creating a temporary directory to store the files. */
    system("if [ ! -d temp/ ]; then\nmkdir temp/\nfi");

    /* Writing the number of rows and columns to their files. */
    system("tput lines >> temp/screen_rows.txt");
    system("tput cols >> temp/screen_cols.txt");

    /* Opening the files. */
    rfp = open_file("temp/screen_rows.txt", "r" );
    cfp = open_file( "temp/screen_cols.txt", "r" );

    /* Getting the number of rows and columns from the files. */
    fgets(rbuf, sizeof(rbuf), rfp);
    fgets(cbuf, sizeof(cbuf), cfp);

    /* Converting the number of rows and columns to integers. */
    (*tp)->bounds.x = atoi(rbuf); //strtol( cbuf, &end, 10 );
    (*tp)->bounds.y = atoi(cbuf); //strtol( rbuf, &end, 10 );

    /* Closing the files. */
    close_file(rfp);
    close_file(cfp);

    /* Deleting the files. */
    system("rm temp/screen_rows.txt");
    system("rm temp/screen_cols.txt");
}

void textmode(enum textmodes m)
{
    switch (m) 
    {
        case BOLD       : system( "tput bold" ); break;
        case NORMAL     : system( "tput sgr0" ); break;
        case BLINK      : system( "tput blink" ); break;
        case REVERSE    : system( "tput smso" ); break;
        case UNDERLINE  : system( "tput smul" ); break;
    }
}

/**
 * This function sets the background colour of the terminal cursor.
 */
void bgcolour( enum termcolours c )
{
    char* command;  // The command

    /* Creating the command. */
    stringf(&command, "tput setab %d", c);

    /* Setting the background colour. */
    system(command);

    /* De-allocating memory. */
    free(command);
}

/**
 * This function sets the foreground colour of the temrinal cursor.
 */
void fgcolour( enum termcolours c )
{
    char* command;   // The command

    /* Creating the command. */
    stringf(&command, "tput setaf %d", c);
    
    /* Setting the foreground colour. */
    system(command);

    /* De-allocating memory. */
    free(command);
}

/**
 * This function draw a single row of an art file.
 */
void drawline(char* text, size_t text_len, coord2D origin, coord2D bounds)
{
    int c;  // The current column of the row

    /* Moving the cursor to the origin. */
    placecursor(origin.x, origin.y);
    
    /* Drawing the row. */
    for (c = 0; c < text_len && c < bounds.x; c++)
    {
        /* Checking if there should be something drawn in theis column. */
        if (text[c] == '1')
        {
            /* Drawing a filled space. */
            bgcolour(WHITE);
            system("printf \" \"");
        }
        else
        {
            /* Drawing an empty space. */
            system("tput cuf1");
        }
    }
}

void drawfile(char* filepath, coord2D origin, coord2D bounds)
{
    FILE* fp;   /* Pointer to the file stream. */
    char* line; /* The text in the file. */
   
    /* Ensuring that the buffer is set to NULL. */
    line = NULL;
    
    /* Opening the file. */ 
    fp = open_file(filepath, "r");
   
    /* Reading the line from the file. */ 
	while (read_fileln(fp, &line)) 
    {
        /* Drawing the line. */
        drawline(line, strlen(line), origin, bounds);

        /* Getting ready to draw the next line. */
        origin.y++;
        free(line);
        line = NULL;
    }

    /* Closing the file. */
    close_file( fp );
}

void drawstr(char* str, coord2D origin, coord2D bounds)
{
    char* filepath;
    int c;

    for (c = 0; c < strlen(str); c++)
    {
        stringf(&filepath, "../../art/%c.txt", str[c]);
        drawfile(filepath, origin, bounds);
        origin.x += CHAR_WIDTH;
        free(filepath);
    }
}


void printstr(char* str, coord2D origin)
{
    char* command;

    placecursor(origin.x, origin.y);
    stringf(&command, "printf \"%s\"", str);
    system(command);
    free(command);
}
