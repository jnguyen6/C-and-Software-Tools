/**
 * Header file that provides a prototype function
 * for parsing the given string into a specific
 * Pattern object.
 *
 * @file parse.h
 * @author Jimmy Nguyen (jnguyen6)
 * @author David Sturgill (dbsturgi)
 */

#ifndef PARSE_H
#define PARSE_H

#include "pattern.h"

/** Parse the given string into Pattern object.

    @param str string cntaining a pattern.
    @return pointer to a representation of the pattern.
***/
Pattern *parsePattern( char const *str );

#endif
