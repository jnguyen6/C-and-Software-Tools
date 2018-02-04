/** Replace with your own file block comment.
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

/** Replace with your own function block comment.
    Remember, for functions prototyped in a header, the block
    comment goes on the prototype.
 */
bool leftOf( double xa, double ya, double xb, double yb,
             double x, double y );

/** Replace with your own function block comment.
 */
bool inside( double x1, double y1, double x2, double y2,
             double x3, double y3, double x, double y );
