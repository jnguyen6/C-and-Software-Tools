/**
 * Program that reads the commands from user input and perform
 * specific tasks based on the given commands. The program
 * supports the following commands: add, remove, move, list,
 * nearby, match, help, and quit. The user can add points of
 * interests to the list of points, remove points of interests
 * from the list of points, move the user's current location,
 * display a list of all the points of interests currently in
 * the list, display a list of points of interests that are near
 * the user's current location, display a list of points of
 * interests that contains the given word, display a list of
 * valid commands, and quit the program.
 *
 * @file attractions.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "pointlist.h"
#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/** The default latitude value. */
#define DEFAULT_LAT 35.772325
/** The default longitude value. */
#define DEFAULT_LON -78.673581
/** The maximum length of the word to match. */
#define MAX_WORD_LENGTH 20
/** Value used to convert ASCII uppercase letters to lowercase letters. */
#define TO_LOWERCASE 32
/** The user's current location. */
static Coords currentLocation = { DEFAULT_LAT, DEFAULT_LON };
 
 /**
  * Function that reports the invalid command message to standard
  * output.
  */
void printInvalidCommandMessage( ) {
    scanf( "%*[^\n]%*c" );
    printf( "\nInvalid command\n" );
}

/**
 * A list command function that always returns true, since the list
 * command is intended to list all the Points in the PointList.
 *
 * @param pt the pointer to the Point
 * @param data the pointer to the data value, which should be NULL
 * @return true since it is intended for the list command to list
 * all the Points in the PointList
 */
static bool list( Point const *pt, void *data )
{
    return true;
}

/**
 * A nearby command function that returns true if the given Point
 * is within the given distance from the user's current location,
 * or false otherwise.
 *
 * @param pt the pointer to the Point
 * @param data the pointer to the data value, which is the distance
 * from the user's current location
 * @return true if the given Point is within the given distance from
 * the user's current location or false otherwise
 */
static bool nearby( Point const *pt, void *data )
{
    double *distance = data;
    return globalDistance( &pt->location, &currentLocation ) <= *distance;
}

/**
 * A match command function that returns true if the given Point's
 * description that contains the given word or false otherwise.
 *
 * @param pt the pointer to the Point
 * @param data the pointer to the data value, which is the word to
 * match the word in the Point's description
 * @return true if the given Point's description contains the given
 * word or false otherwise
 */
static bool match( Point const *pt, void *data )
{
    int count = 0;
    char *wordToMatch = data;
    char wordFromDesc[ MAX_WORD_LENGTH + 1 ];
    for ( int i = 0; pt->desc[ i ]; i++ ) {
        //If the word is longer than the expected max length of the
        //word to match, then the word is invalid So, reset the
        // wordFromDesc array, and keep on iterating through the point
        // description until a space or a comma has been reached.
        if ( count >= MAX_WORD_LENGTH && 
              ( pt->desc[ i ] != ' ' && pt->desc[ i ] != ',' 
                 && pt->desc[ i ] != '-' && pt->desc[ i ] != '/' ) ) {
            wordFromDesc[ count ] = '\0';
            for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                wordFromDesc[ j ] = '\0';
            }
            while ( pt->desc[ i ] != ' ' && pt->desc[ i ] != ',' 
                   && pt->desc[ i ] != '-' && pt->desc[ i ] != '/' ) {
                i++;
            }
            count = 0;
            
        //If we reached a comma, and the word parsed is a valid length,
        //then start comparison
        } else if ( pt->desc[ i ] == ' ' || pt->desc[ i ] == ',' 
                    || pt->desc[ i ] == '-' || pt->desc[ i ] == '/' ) {
            wordFromDesc[ count ] = '\0';
            bool matchFound = true;
            
            //First, do comparison. If we have found even one mismatch,
            //then the word is not a valid match.
            if ( strlen( wordFromDesc ) != strlen( wordToMatch ) ) {
                matchFound = false;
            }
            for ( int j = 0; wordToMatch[ j ]; j++ ) {
                if ( wordFromDesc[ j ] != wordToMatch[ j ] 
                      && wordFromDesc[ j ] + TO_LOWERCASE != wordToMatch[ j ] ) {
                    matchFound = false;
                }
            }
            
            //If count is 0, that means we reached ', '.
            if ( matchFound && count != 0 ) {
                for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                    wordFromDesc[ j ] = '\0';
                }
                count = 0;
                return true;
            }
            for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                wordFromDesc[ j ] = '\0';
            }
            count = 0;
            matchFound = true;
        } else {
            wordFromDesc[ count ] = pt->desc[ i ];
            count++;
        }
    }
    //If we reached this point, that means we got to the null-terminator of
    //the description. But, we need to make one last comparison just in case
    if ( count < MAX_WORD_LENGTH ) {
        wordFromDesc[ count ] = '\0';
        bool matchFound = true;
            
        //First, do comparison. If we have found even one mismatch,
        //then the word is not a valid match.
        if ( strlen( wordFromDesc ) != strlen( wordToMatch ) ) {
            matchFound = false;
        }
        for ( int j = 0; wordToMatch[ j ]; j++ ) {
            if ( wordFromDesc[ j ] != wordToMatch[ j ] 
                    && wordFromDesc[ j ] + TO_LOWERCASE != wordToMatch[ j ] ) {
                matchFound = false;
            }
        }
            
        //If count is 0, that means we reached ', '.
        if ( matchFound && count != 0 ) {
            for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                wordFromDesc[ j ] = '\0';
            }
            count = 0;
            return true;
        }
        for ( int j = 0; wordFromDesc[ j ]; j++ ) {
            wordFromDesc[ j ] = '\0';
        }
    }
    for ( int j = 0; wordFromDesc[ j ]; j++ ) {
        wordFromDesc[ j ] = '\0';
    }
    return false;
}

/**
 * Function that executes the add command. If the point is null
 * or is a duplicate, then an invalid command message is displayed.
 * Otherwise, the point is added to the pointlist.
 *
 * @param ptlist the pointer to the point list
 */
void executeAddCommand( PointList *ptlist )
{
    Point *pt = parsePoint( );
    if ( pt == NULL ) {
        printInvalidCommandMessage( );
    } else if ( !addPoint( ptlist, pt ) ) {
        freePoint( pt );
        printInvalidCommandMessage( );
    } else {
        printf( "\n" );
    }
}

/**
 * Function that executes the remove command. If the given point
 * name is invalid or is too long, then an invalid command message
 * is displayed. Otherwise, the point is removed from the pointlist.
 *
 * @param ptlist the pointer to the pointlist
 */
void executeRemoveCommand( PointList *ptlist )
{
    char name[ MAX_NAME_LENGTH + 1 ];
    int matches = scanf( "%20s", name );
    if ( matches == EOF ) {
        freePointList( ptlist );
        printf( "\n" );
        exit( EXIT_SUCCESS );
    } else if ( matches != 1 || scanf( "%*[^\n\t]" ) == 1 ) {
        printInvalidCommandMessage( );
    } else {
        if ( !removePoint( ptlist, name ) ) {
            printInvalidCommandMessage( );
        } else {
            printf( "\n" );
        }
    }
}

/**
 * Function that executes the move command. If the given latitude
 * and/or longitude values is invalid, then an invalid command
 * message is displayed. Otherwise, the user's current location
 * is updated.
 *
 * @param ptlist the pointer to the pointlist
 */
void executeMoveCommand( PointList *ptlist )
{
    double lat;
    double lon;
    int matches = scanf( "%lf %lf", &lat, &lon );
    if ( matches == EOF ) {
        freePointList( ptlist );
        printf( "\n" );
        exit( EXIT_SUCCESS );
    } else if ( matches != 2 ) {
        printInvalidCommandMessage( );
    } else {
        currentLocation.lat = lat;
        currentLocation.lon = lon;
        printf( "\n" );
    }
}

/**
 * Function that executes the list command. If there are
 * any additional input read, then an invalid command message
 * is displayed. Otherwise, the function will display a list of
 * all the points in the pointlist.
 *
 * @param ptlist the pointer to the pointlist
 */
void executeListCommand( PointList *ptlist )
{
    if ( scanf( "%*[^\n\t]" ) == 1 ) {
        printInvalidCommandMessage( );
    } else {
        listPoints( ptlist, &currentLocation, list, NULL );
        printf( "\n" );
    }
}

/**
 * Function that executes the nearby command. If the given distance
 * is invalid, or if there are any additional input read after the
 * distance, then an invalid command message is displayed. Otherwise,
 * the function will display a list of all the points in the pointlist
 * that is within the given distance, in miles.
 *
 * @param ptlist the pointer to the pointlist
 */
void executeNearbyCommand( PointList *ptlist )
{
    double distance;
    int matches = scanf( " %lf", &distance );
    if ( matches != 1 ) {
        printInvalidCommandMessage( );
    } else if ( scanf( "%*[^\n\t]" ) == 1 ) {
        printInvalidCommandMessage( );
    } else {
        listPoints( ptlist, &currentLocation, nearby, &distance );
        printf( "\n" );
    }
}

/**
 * Function that executes the match command. If the given word is
 * too long or is invalid, then an invalid command message is displayed.
 * Otherwise, the function will display a list of all the points in the
 * pointlist that contains the given word.
 *
 * @param ptlist the pointer to the pointlist
 */
void executeMatchCommand( PointList *ptlist )
{
    char word[ MAX_WORD_LENGTH + 1 ];
    int matches = scanf( " %20[a-z]", word );
    if ( matches != 1 ) {
        printInvalidCommandMessage( );
    } else if ( scanf( "%*[^\n\t]" ) == 1 ) {
        printInvalidCommandMessage( );
    } else {
        listPoints( ptlist, &currentLocation, match, word ); 
        printf( "\n" );
    }
}

/**
 * Function that executes the list command. A list of all the commands
 * the program supports is displayed.
 */
void executeHelpCommand( )
{
    printf( "\nadd <name> <latitude> <longitude> <description>\n" );
    printf( "remove <name>\n" );
    printf( "move <latitude> <longitude>\n" );
    printf( "list\n" );
    printf( "nearby <distance>\n" );
    printf( "match <word>\n" );
    printf( "help\n" );
    printf( "quit\n" );
}

/**
 * Function that executes the quit command. The given pointlist is
 * freed from memory, and the program exits successfully.
 *
 * @param ptlist the pointer to the pointlist
 */
void executeQuitCommand( PointList *ptlist )
{
    freePointList( ptlist );
    printf( "\n" );
    exit( EXIT_SUCCESS );
}
 
/**
 * The starting point of the program. The function reads and
 * processes input provided by the user. The function first reads
 * the command from user input. If command is valid, the function
 * then performs specific tasks and make specific calls based on
 * the given unique command. If the command is invalid, or if the
 * remaining input is invalid for certain commands, then the function
 * reports that the provided command is invalid, and the user is
 * prompted again to provide a valid input. Returns 0 if quit is read
 * from user input.
 */
int main( )
{
    PointList *ptlist = createPointList( );
    int num = 1;
    char command[ 7 ];
    printf( "%d> ", num );
    while ( scanf( "%6s", command ) != EOF ) {
        if ( strcmp( command, "add" ) == 0 ) {
            //call addPoint here
            executeAddCommand( ptlist );
        } else if ( strcmp ( command, "remove" ) == 0 ) {
            //call removePoint here
            executeRemoveCommand( ptlist );
        } else if ( strcmp( command, "move" ) == 0 ) {
            //Change user's current location here
            executeMoveCommand( ptlist );
        } else if ( strcmp( command, "list" ) == 0 ) {
            //call listPoints here, passing a list test function
            executeListCommand( ptlist );
        } else if ( strcmp( command, "nearby" ) == 0 ) {
            //call listPoints here, passing a nearby test function
            executeNearbyCommand( ptlist );
        } else if ( strcmp( command, "match" ) == 0 ) {
            //call listPoints here, passing a match test function
            executeMatchCommand( ptlist );
        } else if ( strcmp( command, "help" ) == 0 ) {
            //report valid commands
            executeHelpCommand( );
        } else if ( strcmp( command, "quit" ) == 0 ) {
            //free point list, then close successfully
            executeQuitCommand( ptlist );
        } else {
            //Invalid command
            printInvalidCommandMessage( );
        }
        num++;
        printf( "%d> ", num );
        //Reset command array
        for ( int i = 0; command[ i ]; i++ ) {
            command[ i ] = '\0';
        }
    }
    freePointList( ptlist );
    printf( "\n" );
    return EXIT_SUCCESS;
}