/**
 * Program that determines if a pixel is inside a triangle of a
 * set size based on the given vertices and the x- and y- coordinates
 * of the specific location in a pixel.
 *
 * @file geometry.c
 * @author Jimmy Nguyen (jnguyen6)
 */
 
#include "geometry.h"
#include <stdlib.h>
#include <stdio.h>
 
 /**
  * Determines if the pixel is inside the triangle through the left
  * turn test, which involves the use of the cross product. First,
  * the function finds the vector between two consecutive vertices
  * and another vector between the first vertex and the x- and y-
  * coordinates of the specific location in a pixel. If the sign of
  * the cross product of the two vectors is negative, then this 
  * indicates that the pixel is in the triangle, so the function 
  * returns true. Otherwise, the function returns false.
  *
  * @param xa the x-coordinate of the first vertex of the triangle
  * @param ya the y-coordinate of the first vertex of the triangle
  * @param xb the x-coordinate of the second vertex of the triangle
  * @param yb the y-coordinate of the second vertex of the triangle
  * @param x the x-coordinate of the specific location of the pixel
  * @param y the y-coordinate of the specific location of the pixel
  * @return true if the pixel is in the triangle or false otherwise
  */
bool leftOf( double xa, double ya, double xb, double yb, double x, double y )
{
    //Let's use xa and ya as the starting point to find the components
    //for the vector.
    double vectAX, vectAY, vectBX, vectBY;
    if ( xa < 0 ) {
        xa = -xa;
        vectAX = xb + xa;
        vectBX = x + xa;
    } else {
        vectAX = xb - xa;
        vectBX = x - xa;
    }
    if ( ya < 0 ) {
        ya = -ya;
        vectAY = yb + ya;
        vectBY = y + ya;
    } else {
        vectAY = yb - ya;
        vectBY = y - ya;
    }
    
    if ( vectAY * vectBX < 0 ) {
        vectAY = -vectAY; //For this case, just make one vector component negative
        return vectAX * vectBY + vectAY * vectBX <= 0;
    } else {
        return vectAX * vectBY - vectAY * vectBX <= 0;
    }
}

/**
 * Determines whether the pixel is inside the triangle or not. Returns
 * true if the pixel is inside the triangle or false otherwise.
 *
 * @param x1 the x-coordinate of the first vertex of the triangle
 * @param y1 the y-coordinate of the first vertex of the triangle
 * @param x2 the x-coordinate of the second vertex of the triangle
 * @param y2 the y-coordinate of the second vertex of the triangle
 * @param x3 the x-coordinate of the third vertex of the triangle
 * @param y3 the y-coordinate of the third vertex of the triangle
 * @param x the x-coordinate of the specific location of the pixel
 * @param y the y-coordinate of the specific location of the pixel
 * @return true if the pixel is inside the triangle or false otherwise
 */
bool inside( double x1, double y1, double x2, double y2, double x3, double y3,
             double x, double y )
{
    if ( !leftOf(x1, y1, x2, y2, x, y ) ) {
        return false;
    } else if ( !leftOf(x2, y2, x3, y3, x, y ) ) {
        return false;
    } else if ( !leftOf(x3, y3, x1, y1, x, y ) ) {
        return false;
    } else {
        return true;
    }
}


