/**
 * The Triangle program reads several integer and floating-point values from
 * input and creates an output ppm file based on the input values. The program first
 * expects two integers, which represents the height and width of the triangle.
 * The program then expects six floating-point values, in which each pair of values
 * are used to define the location of each vertex for the triangle. Finally, the
 * program expects three integers, which represents the RGB values used to color
 * the triangle.
 *
 * @file triangle.c
 * @author Jimmy Nguyen (jnguyen6)
 */ 
 
 #include "encoding.h"
 #include "geometry.h"
 #include <stdio.h>
 #include <stdbool.h>
 #include <stdlib.h>
 
 /** Value used to help find the center point of the pixel. */
 #define HALF_STEP 0.5
 
 /**
  * The starting point of the program. The function reads input from the user
  * and generates the output ppm file. If input values are out of range or are
  * not read as integers or floating-point values, then the program exits with
  * a failure status (1). Otherwise, the program exits with a success status (0).
  *
  * @return the program's exit status
  */
int main( )
{
    int width, height;
    double x1, y1, x2, y2, x3, y3;
    int red, green, blue;
    int matches;
    matches = scanf( "%d %d", &width, &height );
    if ( matches != 2 || width < 0 || height < 0) {
        return EXIT_FAILURE;
    }
    matches = scanf( "%lf %lf %lf %lf %lf %lf", &x1, &y1, &x2, &y2, &x3, &y3 );
    if ( matches != 6 ) {
        return EXIT_FAILURE;
    }
    matches = scanf("%d %d %d", &red, &green, &blue);
    if ( matches != 3 || red < 0 || red > CMAX || green < 0 || green > CMAX
          || blue < 0 || blue > CMAX) {
        return EXIT_FAILURE;
    }
    printHeader( width, height );
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ( inside( x1, y1, x2, y2, x3, y3, j + HALF_STEP, i + HALF_STEP )) {
                printValue(red);
                printValue(green);
                printValue(blue);
            } else {
                printValue(0);
                printValue(0);
                printValue(0);
            }
        }
    }
    printf("\n");
    return EXIT_SUCCESS;
    
}