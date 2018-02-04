/**
 * Program that writes the output image to an output ppm file in
 * text format. The program first prints the header, which consists
 * of the image format (P3), the width and height of the image, and
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

/** The text image format. */
#define IMAGE_FORMAT "P3"
/** The maximum line length. */
#define MAX_LENGTH 70

/**
 * Prints a header for the text ppm file. The header consists of the
 * image format, the size of the image, and the maximum intensity for
 * RGB color components of the image.
 *
 * @param width the width of the image
 * @param height the height of the image
 */
void printHeader( int width, int height )
{
    printf("%s", IMAGE_FORMAT);
    printf("\n");
    printf("%d %d", width, height);
    printf("\n");
    printf("%d", CMAX);
    
}

/**
 * Prints the given RGB color value on an output line in text encoding. 
 * If the number of digits in the given color value exceeds the maximum
 * line length of 70, then the color value is printed on the next output
 * line.
 *
 * @param c the RGB color value to print
 */
void printValue( unsigned char c )
{
    unsigned char x = c;
    int numDigits = 0;
    while ( x != 0 ) {
        numDigits++;
        x = x / 10;
    }
    static int count = 0;
    if ( numDigits + count == 70 ) {
        count = 0;
        putchar('\n');
    }
    putchar(c);
    putchar(' ');
}