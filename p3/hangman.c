/**
 * Program that simulates a hangman game. The program will read a list
 * of words and an optional seed for the random number generator to use
 * to choose a specific word for the user to guess. The program will
 * then display underscores that represents the selected word to guess.
 * The user will enter a letter that is or is not in the word. If the
 * letter is not in the word, then a stick figure is created, along with
 * additional body parts from subsequent incorrect guesses. If the letter
 * is in the word, then the word will be displayed, with the correct letter
 * replacing the underscore. After the user wins or loses the game, the
 * program will ask the user if he or she would like to play the game again.
 * If the user enters anything that begins with 'y' or 'Y', then the program
 * selects another word for the user to guess. Otherwise, the program closes
 * successfully. If the provided word list and/or seed is invalid, then the
 * program prints an error message and closes unsuccessfully.
 *
 * @file hangman.c
 * @author Jimmy Nguyen (jnguyen6)
 */
 
 #include "display.h"
 #include "wordlist.h"
 #include <stdio.h>
 #include <stdlib.h>
 #include <stddef.h>
 
 /** The number of letters in the English alphabet. */
 #define NUM_LETTERS 26
 /** The maximum number of incorrect inputs the program can have. */
 #define MAX_NUM_INCORRECT 7
 /** The ASCII code for 'a'. */
 #define ASCII_A 97
 
/**
 * The starting point for the program. The function will get the word file
 * name and the optional seed for the random number generator. If the user
 * does not provide the correct number of command line arguments, then a
 * usage message is displayed, and the program closes unsuccessfully.
 *
 * @param argc the number of command line arguments given
 * @param argv the array of null terminated strings for the command line 
 * arguments
 * @return the program's exit status
 */
int main ( int argc, char *argv[] )
{
    if ( argc = 1 ) {
        fprintf( stderr, "usage: hangman <word-file> [seed]");
        exit( 1 );
    }
    readWord( &argv[ 1 ] );
    int index;
    int seed = NULL;
    if ( argc = 3 ) {
        seed = atoi( argv[ 2 ] );
        if ( seed < 0 ) {
            fprintf( stderr, "usage: hangman <word-file> [seed]");
            exit( 1 );
        }
    }
    
    char response[ ] = "y";
    while ( response[ 0 ] == 'y' || response[ 0 ] == 'Y' ) {
        index = srand( time( seed ) ) % wordCount;
        char word[ MAX_CHAR_LENGTH ];
        //Select the word for the user to guess
        for ( int i = 0; i < MAX_CHAR_LENGTH; i++ ) {
            word[ i ] = words[ index ][ i ];
        }
        int numInvalid = 0;
        int numValid = 0;
        int target = sizeof( word[ ] / word[ 0 ] );
        char letters[ NUM_LETTERS ];
        //Create the array of letters
        for ( int i = 0; i < NUM_LETTERS; i++ ) {
            letters[ i ] = ( char ) ( i + ASCII_A );
        }
        while ( numInvalid != MAX_NUM_INCORRECT || numValid != target ) {
            displayFigure( numInvalid );
            displayWord( word );
            printf( "Remaining letters:");
            for ( int i = 0; letters[ i ]; i++ ) {
                printf(" %c", letters[ i ]);
            }
            
            printf("\nletter> ");
            char letter[2];
            char ch;
            while ( scanf( "%1[a-z]", letter ) != 1 ) {
                printf( "Invalid letter\n" )
                scanf( "%*[^\n]%c", &ch );
            }
            
        }
    }
}