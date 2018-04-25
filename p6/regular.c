/**
 * Component program that performs the following operations:
 * handle command-line arguments, parse the pattern given
 * as a command-line argument and create a regular expression
 * out of that pattern, read input lines and pinpoint where
 * the pattern matches the input line, and print out lines
 * where the match occurred, in which the matches are highlighted
 * in red.
 *
 * @file regular.c
 * @author Jimmy Nguyen (jnguyen6)
 * @author David Sturgill (dbsturgi)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern.h"
#include "parse.h"

/** On the command line, which argument is the pattern. */
#define PAT_ARG 1

/** On the command line, which argument is the input file. */
#define FILE_ARG 2

/** The minimum number of valid arguments. */
#define MIN_NUM_ARGS 2

/** The maximum number of valid arguments. */
#define MAX_NUM_ARGS 3

/**
 * The maximum number of characters that can be read from input,
 * including the new line character.
 */
#define MAX_INPUT_LEN 101

/** The ASCII value that represents an escape sequence. */
#define ASCII_ESC 27

/**
   Entry point for the program, parses command-line arguments, builds
   the pattern and then tests it against lines of input.

   @param argc Number of command-line arguments.
   @param argv List of command-line arguments.
   @return exit status for the program.
*/
int main( int argc, char *argv[] )
{

  if ( argc < MIN_NUM_ARGS || argc > MAX_NUM_ARGS ) {
    fprintf( stderr, "usage: regular <pattern> [input-file.txt]\n" );
    return EXIT_FAILURE;
  }
  
  FILE *infile = NULL;
  if ( argc == MAX_NUM_ARGS ) {
    infile = fopen( argv[ FILE_ARG ], "r" );
    if ( !infile ) {
      fprintf( stderr, "Can't open input file: %s\n", argv[ FILE_ARG ] );
      return EXIT_FAILURE;
    }
  } else {
    infile = stdin;
  }
  
  Pattern *pat = parsePattern( argv[ PAT_ARG ] );
  char input[ MAX_INPUT_LEN + 1 ] = "";
//   while ( fscanf( infile, "%100[^\n]", input ) != EOF ) {
//     if ( fscanf( infile, "%[^\n]", input ) == 1 ) {
//       fprintf( stderr, "Input line too long\n" );
//       return EXIT_FAILURE;
//     }
  while ( fgets( input, MAX_INPUT_LEN, infile ) ) {
    if ( input[ strlen( input ) - 1 ] != '\n' ) {
      fprintf( stderr, "Input line too long\n" );
      return EXIT_FAILURE;
    }
    
    input[ strlen( input ) - 1 ] = '\0';
    
    pat->locate( pat, input );
    
    //First, check if we have found a match or not
    bool foundMatch = false;
    for ( int i = 0; i <= pat->len; i++ ) {
      for ( int j = i; j <= pat->len; j++ ) {
        if ( matches( pat, i, j ) ) {
          foundMatch = true;
        }
      }
    }
    
    if ( foundMatch ) {
      bool redTextOn = false;
      foundMatch = false;
      for ( int i = 0; i <= pat->len; i++ ) {
        for ( int j = i; j <= pat->len; j++ ) {
          if ( matches( pat, i, j ) ) {
          //Special case: if we have found a match, but that match is a new line
          //character (empty line). If there happens to be an empty line, do not
          //"highlight" that line.
            if ( pat->len == 0 ) {
              break;
            } else {
              if ( !redTextOn ) {
                printf( "%c%c%c%c%c", ASCII_ESC, '[', '3', '1', 'm' );
                redTextOn = true;
              }
              foundMatch = true;
              if ( j == pat->len ) {
                int begin;
                for ( begin = i; begin < j ; begin++ ) {
                  printf( "%c", input[ begin ] );
                }
                i = begin;
                j = i;
                printf( "%c%c%c%c", ASCII_ESC, '[', '0', 'm' );
                redTextOn = false;
              }
            }
          } else {
            if ( foundMatch ) {
              int begin;
              for ( begin = i; begin < j - 1; begin++ ) {
                printf( "%c", input[ begin ] );
              }
              i = begin;
              j = i;
              foundMatch = false;
            }
            if ( redTextOn ) {
                printf( "%c%c%c%c", ASCII_ESC, '[', '0', 'm' );
                redTextOn = false;
            }
          }
        }
        if ( !foundMatch && i < pat->len ) {
          printf( "%c", input[ i ] );
        }
      }
      printf( "\n" );
    }
    //fscanf( infile, "%*c" );
    for ( int i = 0; input[ i ]; i++ ) {
      input[ i ] = '\0';
    }
  }

  pat->destroy( pat );
  if ( infile != stdin ) {
    fclose( infile );
  }
  return EXIT_SUCCESS;
}
