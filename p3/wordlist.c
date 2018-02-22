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

/**
 * Reads the words from the given file and stores them in the two-dimensional
 * character array.
 *
 * @param filename the file to read the words from
 */
void readWords( char const *filename )
{
    FILE *fp = fopen( *filename, "r" );
    if ( !fp ) {
        fprintf( stderr, "Can't open word file" );
        exit( 1 );
    }
    
    int ch = fgetc( fp );
    while ( ch != EOF ) {
        ungetc( ch, fp );
        //If the number of words in the file exceeds the word limit
        if ( wordCount > MAX_WORDS ) {
            fprintf( stderr, "Invalid word file" );
            fclose( fp );
            exit( 1 );
        }
        char word[ ];
        //If the word is too long
        fscanf( "%s", word );
        if ( strlen( word ) > MAX_CHAR_LENGTH - 1 ) {
            fprintf( stderr, "Invalid word file" );
            fclose( fp );
            exit( 1 );
        }
        //Now, add the word to the word list
        for ( int i = 0; word[ i ]; i++ ) {
            words[ wordCount ][ i ] = word[ i ];
        }
        wordCount++;
        ch = fgetc( fp );
    }
    fclose( fp );
}