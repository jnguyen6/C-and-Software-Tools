/**
 * Program that creates a new point of interest, frees the
 * given point of interest from a dynamically allocated memory,
 * prints the description of the given point of interest, and
 * determines and prints the distance between the given point
 * of interest and the user's current location.
 *
 * @file point.c
 * @author Jimmy Nguyen (jnguyen6)
 */
 
#include "point.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

/** The maximum length of the text description of the point of interest. */
#define MAX_DESC_LENGTH 1024
/** Multiplier for converting degrees to radians */
#define DEG_TO_RAD ( M_PI / 180 )
/** Radius of the earth in miles. */
#define EARTH_RADIUS 3959.0
/** The minimum latitude value. */
#define MIN_LAT_VAL -90
/** The maximum latitude value. */
#define MAX_LAT_VAL 90
/** The minimum longitude value. */
#define MIN_LON_VAL -180
/** The maximum longitude value. */
#define MAX_LON_VAL 180
/** The number of valid input arguments that can be scanned. */
#define MAX_NUM_VALID_ARGUMENTS 4
/** 
 * The maximum number of characters that can be printed for
 * a description on a single line.
 */
#define MAX_CHAR 75

Point *parsePoint( )
{
    Point *pt = (Point *)malloc( sizeof( *pt ) );
    pt->desc = NULL;
    double lat;
    double lon;
    char description[ MAX_DESC_LENGTH + 1 ];
    if ( scanf( "%20s %lf %lf %1024[^\n\t]", pt->name, &lat, &lon, description )
                != MAX_NUM_VALID_ARGUMENTS ) {
        freePoint( pt );
        return NULL;
    }
    description[ MAX_DESC_LENGTH ] = '\0';
    if ( lat < MIN_LAT_VAL || lat > MAX_LAT_VAL ) {
        freePoint( pt );
        return NULL;
    }
    if ( lon < MIN_LON_VAL || lon > MAX_LON_VAL ) {
        freePoint( pt );
        return NULL;
    }
    char temp[ MAX_DESC_LENGTH + 1 ];
    int matches = scanf( "%[^\n\t]", temp );
    if ( matches == 1 ) {
        freePoint( pt );
        return NULL;
    } else {
        pt->location.lat = lat;
        pt->location.lon = lon;
        char *desc = (char *)malloc( strlen( description ) + 1 * sizeof( char ) );
        for ( int i = 0; description[ i ]; i++ ) {
            desc[ i ] = description[ i ];
        }
        desc[ strlen( description ) ] = '\0';
        pt->desc = desc;
        
        for ( int i = 0; description[ i ]; i++ ) {
            description[ i ] = '\0';
        }
        return pt;
    }
}

void freePoint( Point *pt )
{
    if ( pt->desc != NULL ) {
        free( pt->desc );
    }
    free( pt );
}

void reportPoint( Point const *pt, Coords const *ref )
{
    double distance = globalDistance( &pt->location, ref );
    printf( "\n%s (%.1f miles)\n", pt->name, distance );
    //  printf( "  %s", pt->desc );
    
    //Extra credit
    char currentWord[ MAX_DESC_LENGTH + 1 ] = "";
    char nextWord[ MAX_DESC_LENGTH + 1 ] = "";
    for ( int i = 0; currentWord[ i ]; i++ ) {
        currentWord[ i ] = '\0';
    }
    for ( int i = 0; nextWord[ i ]; i++ ) {
        nextWord[ i ] = '\0';
    }
    int currentNum = 0;
    int nextWordNum = 0;
    int currentWordIndex = 0;
    bool firstWordPrinted = false;
    bool wordFound = false;
    for ( int i = 0; pt->desc[ i ]; i++ ) {
        if ( !wordFound ) {
            if ( pt->desc[ i ] != ' ' ) {
                currentWord[ currentWordIndex ] = pt->desc[ i ];
                currentNum++;
                currentWordIndex++;
            } else {
                wordFound = true;
                //Preserve whitespace
                while ( pt->desc[ i ] == ' ' ) {
                    currentWord[ currentWordIndex ] = pt->desc[ i ];
                    currentNum++;
                    i++;
                    currentWordIndex++;
                }
                currentWord[ currentWordIndex ] = '\0';
                //Make sure we decrement i here so that we
                //can read the next non-whitespace character
                //again.
                i--;
            }
        } else {
            if ( currentNum < MAX_CHAR ) {
                int currentIndex = i;
                int nextWordIndex = 0;
                while ( pt->desc[ currentIndex ] != ' ' && pt->desc[ currentIndex ] != '\0' ) {
                    nextWord[ nextWordIndex ] = pt->desc[ currentIndex ];
                    currentIndex++;
                    nextWordIndex++;
                    nextWordNum++;
                }
                nextWord[ currentIndex ] = '\0';
                if ( currentNum + nextWordNum > MAX_CHAR ) {
                    if ( !firstWordPrinted ) {
                        printf( "  " );
                    }
                    for ( int j = 0; currentWord[ j ] != '\0' && currentWord[ j ] != ' '; j++ ) {
                        printf( "%c", currentWord[ j ] );
                    }
                    printf( "\n" );
                    firstWordPrinted = false;
                    currentNum = 0;
                } else {
                    if ( !firstWordPrinted ) {
                        printf( "  " );
                        firstWordPrinted = true;
                    }
                    printf( "%s", currentWord );
                }
            } else {
                //If we get to this point, that means the current word is
                //already too long, but we print it in one line anyway.
                if ( !firstWordPrinted ) {
                    printf( "  " );
                }
                for ( int j = 0; currentWord[ j ] != '\0' && currentWord[ j ] != ' '; j++ ) {
                    printf( "%c", currentWord[ j ] );
                }
                printf( "\n" );
                currentNum = 0;
                firstWordPrinted = false;
            }
            wordFound = false;
            nextWordNum = 0;
            //Reset currentWord and nextWord array
            for ( int j = 0; currentWord[ j ]; j++ ) {
                currentWord[ j ] = '\0';
            }
            for ( int j = 0; nextWord[ j ]; j++ ) {
                nextWord[ j ] = '\0';
            }
            currentWordIndex = 0;
            i--;
        }
    }
    if ( !firstWordPrinted ) {
        printf( "  " );
    }
    printf( "%s", currentWord );
}

double globalDistance( Coords const *c1, Coords const *c2 )
{
    double v1[] = { cos( c1->lon * DEG_TO_RAD ) * cos( c1->lat * DEG_TO_RAD ),
                    sin( c1->lon * DEG_TO_RAD ) * cos( c1->lat * DEG_TO_RAD ),
                    sin( c1->lat * DEG_TO_RAD ) };
    double v2[] = { cos( c2->lon * DEG_TO_RAD ) * cos( c2->lat * DEG_TO_RAD ),
                    sin( c2->lon * DEG_TO_RAD ) * cos( c2->lat * DEG_TO_RAD ),
                    sin( c2->lat * DEG_TO_RAD ) };

    //Find dot product of v1 and v2
    double dp = 0.0;
    for ( int i = 0; i < sizeof( v1 ) / sizeof( v1[ 0 ] ); i++ ) {
        dp += v1[ i ] * v2[ i ];
    }
    
    //Find angle between two vectors based on the dot product found
    double angle = acos( dp );
    
    // Return distance based on the radius of the earth.
    return EARTH_RADIUS * angle;
}
