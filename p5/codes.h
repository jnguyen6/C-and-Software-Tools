/**
 * Header file for the code.c component that provides prototype
 * functions for creating a code struct, which consists of a
 * symbol name and a sequence of characters represented as 0s or
 * 1s, converting a legal symbol name into a binary code that
 * represents the symbol name, and converting a binary code into
 * a legal symbol name.
 *
 *
 * @file code.h
 * @author Jimmy Nguyen (jnguyen6)
 */

#ifndef _CODES_H_
#define _CODES_H_

#include <stdio.h>
#include <stdbool.h>

/** The maximum number of code instances the code list can hold. */
#define MAX_NUM_CODES 29

/** The maximum number of 0 or 1 characters a bit sequence can have. */
#define MAX_NUM_BITS 12

/**
 * The error number that is returned in the codeToSym function when
 * a character is not found in the list of code instances.
 */
#define ERR_NUM -2

/**
 * The maximum number of characters that can be scanned and stored
 * in a character array.
 */
#define MAX_NUM_CHAR 1024

/**
 * A struct that represents a prefix code, which consists of a
 * symbol name and a sequence of 0s and 1s.
 */
typedef struct {
    /** The symbol name of the code. */
    char *name;
    /** The sequence of 0s and 1s representing bits. */
    char bits[ MAX_NUM_BITS + 1 ];
} Code;

/**
 * A struct that represents a list of code instances. The struct
 * a pointer to an array of pointers to code instances as well as
 * the number of code instances in the list.
 */
typedef struct {
    /** A pointer to the array of pointers to code instances. */
    Code **list;
    /** The number of codes currently in the list. */
    int num;
} CodeList;

/**
 * Dynamically allocates a code list instance and initializes the
 * instance's fields. The code list will be initially empty.
 */
void createCodeList( );

/**
 * Frees all the dynamically allocated memory used by the code list
 * instance. This includes the pointers to all code instances and
 * the code list instance itself.
 */
void freeCodeList( );

/**
 * Adds the code to the code list instance, in which the code to be
 * added will consist of the given name and bit sequence. If the
 * code list is at capacity, then the function returns false. Otherwise,
 * the code is added to the code list, and the function returns true.
 *
 * @param name the name of the code
 * @param bits the bit sequence of the code
 * @return true if the code is successfully added to the code list or
 * false otherwise
 */
bool addCode( char *name, char bits[] );

/**
 * Reads an input file that contains code information and stores each
 * code information in a code instance. If the given input file is
 * invalid, meaning that the code does not have legal symbol names or
 * bits that is not 0s and 1s or is too long, then the function returns
 * false. Otherwise, the function returns true, indicating that the
 * input file is successfully processed.
 *
 * @param fp the pointer to the input file containing code information
 * @return true if the input file has been successfully processed or
 * false if the input file contains invalid code information
 */
bool readCodeFile( FILE *fp );

/**
 * Returns the string that represents the bit sequence of the
 * given ASCII character or EOF. If there is no code that
 * represents the given ASCII character or EOF, then NULL is
 * returned.
 *
 * @param ch the ASCII character or EOF to find the code
 * representation for
 * @return the string that represents the code of the given ASCII
 * character or EOF or NULL if there is no code that represents
 * the given character
 */
const char * symToCode( int ch );

/**
 * Returns the ASCII character or EOF(-1) that represents the given
 * string of code. If there is no character that represents the
 * code, then -2 is returned.
 *
 * @param code the code to find character representation for
 * @return the ASCII character or EOF(-1) that represents the given
 * code or -2 if there is no character that represents the given code
 */
int codeToSym( const char *code );

#endif
