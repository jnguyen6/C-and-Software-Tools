#ifndef PATTERN_H
#define PATTERN_H

#include <stdbool.h>

//////////////////////////////////////////////////////////////////////
// Superclass for Patterns

/** A short name to use for the Pattern interface. */
typedef struct PatternStruct Pattern;

/** 
  Structure used as a superclass/interface for a regular expression
  pattern.  It includes a representation for a resizable 2D table
  where we can record regions (substrings) of an input string that are
  matched by the pattern.  There's a function pointer for an
  overridable method, locate(), that uses the table to mark all the
  places where the pattern matches the input string, and another
  overridable method for freeing resources for the pattern.
*/
struct PatternStruct {
  /** Length of the current input string, as recorded by the latest call
      to locate(). */
  int len;
  
  /** The match table, a (len + 1) X (len + 1) 2D array represented as
      an array of pointers to arrays.  The job of the locate() functon
      is to fill in this table so table[ begin ][ end ] is true if
      this pattern can match the [ begiin, end ) substring of the
      current input string.  */
  bool **table;

  /** Relallocate the match table.  Find all the [ begin, end )
      substrings of input string, str, that match the pattern.  For
      any match, set table[ begin ][ end ] to true in the match
      table.

      @param pat pointer to the pattern being matched (essentially, a this
                 pointer.
      @param str input string in which we're finding matches.
  */
  void (*locate)( Pattern *pat, char const *str );

  /** Free memory for this pattern, including any subpatterns it contains.
      @param pat pattern to free.
  */
  void (*destroy)( Pattern *pat );
};

/** Report elements of the match table for the given pattern.  This
    can be called after the pattern's locate() function, to see where
    it found a match.  It's like a non-overridable instance method for
    the Pattern object.

    @param pat pointer to the pattern for which we want to check
               reported matches.
    @param begin index of the first character in the substring
    @param end index one-past-the-end of the string
    @return true if this pattern matches the [ begin, end ) substring
            of the most recent input string
 */
bool matches( Pattern *pat, int begin, int end );

/** 
  Make a pattern for a single, non-special character, like `a` or `5`.

  @param sym The symbol this pattern is supposed to match.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeSymbolPattern( char sym );

/** 
  Make a pattern for the concatenation of patterns p1 and p2.  It
  should match anything that can be broken into two substrings, s1 and
  s2, where the p1 matches the first part (s1) and p2 matches the
  second part (s2).

  @param p1 Subpattern for matching the first part of the string.
  @param p2 Subpattern for matching the second part of the string.
  @return A dynamically allocated representation for this new pattern.
*/
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 );

/**
 * Make a pattern for a metacharacter, which are special characters like 
 * '.', '$', and '^'.
 *
 * @param metachar the metacharacter pattern
 * @return a dynamically allocated representation for this new pattern
 */
Pattern *makeMetacharPattern( char metachar );

/**
 * Make a pattern for a character class. The character class consists of
 * a sequence of characters that are inside square brackets (Ex. [abc] ).
 * The pattern should match any single occurrence of a character from a
 * given substring.
 *
 * @param str the string that represents the character class pattern
 * @return a dynamically allocated representation for this new pattern
 */
Pattern *makeCharClassPattern( char *str );

/**
 * Make a pattern for the alternation between two pattern types. This pattern
 * should match anything that matches with either one of the two pattern types.
 *
 * @param p1 the subpattern for matching the first pattern type
 * @param p2 the subpattern for matching the second pattern type
 * @return a dynamically allocated representation for this new pattern
 */
Pattern *makeAlternationPattern( Pattern *p1, Pattern *p2 );

/**
 * Make a pattern for the repetition of a pattern. This pattern should
 * match 0, 1, or possibly multiple occurrences of a given string to match,
 * depending on the repetition pattern.
 *
 * @param pat the pointer to the pattern being matched
 * @param rpat the repetition pattern as a single character
 * @return a dynamically allocated representation for this new pattern
 */
Pattern *makeRepetitionPattern( Pattern *pat, char rpat );

#endif
