/**
 * Component program that is responsible for reading bits from an
 * input file, converting those bits into ASCII characters or EOF,
 * and writing those converted codes to an output file. The code
 * component is used to help convert the codes read into legal
 * characters, and the bits component is used to help read the
 * bits from the input file.
 *
 * @file decode.c
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
 * decode program.
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
 * arguments have been read, the function then reads all the bits from the
 * input file, convert them into ASCII characters or EOF, and then print
 * them to the output file. If the number of command-line arguments
 * provided is invalid, then a usage message is displayed, and the
 * function exits with a status of 1. If the input file does not contain
 * the correct bit characters, then an error message is displayed, and the
 * function exits with a status of 1. If the given code file is invalid,
 * then an error message is displayed, and the function exits with a status
 * of 1. If the given code, input, and/or output files cannot be opened,
 * then an error message is displayed, and the function exits with a status
 * of 1. Otherwise, the bits would be successfully converted to ASCII
 * characters or EOF and printed to an output file, and the function will
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
    FILE *input = fopen( argv[ INPUT_FILE_INDX ], "rb" );
    if ( !input ) {
        perror( argv[ INPUT_FILE_INDX ] );
        fclose( codeFile );
        return EXIT_FAILURE;
    }
    FILE *output = fopen( argv[ OUTPUT_FILE_INDX ], "w" );
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
    int chInt = 0;
    BitBuffer *buffer = (BitBuffer *) malloc( sizeof( BitBuffer ) );
    buffer->bits = 0x00;
    buffer->bcount = 0;
    int bit;
    int index = 0;
    char codeForCmp[ MAX_NUM_BITS ] = "";
    bool matchFound = false;
    while ( ( bit = readBit( buffer, input ) ) != -1 && chInt != -1 ) {
        if ( bit == 1 ) {
            codeForCmp[ index ] = '1';
        } else {
            codeForCmp[ index ] = '0';
        }
        chInt = codeToSym( codeForCmp );
        if ( chInt != ERR_NUM && chInt != -1 ) {
            matchFound = true;
            ch = chInt;
            fprintf( output, "%c", ch );
            for ( int i = 0; codeForCmp[ i ]; i++ ) {
                codeForCmp[ i ] = '\0';
            }
            index = -1;
        }
        if ( chInt == ERR_NUM ) {
            matchFound = false;
        } else {
            matchFound = true;
        }
        index++;
    }
    //If we have reached this point, that means that either there are no
    //more bits to read or we have read bits that represents an EOF. If
    //we have not found the EOF, then the input file is invalid.
    if ( !matchFound ) {
        fprintf( stderr, "Invalid input file\n" );
        free( buffer );
        freeCodeList( );
        fclose( codeFile );
        fclose( input );
        fclose( output );
        return EXIT_FAILURE;
    }
    free( buffer );
    freeCodeList( );
    fclose( codeFile );
    fclose( input );
    fclose( output );
    return EXIT_SUCCESS;
}
