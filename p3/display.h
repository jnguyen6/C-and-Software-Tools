/**
 * The header file that contains prototypes for printing characters
 * in a word to standard output and printing the stick figure to
 * standard output based on the given number of body parts.
 *
 * @file display.h
 * @author Jimmy Nguyen (jnguyen6)
 */

/**
 * Prints the characters in the given word array to standard
 * output. Some or all of the characters in the word array
 * can be underscores. A whitespace is used to separate each
 * printed character.
 *
 * @param word the word array to print to standard output
 */
void displayWord( char word[ ] );

/**
 * Prints the stick figure to standard output based on the given
 * number of body parts. If the given number of body parts is 0,
 * then only a new line character is printed.
 *
 * @param numberOfParts the number of parts to print
 */
void displayFigure( int numberOfParts );
