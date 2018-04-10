/**
 * Component program that provides functions for writing and
 * reading bits to and from a file, one byte at a time, and
 * buffering bits to be used for reading and writing to and
 * from a file.
 *
 * @file bits.c
 * @author Jimmy Nguyen (jnguyen6)
 */

#include "bits.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void writeBits( const char *code, BitBuffer *buffer, FILE *fp )
{
    int index = 0;
    bool codeLeftOver = true;
    while ( codeLeftOver ) {
        while ( buffer->bcount < BITS_PER_BYTE && code[ index ] != '\0' ) {
            if ( code[ index ] == '1' ) {
                buffer->bits = buffer->bits | ( 1 << ( BITS_PER_BYTE - buffer->bcount - 1 ) );
                index++;
                buffer->bcount++;
            } else {
                buffer->bits = buffer->bits | ( 0 << ( BITS_PER_BYTE - buffer->bcount - 1 ) );
                index++;
                buffer->bcount++;
            }
        }
        //If the buffer is full, print the contents of the buffer
        //to the output file, and add any remaining characters in
        //the code to the buffer
        if ( buffer->bcount == BITS_PER_BYTE ) {
            fprintf( fp, "%c", buffer->bits );
            buffer->bits = 0x00;
            buffer->bcount = 0;
        }
        //If there is still some code left, keep storing and printing if necessary
        if ( code[ index ] == '\0' ) {
            codeLeftOver = false;
        }
    }
    
}

void flushBits( BitBuffer *buffer, FILE *fp )
{
    //If there is nothing in the buffer, then there is
    //nothing to print to the output file. Otherwise,
    //print the remaining bits to the output file,
    //including any additional 0s
    if ( buffer->bcount != 0 ) {
        fprintf( fp, "%c", buffer->bits );
        buffer->bits = 0x00;
        buffer->bcount = 0;
    }
}

int readBit( BitBuffer *buffer, FILE *fp )
{
    //If the buffer is empty, fill the buffer with up to 8 bits
    if ( buffer->bcount == 0 ) {
        buffer->bits = 0x00;
        if ( fscanf( fp, "%c", &buffer->bits ) == EOF ) {
            return -1;
        }
        buffer->bcount = BITS_PER_BYTE;
    }
    int bit;
    unsigned char bits = buffer->bits;
    if ( ( ( bits >>  ( BITS_PER_BYTE - 1 ) ) ^ 0x01 ) == 0x00 ) {
        bit = 1;
    } else {
        bit = 0;
    }
    buffer->bits = buffer->bits << 1;
    buffer->bcount--;
    return bit;
}
