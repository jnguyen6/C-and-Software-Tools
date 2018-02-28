/**
 * Program that reads the words from the given input file and
 * stores them in the two-dimensional character array of words.
 *
 * @file wordlist.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "wordlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

void readWords( char const *filename )
{
    FILE *fp = fopen( filename, "r" );
    if ( !fp ) {
        fprintf( stderr, "Can't open word file\n" );
        exit( EXIT_FAILURE );
    }
    
    wordCount = 0;
    char word[ MAX_CHAR_LENGTH ];
    int matches = fscanf( fp, "%21[a-z]", word );
    while ( matches != EOF ) {
        //If the number of words in the file exceeds the word limit
        if ( wordCount >= MAX_WORDS ) {
            fprintf( stderr, "Invalid word file\n" );
            fclose( fp );
            exit( EXIT_FAILURE );
        }
        //If the characters in a word are not all lowercase letters
        if ( matches == 0 ) {
            fprintf( stderr, "Invalid word file\n" );
            fclose( fp );
            exit( EXIT_FAILURE );
        }
        //If the word is too long
        if ( strlen( word ) > MAX_CHAR_LENGTH - 1 ) {
            fprintf( stderr, "Invalid word file\n" );
            fclose( fp );
            exit( EXIT_FAILURE );
        }
        //Now, add the word to the word list
        for ( int i = 0; word[ i ]; i++ ) {
            words[ wordCount ][ i ] = word[ i ];
        }
        wordCount++;
        //Skip any whitespace until the next word is reached.
        //Then, scan the next word
        matches = fscanf( fp, " %[a-z]", word );
    }
    fclose( fp );
}
