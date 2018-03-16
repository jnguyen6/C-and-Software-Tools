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

Point *parsePoint( )
{
    char name[ MAX_NAME_LENGTH + 1 ];
    double lat;
    double lon;
    char description[ MAX_DESC_LENGTH + 1 ];
    if ( scanf( "%20s %lf %lf %1024[^\n\t]", name, &lat, &lon, description ) != 4 ) {
        return NULL;
    }
    if ( lat < MIN_LAT_VAL || lat > MAX_LAT_VAL ) {
        return NULL;
    if ( lon < MIN_LON_VAL || lon > MAX_LON_VAL ) {
        return NULL;
    }
    
    int firstInput = getchar( );
    int secondInput = getchar( );
    if ( firstInput != '\n' && firstInput != '\t' ) {
        return NULL;
    } else if ( firstInput == '\n' && secondInput != EOF ) {
        return NULL;
    } else if ( firstInput == '\t' && secondInput != EOF ) {
        return NULL;
    } else {
        Point *pt = (Point *)malloc( sizeof( Point ) );
        pt->name = name;
        pt->location->lat = lat;
        pt->location->lon = lon;
        pt->desc = description;
        return pt;
    }
}

void freePoint( Point *pt )
{
    free( pt->desc );
    free( pt );
}

void reportPoint( Point const *pt, Coords const *ref )
{
    double distance = globalDistance( &pt->location, &ref );
    printf( "%s %f\n", pt->name, distance );
    printf( "  %s\n", pt->desc );
}

double globalDistance( Coords const *c1, Coords const *c2 )
{
    //Convert coordinates into vectors
    double v1[] = { cos( c1->location->lon * DEG_TO_RAD ) 
                    * cos( c1->location->lat * DEG_TO_RAD ),
                    sin( c1->location->lon * DEG_TO_RAD )
                    * cos( c1->location->lat * DEG_TO_RAD ),
                    sin( c1->location->lat * DEG_TO_RAD ) };
    double v2[] = { cos( c2->location->lon * DEG_TO_RAD )
                    * cost( c2->location->lat * DEG_TO_RAD ),
                    sin( c2->location->lon * DEG_TO_RAD )
                    * cos( c2->location->lat * DEG_TO_RAD ),
                    sin( c2->location->lat * DEG_TO_RAD ) };

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