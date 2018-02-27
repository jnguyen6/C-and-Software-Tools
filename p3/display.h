/**
 * The header file that contains prototypes for printing characters
 * in a word to standard output and printing the stick figure to
 * standard output based on the given number of body parts.
 *
 * @file display.h
 * @author Jimmy Nguyen (jnguyen6)
 */

/**
 * Prototype that prints the characters in the given word array to
 * standard output. Some or all of the characters in the word array
 * can be underscores. A whitespace is used to separate each printed
 * character.
 *
 * @param word the word array to print to standard output
 */
void displayWord( char word[ ] );

/**
 * Prototype that prints the stick figure to standard output based on
 * the given number of body parts.
 *
 * @param numberOfParts the number of parts to print
 */
void displayFigure( int numberOfParts );