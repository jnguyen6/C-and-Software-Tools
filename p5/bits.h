/**
    @file bits.h
    @author Suzanne Balik (spbalik)

    Header file for the bits.c component, with functions supporting
    reading and writing a file a byte at a time and buffering bits for
    codes that extend over byte boundaries.
*/

#ifndef _BITS_H_
#define _BITS_H_

#include <stdio.h>

/** Number of bits per byte.  This isn't going to change, but it lets us give
    a good explanation instead of just the literal value, 8. */
#define BITS_PER_BYTE 8

/** Buffer space for up to 8 bits that have been written by the
    application but haven't yet been written out to a file, or that
    have been read from a file but not yet used by the application.
    When this structure is initialized, zeros should be stored in both
    fields.
*/
typedef struct {
  /** Storage for up to 8 bits left over from an earlier read or waiting
      to be written in a subsequent write. */
  unsigned char bits;

  /** Number of bits currently buffered. */
  int bcount;
} BitBuffer;

/** Write the code stored in the code parameter.  Temporarily
    store bits in the given buffer until we have 8 of them and can
    write them to the file.
    @param code bit string for code
    @param buffer pointer to storage for bits that can't be written to the
    file yet.  When this function is called, this buffer may already
    contain some bits left-over from a previous write.
    @param fp file we're writing to, opened for writing in binary mode.
*/
void writeBits( const char *code, BitBuffer *buffer, FILE *fp );


/** If there are any bits buffered in buffer, write them out to the
    given file in the high-order bit positions of a byte, leaving zeros
    in the low-order bits.
    @param buffer pointer to storage for unwritten bits left over
    from a previous write.
    @param fp file these bits are to be written to, opened for writing
    in binary mode
*/
void flushBits( BitBuffer *buffer, FILE *fp );


/** Reads and returns value of the next bit (0 or 1) or -1 if the end of file has been reached. 
    The given buffer may contain some bits left over from the last read, and if this
    the read has any left-over bits, it should leave them in that buffer.
    @param buffer pointer to storage for left-over from one read call to the
    next.
    @param fp file bits are being read from, opened for reading in binary.
    @return value of the next bit (0 or 1), -1 if we reach the
    end-of-file.
*/
int readBit( BitBuffer *buffer, FILE *fp );

#endif
