/**
 * Program that determines if a pixel is inside a triangle of a
 * set size based on the given vertices and the location of the
 * center of the pixel.
 *
 * @file geometry.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include <stdbool.h>

/** For the extra credit, the number of horizontal and vertical super-samples
    per pixel.  This is a preprocessor macro with conditional compilation
    code around it.  That lets us replace this value on the gcc line, if
    we want to.  This constant definition should probably be in triangle.c
    rather than in this header, but putting it here lets me give it to you
    with the starter. */
#ifndef SSAMP
#define SSAMP 1
#endif

 /**
  * Determines if the pixel is inside the triangle through the left
  * turn test, which involves the use of the cross product. First,
  * the function finds the vector between two consecutive vertices
  * and another vector between the first vertex and the center of
  * the pixel. If the sign of the cross product of the two vectors
  * is negative, then this indicates that the pixel is in the triangle,
  * so the function returns true. Otherwise, the function returns
  * false.
  *
  * @param xa the x-coordinate of the first vertex of the triangle
  * @param ya the y-coordinate of the first vertex of the triangle
  * @param xb the x-coordinate of the second vertex of the triangle
  * @param yb the y-coordinate of the second vertex of the triangle
  * @param x the x-coordinate of the center of the pixel
  * @param y the y-coordinate of the center of the pixel
  * @return true if the pixel is in the triangle or false otherwise
  */
bool leftOf( double xa, double ya, double xb, double yb,
             double x, double y );

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
 * @param x the x-coordinate of the center of the pixel
 * @param y the y-coordinate of the center of the pixel
 * @return true if the pixel is inside the triangle or false otherwise
 */
bool inside( double x1, double y1, double x2, double y2,
             double x3, double y3, double x, double y );
