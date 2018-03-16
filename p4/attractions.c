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

#include "pointlist.c"
#include "point.c"
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
void printInvalidCommandMessage( char command[ ] ) {
    scanf( "*[^n]*c", command );
    printf( "Invalid command.\n" );
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
bool list( Point const *pt, void *data )
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
bool nearby( Point const *pt, double *data )
{
    return globalDistance( &pt->location, &currentLocation ) <= data;
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
bool match( Point const *pt, char *data )
{
    int count = 0;
    char wordFromDesc[ MAX_WORD_LENGTH + 1 ];
    for ( int i = 0; pt->desc[ i ]; i++ ) {
        //If the word is longer than the expected max length of the
        //word to match, then the word is invalid So, reset the
        // wordFromDesc array, and keep on iterating through the point
        // description until a space or a comma has been reached.
        if ( count >= MAX_WORD_LENGTH && 
              ( pt->desc[ i ] != ' ' && pt->desc[ i ] != ',' ) ) {
            wordFromDesc[ count ] = '\0';
            for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                wordFromDesc[ j ] = '\0';
            }
            while ( pt->desc[ i ] != ' ' && pt->desc[ i ] != ',' ) {
                i++;
            }
            count = 0;
            
        //If we reached a comma, and the word parsed is a valid length,
        //then start comparison
        } else if ( pt->desc[ i ] == ' ' || pt->desc[ i ] == ',' ) {
            wordFromDesc[ count ] = '\0';
            bool matchFound = true;
            
            //First, do comparison. If we have found even one mismatch,
            //then the word is not a valid match.
            for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                if ( wordFromDesc[ j ] != data[ j ] 
                      || wordFromDesc[ j ] + TO_LOWERCASE != data[ j ] ) {
                    matchFound = false;
                }
            }
            for ( int j = 0; wordFromDesc[ j ]; j++ ) {
                wordFromDesc[ j ] = '\0';
            }
            count = 0;
            
            if ( matchFound ) {
                return true;
            }
        } else {
            wordFromDesc[ count ] = pt->desc[ i ];
            count++;
        }
    }
    return false;
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
    int num = 0;
    char command[ 7 ];
    printf( "%d> ", num );
    while ( scanf( "6%s", command ) != EOF ) {
        if ( strcmp( command, "add" ) == 0 ) {
            //call addPoint here
            Point *pt = parsePoint( );
            if ( pt == NULL ) {
                printInvalidCommandMessage( command );
            }
            if ( !addPoint( ptlist, pt ) ) {
                printInvalidCommandMessage( command );
            }
        } else if ( strcmp ( command, "remove" ) == 0 ) {
            //call removePoint here
            char name[ MAX_NAME_LENGTH + 1 ];
            int matches = scanf( "20%s", name );
            if ( matches == EOF ) {
                freePointList( ptlist );
                return EXIT_SUCCESS;
            } else if ( matches != 1 || getchar( ) != '\n' ) {
                printInvalidCommandMessage( command );
            } else {
                if ( !removePoint( ptlist, name ) ) {
                    printInvalidCommandMessage( command );
                }
            }
        } else if ( strcmp( command, "move" ) == 0 ) {
            //Change user's current location here
            double lat;
            double lon;
            int matches = scanf( "%lf %lf", &lat, &lon );
            if ( matches == EOF ) {
                freePointList( ptlist );
                return EXIT_SUCCESS;
            } else if ( matches != 2 ) {
                printInvalidCommandMessage( command );
            } else {
                currentLocation->lat = lat;
                currentLocation->lon = lon;
            }
        } else if ( strcmp( command, "list" ) == 0 ) {
            //call listPoints here, passing a list test function
            if ( getchar( ) != '\n' || getchar( ) != EOF ) {
                printInvalidCommandMessage( command );
            }
            listPoints( ptlist, &currentLocation, list, NULL );
        } else if ( strcmp( command, "nearby" ) == 0 ) {
            //call listPoints here, passing a nearby test function
            double distance;
            if ( scanf( " %lf", &distance ) != 1 ) {
                printInvalidCommandMessage( command );
            }
            if ( getchar( ) != '\n' || getchar( ) != EOF ) {
                printInvalidCommandMessage( command );
            } 
            listPoints( ptlist, &currentLocation, nearby, distance );
        } else if ( strcmp( command, "match" ) == 0 ) {
            //call listPoints here, passing a match test function
            char word[ MAX_WORD_LENGTH + 1 ];
            if ( scanf( "%20[a-z]", word ) != 1 ) {
                printInvalidCommandMessage( command );
            }
            if ( getchar( ) != '\n' || getchar( ) != EOF ) {
                printInvalidCommandMessage( command );
            }
            listPoints( ptlist, &currentLocation, match, word ); 
        } else if ( strcmp( command, "help" ) == 0 ) {
            //report valid commands
            printf( "add <name> <latitude> <longitude> <description>\n" );
            printf( "remove <name>\n" );
            printf( "move <latitude> <longitude>\n" );
            printf( "list\n" );
            printf( "nearby <distance>\n" );
            printf( "match <word>\n" );
            printf( "help\n" );
            printf( "quit\n" );
        } else if ( strcmp( command, "quit" ) == 0 ) {
            //free point list, then close successfully
            freePointList( ptlist );
            return EXIT_SUCCESS;
        } else {
            //Invalid command
            printInvalidCommandMessage( command );
        }
        num++;
        printf( "%d> ", num );
        
        //Reset command array
        for ( int i = 0; command[ i ]; i++ ) {
            command[ i ] = '\0';
        }
    }
}