/**
 * The header file that provides prototypes for other programs to
 * use for writing out the image in either text or binary format.
 *
 * @file encoding.h
 * @author Jimmy Nguyen (jnguyen6)
 * @author David Sturgill (dbsturgi)
 */

/** The maximum RGB color component value. */
#define CMAX 255

/**
 * Prints a header for the output ppm file. The header consists of the
 * image format, the size of the image, and the maximum intensity for
 * RGB color components of the image.
 *
 * @param width the width of the image
 * @param height the height of the image
 */
void printHeader( int width, int height );

/**
 * Prints the given RGB color value on an output line. If the number of 
 * digits in the given color value exceeds the maximum line length of 70,
 * then the color value is printed on the next output line.
 *
 * @param c the RGB color value to print
 */
void printValue( unsigned char c );
