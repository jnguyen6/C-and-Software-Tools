/**
 * Program that represents a list of pointers to Point instances.
 * The program provides the following operations: creating a
 * new PointList struct, adding a Point to a PointList, removing
 * a Point from the Pointlist, freeing the dynamically allocated
 * memory used by the PointList, and printing the Points from the
 * PointList with specific conditions.
 *
 * @file pointlist.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "pointlist.h"
#include "point.c"
#include <stdio.h>
#include <stdlib.h>

/** The initial capacity of the PointList. */
#define INITIAL_CAP 10
/** Constant used to increase the capacity of the PointList. */
#define RESIZE_MULTIPLIER 2

/** Holds the user's current location. */
static Coords currentLoc;

/**
 * A comparison function that compares the two pointers to
 * Points based on relative distance from the Point to the
 * user's current location. Returns -1 if the first Point is
 * closer to the user's current location than the second Point.
 * Returns 0 if the two Points have the same distance to the
 * user's current location. Returns 1 if the first Point is
 * farther away from the user's current location than the
 * second Point.
 *
 * @param pptra the pointer to the first Point
 * @param pptrb the pointer to the second Point
 * @return -1 or 1 if the first point is closer to or farther
 * away from the user's current location than the second Point,
 * or 0 if the two Points have the same distance from the user's
 * current location
 */
int pointComp( const void *pptra, const void *pptrb )
{
    Point const *a = pptra;
    Point const *b = pptrb;
    
    if ( globalDistance( &a->location, &currentLoc ) 
         < globalDistance( &b->location, &currentLoc ) ) {
        return -1;
    }
    if ( globalDistance( &a->location, &currentLoc )
         > globalDistance( &b->location, &currentLoc ) ) {
        return 1;
    }
    return 0;
}

PointList *createPointList( )
{
    PointList *ptlist = (PointList *)malloc( sizeof( PointList ) );
    ptlist->list = (Point **)malloc( INITIAL_CAP * sizeof( Point ) );
    ptlist->count = 0;
    ptlist->cap = INITIAL_CAP;
}

void freePointList( PointList *ptlist )
{
    for ( int i = 0; i < count; i++ ) {
        freePoint( ptlist->list[ i ] );
    }
    free( ptlist->list );
    free( ptlist );
}

bool addPoint( PointList *ptlist, Point *pt )
{
    //First, determine if the given point is a duplicate
    for ( int i = 0; i < count; i++ ) {
        if ( strcmp( ptlist->list[ i ]->name, pt->name ) == 0 ) {
            return false;
        }
    }
    //Then, determine if the array needs to be resized
    if ( ptlist->count >= ptlist->cap ) {
        ptlist->cap *= RESIZE_MULTIPLIER;
        ptlist->list = (Point **)realloc( ptlist->list, ptlist->cap * sizeof( Point ) );
    }
    ptlist->list[ count ] = pt;
    ptlist->count++;
    return true;
}

bool removePoint( PointList *ptlist, char const *name )
{
    //To remove a POI from the list, create new array, and update
    //the pointer to the new list
    Point **newList = (Point **)malloc( ptlist->cap * sizeof( Point ) );
    bool pointFound = false;
    for ( int i = 0; i < count; i++ ) {
        if ( strcmp( ptlist->list[ i ]->name, name ) == 0 ) {
            pointFound = true;
        } else {
            newList[ i ] = ptlist->list[ i ];
        }
    }
    for ( int i = 0; i < count; i++ ) {
        freePoint( ptlist->list[ i ] );
    }
    free( ptlist->list );
    ptlist->list = newList;
    ptlist->count--;
    return pointFound;
}

void listPoints( PointList *ptlist, Coords const *ref, bool
                 (*test) ( Point const *pt, void *data ), void *data )
{
    currentLoc = ref;
    //First, sort the points
    qsort( ptlist->list, ptlist->count, sizeof( ptlist->list[ 0 ] ), pointComp );
    for ( int i = 0; i < count; i++ ) {
        //If the point is valid, depending on certain functions, then
        //print the point. Otherwise, ignore the point
        if ( test( ptlist->list[ i ], data ) ) {
            reportPoint( &ptlist->list[ i ], &ref );
        }
    }
}

