/**
 * Program that prints the selected word and the stick figure to
 * standard output.
 *
 * @file display.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "display.h"
#include <stdio.h>

/** The head of the stick figure. */
#define HEAD 1
/** The head and torso top of the stick figure. */
#define HEAD_TOP 2
/** The head and torso top and bottom of the stick figure. */
#define HEAD_TOP_BOTTOM 3
/** The head, torsos, and left arm of the stick figure. */
#define HEAD_TOP_BOTTOM_LEFTA 4
/** The head, torsos, and arms of the stick figure. */
#define HEAD_TOP_BOTTOM_LEFTA_RIGHTA 5
/** The head, torsos, arms, and left leg of the stick figure. */
#define HEAD_TOP_BOTTOM_LEFTA_RIGHTA_LEFTL 6
/** The stick figure will all seven body parts. */
#define FULL_FIGURE 7

/**
 * Prints the characters in the given word array to standard output.
 * Some or all of the characters in the word array can be underscores.
 * A whitespace is used to separate each printed character.
 *
 * @param word the word array to print to standard output
 */
void displayWord( char word[ ] )
{
    printf( "%c", word[ 0 ] );
    for ( int i = 1; word[ i ]; i++ ) {
        printf( " %c", word[ i ] );
    }
}

/**
 * Prints the stick figure to standard output based on the given number of
 * body parts.
 *
 * @param numberOfParts the number of parts to print
 */
void displayFigure( int numberOfParts )
{
    if ( numberOfParts == HEAD ) {
        printf( "\n O \n\n" );
    } else if ( numberOfParts == HEAD_TOP ) {
        printf( "\n O \n | \n\n" );
    } else if ( numberOfParts == HEAD_TOP_BOTTOM ) {
        printf( "\n O \n | \n | \n\n" );
    } else if ( numberOfParts == HEAD_TOP_BOTTOM_LEFTA ) {
        printf( "\n O \n/| \n | \n\n" );
    } else if ( numberOfParts == HEAD_TOP_BOTTOM_LEFTA_RIGHTA ) {
        printf( "\n O \n/|\\\n | \n\n" );
    } else if ( numberOfParts == HEAD_TOP_BOTTOM_LEFTA_RIGHTA_LEFTL ) {
        printf( "\n O \n/|\\\n | \n/\n\n" );
    } else if ( numberOfParts == FULL_FIGURE ) {
        printf( "\n O \n/|\\\n | \n/ \\\n\n" );
    } else {
        printf( "\n" );
    }   
}