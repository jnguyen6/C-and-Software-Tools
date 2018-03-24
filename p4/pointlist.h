/**
 * The header file that maintains an array of pointers to
 * Point structs, called PointList, and provides prototypes
 * for creating a PointList struct, freeing the memory used
 * by the given PointList, adding a Point to the given PointList,
 * removing a Point from the given PointList, and printing the
 * Points from the PointList along with the distance of each Point
 * from the user's current location with specific conditions.
 *
 * @file pointlist.h
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "point.h"
#include <stdbool.h>

/**
 * A struct that represents an array of pointers to Point structs.
 * The struct contains a pointer to the array of pointers to Point
 * instances, the number of Points currently in the array, and
 * the current capacity of the array.
 */
typedef struct {
    /** A pointer to the array of pointers to Point instances. */
    Point **list;
    /** The number of Points currently in the array. */
    int count;
    /** The current capacity of the array. */
    int cap;
} PointList;

/**
 * Dynamically allocates a PointList instance and initializes the
 * PointList fields. The PointList will be initially empty, with
 * an initial capacity of 10. Returns the newly created PointList.
 *
 * @return the instance of the PointList
 */
PointList *createPointList( );

/**
 * Frees all the dynamically allocated memory used by the PointList
 * struct. Freeing the memory includes the PointList struct itself and
 * all of the Points that are in the PointList.
 *
 * @param ptlist the pointer to the PointList struct
 */
void freePointList( PointList *ptlist );

/**
 * Adds the given Point to the given PointList. If the PointList is
 * at capacity, then the PointList's internal array is resized to
 * make room for the Point to add. Returns true if the Point is
 * successfully added to the PointList. Returns false if the given
 * Point has the same name as the other existing Point in the PointList.
 *
 * @param ptlist the pointer to the PointList struct
 * @param pt the Point to add to the PointList struct
 * @return true if the Point is successfully added to the PointList
 * or false if the Point has the same name as the other Point in the
 * PointList
 */
bool addPoint( PointList *ptlist, Point *pt );

/**
 * Removes a specific Point from the given PointList based on the
 * given name. Returns true if the Point is successfully removed
 * from the PointList or false otherwise.
 *
 * @param ptlist the pointer to the PointList struct
 * @param name the name of the Point to remove from the PointList
 * @return true if the Point is removed from the PointList or false
 * otherwise
 */
bool removePoint( PointList *ptlist, char const *name );

/**
 * Prints selected Points from the PointList based on the given
 * test function and the pointer to the data that the test
 * function uses to determine which Points to print.
 *
 * @param ptlist the pointer to the PointList struct
 * @param ref the pointer to the user's current location
 * @param test the pointer to the test function used to help
 * determine which Points to print
 * @param data the pointer to the data for the test function
 * to use
 */
void listPoints( PointList *ptlist, Coords const *ref, bool
                 (*test) ( Point const *pt, void *data ), void *data );
