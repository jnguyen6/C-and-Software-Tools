/**
 * Program that writes the output image to an output ppm file in
 * binary format. The program first prints the header, which consists
 * of the image format (P6), the width and height of the image, and
 * the maximum intensity for the RGB color. After printing the header,
 * the program then prints the color values for all pixels in the image.
 *
 * @file text.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "encoding.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** The binary image format number. */
#define IMAGE_FORMAT_NUMBER 6

/**
 * Prints a header for the binary ppm file. The header consists of the
 * image format, the size of the image, and the maximum intensity for
 * RGB color components of the image.
 *
 * @param width the width of the image
 * @param height the height of the image
 */
void printHeader( int width, int height )
{
    printf("P%d\n", IMAGE_FORMAT_NUMBER);
    printf("%d %d\n", width, height);
    printf("%d\n", CMAX);
}

/**
 * Prints the given RGB color value on an output line in binary. 
 *
 * @param c the RGB color value to print
 */
void printValue( unsigned char c )
{
    putchar( c );
}