/**
 * The header file that contains prototypes for reading words from an
 * input file and storing the words into the words array, which is a
 * two-dimensional character array.
 *
 * @file wordlist.h
 * @author Jimmy Nguyen (jnguyen6)
 */

/** The maximum number of words that can be in the words array. */
#define MAX_WORDS 50
/** The maximum character length of a word, including the null terminator. */
#define MAX_CHAR_LENGTH 21

/**
 * Global variable that stores the list of words. The array can store
 * up to 50 words, with each word having a maximum character length
 * of 20.
 */
char words[ MAX_WORDS ][ MAX_CHAR_LENGTH ];

/** Global variable that keeps track of the number of words in the array. */
int wordCount;

/**
 * Reads the words from the given file and stores them in the
 * two-dimensional character array. If the given input file
 * cannot be opened, then a standard error message is printed,
 * and the program exits with a status of 1. If the given input
 * file contains words that are over the 50 word limit, does not
 * contain all lowercase letters, or are too long, then a standard
 * error message is printed, and the program exits with a status
 * of 1.
 *
 * @param filename the file to read the words from
 */
void readWords( char const *filename );
