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
    FILE *fp = fopen( filename, "r" );
    if ( !fp ) {
        fprintf( stderr, "Can't open word file" );
        exit( 1 );
    }
    
    wordCount = 0;
    char word[ MAX_WORDS ];
    while ( fscanf( fp, "%s", word ) != EOF ) {
        //If the number of words in the file exceeds the word limit
        if ( wordCount >= MAX_WORDS ) {
            fprintf( stderr, "Invalid word file" );
            fclose( fp );
            exit( 1 );
        }
        //If the word is too long
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
    } 
    fclose( fp );
}