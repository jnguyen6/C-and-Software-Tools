/**
 * Component program that is responsible for reading characters
 * from an input file. Once the characters are successfully read,
 * the characters are converted to binary code and then printed to
 * an output file. The codes component is used to help convert
 * the characters read into binary codes, and the bits component
 * is used to help print the binary codes to an output file.
 *
 * @file encode.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "codes.h"
#include "bits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * The maximum number of valid command-line arguments for the
 * encode program.
 */
#define VALID_NUM_ARGS 4
/** The index of the code information file from the arguments array. */
#define CODE_FILE_INDX 1
/** The index of the input file. */
#define INPUT_FILE_INDX 2
/** The index of the output file. */
#define OUTPUT_FILE_INDX 3

/**
 * The starting point of the program. The main function first reads
 * the three command-line arguments, which are the file that contains
 * prefix codes, an input file, and an output file. Once the command-line
 * arguments have been read, the function then reads all the characters
 * from the input file, convert them into binary codes, and then print
 * them to the output file. If the number of command-line arguments
 * provided is invalid, then a usage message is displayed, and the
 * function exits with a status of 1. If the input file contains
 * characters other than lowercase letters, spaces, and new lines,
 * then an error message is displayed, and the function exits with
 * a status of 1. If the given code file is invalid, then an error
 * message is displayed, and the function exits with a status of 1.
 * If the given code, input, and/or output files cannot be opened,
 * then an error message is displayed, and the function exits with a
 * status of 1. Otherwise, the characters would be successfully converted
 * and printed to the output file in binary code, and the function will
 * finally exit with a status of 0.
 *
 * @param argv the number of command-line arguments provided
 * @param argc the command-line arguments
 * @return the program's exit status
 */
int main( int argc, char *argv[] )
{
    if ( argc != VALID_NUM_ARGS ) {
        fprintf( stderr, "usage: encode <codes-file> <infile> <outfile>\n" );
        return EXIT_FAILURE;
    }
    
    FILE *codeFile = fopen( argv[ CODE_FILE_INDX ], "r" );
    if ( !codeFile ) {
        perror( argv[ CODE_FILE_INDX ] );
        return EXIT_FAILURE;
    }
    FILE *input = fopen( argv[ INPUT_FILE_INDX ], "r" );
    if ( !input ) {
        perror( argv[ INPUT_FILE_INDX ] );
        fclose( codeFile );
        return EXIT_FAILURE;
    }
    FILE *output = fopen( argv[ OUTPUT_FILE_INDX ], "wb" );
    if ( !output ) {
        perror( argv[ OUTPUT_FILE_INDX ] );
        fclose( codeFile );
        fclose( input );
        return EXIT_FAILURE;
    }
    
    //Add code for reading code file here
    //First, dynamically allocate array for code struct. Then,
    //read the code file and add each code to the code array
    createCodeList();
    if ( !readCodeFile( codeFile ) ) {
        fprintf( stderr, "Invalid code file\n" );
        freeCodeList( );
        fclose( codeFile );
        fclose( input );
        fclose( output );
        return EXIT_FAILURE;
    }
    
    //Start reading characters and printing them to output file as
    //binary codes
    char ch;
    BitBuffer *buffer = (BitBuffer *) malloc( sizeof( BitBuffer ) );
    buffer->bits = 0x00;
    buffer->bcount = 0;
    //Keep getting each individual character and convert them to
    //binary code until we've reached EOF.
    while ( ( fscanf( input, "%c", &ch ) ) != EOF ) {
        const char *binaryCode = symToCode( ch - '0' );
        if ( binaryCode == NULL ) {
            fprintf( stderr, "Invalid input file\n" );
            free( buffer );
            freeCodeList( );
            fclose( codeFile );
            fclose( input );
            fclose( output );
            return EXIT_FAILURE;
        }
        writeBits( binaryCode, buffer, output );
    }
    //If we have reached this point, the character is an EOF, so
    //do one last conversion to binary code
    const char *binaryCode = symToCode( -1 );
    if ( binaryCode == NULL ) {
        fprintf( stderr, "Invalid input file\n" );
        free( buffer );
        freeCodeList( );
        fclose( codeFile );
        fclose( input );
        fclose( output );
        return EXIT_FAILURE;
    }
    writeBits( binaryCode, buffer, output );
    flushBits( buffer, output );
    free( buffer );
    freeCodeList( );
    fclose( codeFile );
    fclose( input );
    fclose( output );
    return EXIT_SUCCESS;
}
