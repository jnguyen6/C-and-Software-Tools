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
#include <stdbool.h>
#include <string.h>
#include <time.h>
 
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
    //If the user does not provide the input file
    if ( argc == 1 ) {
        fprintf( stderr, "usage: hangman <word-file> [seed]\n" );
        exit( 1 );
    }
    readWords( argv[ 1 ] );
    int seed = -1;
    //If the user provides a seed
    if ( argc == 3 ) {
        seed = atoi( argv[ 2 ] );
        if ( seed < 0 ) {
            fprintf( stderr, "usage: hangman <word-file> [seed]\n" );
            exit( 1 );
        }
    }
    if ( seed < 0 ) {
        srand( time( NULL ) );
    } else {
        srand( (unsigned) seed );
    }
    char response[ 2 ];
    response[ 0 ] = 'y';
    while ( response[ 0 ] == 'y' || response[ 0 ] == 'Y' ) {
        int index = rand( ) % wordCount;
        char selectedWord[ MAX_CHAR_LENGTH ];
        char wordToGuess[ MAX_CHAR_LENGTH ];
        for ( int i = 0; i < MAX_CHAR_LENGTH; i++ ) {
            selectedWord[ i ] = '\0';
            wordToGuess[ i ] = '\0';
        }
        
        //Select the word for the user to guess
        for ( int i = 0; words[ index ][ i ]; i++ ) {
            selectedWord[ i ] = words[ index ][ i ];
        }
        for ( int i = 0; selectedWord[ i ]; i++ ) {
            wordToGuess[ i ] = '_';
        }
        int numTries = 0;
        int numMatches = 0;
        int target = strlen( selectedWord );
        char letters[ NUM_LETTERS + 1 ];
        
        //Create the array of letters
        for ( int i = 0; i < NUM_LETTERS; i++ ) {
            letters[ i ] = ( char ) ( i + ASCII_A );
        }
        letters[ NUM_LETTERS ] = '\0';
        
        //Keep iterating until user uses up all his/her guesses or
        //if the user correctly guessed the word
        while ( numMatches != target && numTries != MAX_NUM_INCORRECT ) {
            displayFigure( numTries );
            displayWord( wordToGuess );
            printf( "\n\nRemaining letters:" );
            for ( int i = 0; letters[ i ]; i++ ) {
                printf( " %c", letters[ i ] );
            }
            
            printf( "\n\nletter> " );
            char letter[ MAX_WORDS ];
            char ch;
            bool isValid = false;
            while ( !isValid ) {
                if ( scanf( "%s", letter) == EOF ) {
                    return EXIT_SUCCESS;
                }
                //If the word is too long
                if ( strlen( letter ) > 1 ) {
                    printf( "Invalid letter\n" );
                    printf( "\nletter> " );
                    scanf( "%*[^\n]%c", &ch );
                } else {
                    for ( int i = 0; letters[ i ]; i++ ) {
                        //If the letter is not previously chosen
                        if ( letter[ 0 ] == letters[ i ] ) {
                            isValid = true;
                        }
                    }
                    //If the letter was already chosen or is not valid
                    if ( !isValid ) {
                        printf( "Invalid letter\n" );
                        printf( "\nletter> " );
                        scanf( "%*[^\n]%c", &ch );
                    }
                }
            }
            //Remove letter from list, then perform shifting
            for ( int i = 0; letters[ i ]; i++ ) {
                if ( letter[ 0 ] == letters[ i ] ) {
                    for ( int j = i; letters[ j ]; j++ ) {
                        letters[ j ] = letters[ j + 1 ];
                    }
                }
            }
            //Now check if the input letter is part of the selected word
            bool matchFound = false;
            for ( int i = 0; selectedWord[ i ]; i++ ) {
                if ( letter[ 0 ] == selectedWord[ i ] ) {
                    matchFound = true;
                    numMatches++;
                    wordToGuess[ i ] = letter[ 0 ];
                }
            }
            if ( !matchFound ) {
                numTries++;
            }
        }
        //If we get to this point, then the user is prompted to either play
        //again or quit
        if ( numMatches == target ) {
            printf( "\n" );
            displayWord( wordToGuess );
            printf( "\n\nYou win!" );
        } else {
            displayFigure( numTries );
            printf( "You lose!\nWord was " );
            for ( int i = 0; selectedWord[ i ]; i++ ) {
                printf( "%c", selectedWord[ i ] );
            }
        }
        printf( "\n\nPlay again(y,n)> " );
        scanf( "%1s%*[^\n]%*c", response );
    }
    return EXIT_SUCCESS;
}