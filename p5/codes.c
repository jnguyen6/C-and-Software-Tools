/**
 * Component program that provides functions for returning
 * a string representing a sequence of bits that corresponds
 * with the given ASCII characer or EOF and returning an ASCII
 * character or EOF that corresponds with the given sequence
 * of bits. The component program also provides a struct that
 * is used to maintain a list of code instances.
 *
 * @file code.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "codes.h"
#include <stdlib.h>
#include <string.h>

/** The global variable that represents the pointer to the code list struct. */
static CodeList *cptr;

void createCodeList( )
{
    CodeList *codelist = (CodeList *) malloc( sizeof( CodeList ) );
    codelist->num = 0;
    codelist->list = (Code **) malloc( MAX_NUM_CODES * sizeof( Code* ) );
    for ( int i = 0; i < MAX_NUM_CODES; i++ ) {
        codelist->list[ i ] = NULL;
    }
    cptr = codelist;
}

void freeCodeList( )
{
    if ( cptr != NULL ) {
        for ( int i = 0; i < cptr->num; i++ ) {
            if ( cptr->list[ i ]->name != NULL ) {
                free( cptr->list[ i ]->name );
            }
            free( cptr->list[ i ] );
        }
        free( cptr->list );
        free( cptr );
    }
}

bool addCode( char *name, char bits[] )
{
    if ( cptr->num > MAX_NUM_CODES ) {
        return false;
    }
    cptr->list[ cptr->num ] = (Code *) malloc( sizeof( Code ) );
    cptr->list[ cptr->num ]->name = (char *) malloc( strlen( name ) + 1 * sizeof( char ) );
    strcpy( cptr->list[ cptr->num ]->name, name );
    strcpy( cptr->list[ cptr->num ]->bits, bits );
    cptr->num++;
    return true;
}

bool readCodeFile( FILE *fp )
{
    char name[ MAX_NUM_CHAR + 1 ];
    char bits[ MAX_NUM_BITS + 1 ];
    int numCodesAdded = 0;
    while ( fscanf( fp, "%1024s %12s", name, bits ) != EOF ) {
        //First, check if the name is valid
        if ( strlen( name ) != 1 ) {
            if ( strcmp( name, "space" ) != 0 && strcmp( name, "newline" ) != 0
                && strcmp( name, "eof" ) != 0 ) {
                return false;
            }
        } else {
            if ( name[ 0 ] < 'a' || name[ 0 ] > 'z' ) {
                return false;
            }
        }
        //Then check if the sequence of bits is valid
        if ( bits[ 0 ] == '\0' ) {
            return false;
        }
        if ( fscanf( fp, "*^[ \n]" ) == 1 ) {
            return false;
        }
        for ( int i = 0; bits[ i ]; i++ ) {
            if ( bits[ i ] != '0' && bits[ i ] != '1' ) {
                return false;
            }
        }
        //If the codes array is full, and there is still more code to
        //add, then print error message and exit with status of 1
        if ( !addCode( name, bits ) ) {
            return false;
        }
        numCodesAdded++;
        for ( int i = 0; name[ i ]; i++ ) {
            name[ i ] = '\0';
        }
        for ( int i = 0; bits[ i ]; i++ ) {
            bits[ i ] = '\0';
        }
    }
    
    if ( numCodesAdded != MAX_NUM_CODES ) {
        return false;
    }
    return true;
}

const char * symToCode( int ch )
{
    if ( ch != ' ' - '0' && ch != '\n' - '0' && ch != -1 ) {
        for ( int i = 0; i < cptr->num; i++ ) {
            if ( ch == cptr->list[ i ]->name[ 0 ] - '0' ) {
                return cptr->list[ i ]->bits;
            }
        }
    } else {
        for ( int i = 0; i < cptr->num; i++ ) {
            if ( strcmp( cptr->list[ i ]->name, "space" ) == 0 && ch == ' ' - '0' ) {
                return cptr->list[ i ]->bits;
            } else if ( strcmp( cptr->list[ i ]->name, "newline" ) == 0 && ch == '\n' - '0' ) {
                return cptr->list[ i ]->bits;
            } else if ( strcmp( cptr->list[ i ]->name, "eof" ) == 0 && ch == -1 ) {
                return cptr->list[ i ]->bits;
            }
        }
    }
    return NULL;
}

int codeToSym( const char *code )
{
    for ( int i = 0; i < cptr->num; i++ ) {
        if ( strcmp( cptr->list[ i ]->bits, code ) == 0 ) {
            if ( strcmp( cptr->list[ i ]->name, "space" ) == 0 ) {
                return ' ';
            } else if ( strcmp( cptr->list[ i ]->name, "newline" ) == 0 ) {
                return '\n';
            } else if ( strcmp( cptr->list[ i ]->name, "eof" ) == 0 ) {
                return -1;
            } else {
                return cptr->list[ i ]->name[ 0 ];
            }
        }
    }
    return ERR_NUM;
}
