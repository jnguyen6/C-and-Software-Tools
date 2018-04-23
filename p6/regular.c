#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern.h"
#include "parse.h"

// On the command line, which argument is the pattern.
#define PAT_ARG 1

// On the command line, which argument is the input file.
#define FILE_ARG 2

/** The minimum number of valid arguments. */
#define MIN_NUM_ARGS 2

/** The maximum number of valid arguments. */
#define MAX_NUM_ARGS 3

/** The maximum number of characters that can be read from input. */
#define MAX_INPUT_LEN 100

// You won't need this function in the final version of your program.
// It prints out the input string and all matches of the pattern inside
// it.
void reportMatches( Pattern *pat, char const *pstr, char const *str )
{
  // Report the original string and copies of all the matches.
  printf( "Pattern: %s\n", pstr );
  printf( "String:  %s\n", str );

  int len = strlen( str );
  bool mflag = false;
  for ( int begin = 0; begin <= len; begin++ )
    for ( int end = begin; end <= len; end++ )
      if ( matches( pat, begin, end ) ) {
        // Report the matching substring.
        
        // Skip over to the start of the match.
        printf( "Match:   %*s", begin, "" );
        
        // Print the matchng string.
        for ( int k = begin; k < end; k++ )
          printf( "%c", str[ k ] );
        printf( "\n" );

        // Remember that we found a match.
        mflag = true;
      }

  if ( !mflag )
    printf( "No matches\n" );

  printf( "\n" );
}

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
  while ( fscanf( infile, "%100[^\n]", input ) != EOF ) {
    if ( fscanf( infile, "%[^\n]", input ) == 1 ) {
      fprintf( stderr, "Input line too long\n" );
      return EXIT_FAILURE;
    }
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
            if ( !redTextOn ) {
              printf( "%c%c%c%c%c", 27, '[', '3', '1', 'm' );
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
              printf( "%c%c%c%c", 27, '[', '0', 'm' );
              redTextOn = false;
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
              printf( "%c%c%c%c", 27, '[', '0', 'm' );
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
    fscanf( infile, "%*[\n]" );
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
