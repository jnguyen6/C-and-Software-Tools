/**
 * The header file that contains prototypes for creating a new
 * point of interest, freeing the given point of interest from
 * memory, printing out the description of the given point
 * of interest, and determining the distance between the given
 * point of interest and the provided coordinates.
 *
 * @file point.h
 * @author Jimmy Nguyen (jnguyen6)
 * @author David Sturgill (dbsturgi)
 */

#ifndef POINT_H
#define POINT_H

/** The maximum length of the name of the point of interest. */
#define MAX_NAME_LENGTH 20

/** Representation for a location, in latitude and longitude. */
typedef struct {
    /** Latitude value for a global location. */
    double lat;
  
    /** Longitude value for a global location. */
    double lon;
} Coords;

/**
 * A struct that represents a point of interest.
 */
typedef struct {
    /** A character array that represents the name of the point of interest. */
    char name[ MAX_NAME_LENGTH + 1 ];
    /** The location of the point of interest, in latitude and longitude. */
    Coords location;
    /** A character pointer to the character array that contains
     * the text description of the point of interest.
     */
    char *desc;
} Point

/**
 * Creates a new Point struct based on the standard input. The
 * function will expect to read a name, a latitude and longitude,
 * and a description in order to create a new Point. Returns a
 * pointer to the constructed Point struct if the Point is
 * successfully constructed. Otherwise, the function returns NULL.
 *
 * @return the pointer to the constructed Point or NULL if the
 * standard input contains invalid arguments
 */
Point *parsePoint( );

/**
 * Frees the memory of the given Point, which includes the Point
 * struct itself and the memory of the pointer to the description
 * that the Point struct has.
 *
 * @param pt the pointer to the Point struct to free the memory for
 */
void freePoint( Point *pt );

/**
 * Prints the description of the given Point struct. The first line
 * to output will consist of the name of the Point and the distance
 * from the user's current location to the Point, in miles, separated
 * by a whitespace. The next line consists of the Point's description,
 * indented by two whitespaces.
 *
 * @param pt the pointer to the Point struct
 * @param ref the pointer to the user's current location
 */
void reportPoint( Point const *pt, Coords const *ref );

/**
 * Determines and returns the distance between the given two addresses
 * of the Coords struct, in miles.
 *
 * @param c1 the pointer to the address of the first Coords struct
 * @param c2 the pointer to the address of the second Coords struct
 * @return the distance between the given two coordinates in miles
 */
double globalDistance( Coords const *c1, Coords const *c2 );
#endif
