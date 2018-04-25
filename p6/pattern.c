/**
 * Component program that implements the inheritance
 * hierarchy used for interpreting regular expressions.
 * This program provides structs that represents the
 * subclasses/sub patterns of the Pattern superclass.
 * This program also provides the following functions:
 * initializing and freeing the match table, creating
 * and destroying sub patterns, finding values (true or false)
 * of the match table for the given pattern, and matching
 * sub patterns with the given string.
 *
 * @file pattern.c
 * @author Jimmy Nguyen (jnguyen6)
 * @author David Sturgill (dbsturgi)
 */

#include "pattern.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Free the table inside a pattern, if there is one.

    @param this The pattern we're supposed to operate on.
*/
static void freeTable( Pattern *this )
{
  if ( this->table ) {
    for ( int r = 0; r <= this->len; r++ )
      free( this->table[ r ] );
    free( this->table );
  }
}

/** Make and initialize a new match table inside the given pattern,
    large enough to store matches for the given string.

    @param this The pattern we're supposed to operate on.
    @param str The string we're going to store mageches for */
static void initTable( Pattern *this, char const *str )
{
  // If we already had a table, free it.
  freeTable( this );

  // Make a table big enough for str.
  this->len = strlen( str );
  this->table = (bool **) malloc( ( this->len + 1 ) * sizeof( bool * ) );
  for ( int r = 0; r <= this->len; r++ )
    this->table[ r ] = (bool *) calloc( ( this->len + 1 ), sizeof( bool ) );
}

// Documented in the header.
bool matches( Pattern *pat, int begin, int end )
{
  return pat->table[ begin ][ end ];
}

/**
   A simple function that can be used to free the memory for any
   pattern that doesn't allocate any additional memory other than the
   struct used to represent it (e.g., if it doesn't contain any
   sub-patterns).  It's like a default implementation of the destroy
   method.

   @param pat The pattern to free memory for.
*/
static void destroySimplePattern( Pattern *pat )
{
  freeTable( pat );
  // If we don't need fields that are specific to the sub-type, we can just
  // free the block of memory where the object is stored.
  free( pat );
}

/**
   Type of pattern used to represent a single, ordinary symbol,
   like 'a' or '5'.
*/
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );
  
  /** Symbol this pattern is supposed to match. */
  char sym;
} SymbolPattern;


// Overridden locate() method for a SymbolPattern
static void locateSymbolPattern( Pattern *pat, char const *str )
{
  // Cast down to the struct type pat really points to.
  SymbolPattern *this = (SymbolPattern *) pat;

  // Make a fresh table for this input string.
  initTable( pat, str );

  // Find all occurreces of the symbol we're supposed to match, and
  // mark them in the match table as matching, 1-character substrings.
  for ( int begin = 0; str[ begin ]; begin++ )
    if ( str[ begin ] == this->sym )
      this->table[ begin ][ begin + 1 ] = true;
}

// Documented in the header.
Pattern *makeSymbolPattern( char sym )
{
  // Make an instance of SymbolPattern, and fill in its state.
  SymbolPattern *this = (SymbolPattern *) malloc( sizeof( SymbolPattern ) );
  this->table = NULL;
  
  this->locate = locateSymbolPattern;
  this->destroy = destroySimplePattern;
  this->sym = sym;

  return (Pattern *) this;
}

/**
   Representation for a type of pattern that contains two sub-patterns
   (e.g., concatenation).  This representation could be used by more
   than one type of pattern, as long as it uses a pointer to a
   different locate() function.
*/
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );
  
  // Pointers to the two sub-patterns.
  Pattern *p1, *p2;
} BinaryPattern;

// destroy function used for BinaryPattern
static void destroyBinaryPattern( Pattern *pat )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  // Free our table.
  freeTable( pat );
  // Free our two sub-patterns.
  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  // Free the struct representing this object.
  free( this );
}

// locate function for a BinaryPattern used to handle concatenation.
static void locateConcatenationPattern( Pattern *pat, const char *str )
{
  // Cast down to the struct type pat really points to.
  BinaryPattern *this = (BinaryPattern *) pat;

  initTable( pat, str );

  //  Let our two sub-patterns figure out everywhere they match.
  this->p1->locate( this->p1, str );
  this->p2->locate( this->p2, str );

  // Then, based on their matches, look for all places where their
  // concatenaton matches.  Check all substrings of the input string.
  for ( int begin = 0; begin <= this->len; begin++ )
    for ( int end = begin; end <= this->len; end++ ) {
    
      // For the [ begin, end ) range, check all places where it could
      // be split into two substrings, the first matching p1 and the second
      // matching p2.
      for ( int k = begin; k <= end; k++ )
        if ( matches( this->p1, begin, k ) &&
             matches( this->p2, k, end ) )
          this->table[ begin ][ end ] = true;
    }
}

// Documented in header.
Pattern *makeConcatenationPattern( Pattern *p1, Pattern *p2 )
{
  // Make an instance of Binary pattern and fill in its fields.
  BinaryPattern *this = (BinaryPattern *) malloc( sizeof( BinaryPattern ) );
  this->table = NULL;
  this->p1 = p1;
  this->p2 = p2;
  
  this->locate = locateConcatenationPattern;
  this->destroy = destroyBinaryPattern;
  
  return (Pattern *) this;
}

/** Pattern that represents metacharacters, like '.', '$', and '^'. */
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );
  
  /** The metacharacter this pattern is supposed to match. */
  char metachar;
} MetacharPattern;

/**
 * Overridden locate() method for a MetacharPattern. This
 * method finds a single occurrence of a character that corresponds
 * with the metacharacter pattern.
 *
 * @param pat the metacharacter pattern
 * @param str the string to find a match for
 */
static void locateMetacharPattern( Pattern *pat, char const *str )
{
  // Cast down to the struct type pat really points to.
  MetacharPattern *this = (MetacharPattern *) pat;

  // Make a fresh table for this input string.
  initTable( pat, str );

  if ( this->metachar == '.' ) {
    if ( strlen( str ) != 0 ) {
      for ( int begin = 0; str[ begin ]; begin++ ) {
        this->table[ begin ][ begin + 1 ] = true;
      }
    }
  } else if ( this->metachar == '^' ) {
    this->table[ 0 ][ 0 ] = true;
  } else {
    this->table[ pat->len ][ pat->len ] = true;
  }
}

// Documented in header.
Pattern *makeMetacharPattern( char metachar )
{
  MetacharPattern *this = (MetacharPattern *) malloc( sizeof( MetacharPattern ) );
  this->table = NULL;
  this->locate = locateMetacharPattern;
  this->destroy = destroySimplePattern;
  this->metachar = metachar;
  
  return (Pattern *) this;
}


/** Pattern that represents the character class. */
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );
  
  /** The character class this pattern is supposed to match. */
  char *cclass;
} CharClassPattern;

/** Destroy function used for CharacterClassPattern. */
static void destroyCharClassPattern( Pattern *pat )
{
  CharClassPattern *this = (CharClassPattern *) pat;

  // Free our table.
  freeTable( pat );
  //Free char pointer
  free( this->cclass );
  // Free the struct representing this object.
  free( this );
}

/**
 * Overridden locate() method for a CharClassPattern. This
 * method finds a single occurrence of a character that corresponds
 * any character from the character class.
 *
 * @param pat the character class pattern
 * @param str the string to find a match for
 */
static void locateCharClassPattern( Pattern *pat, char const *str )
{
  // Cast down to the struct type pat really points to.
  CharClassPattern *this = (CharClassPattern *) pat;

  // Make a fresh table for this input string.
  initTable( pat, str );
  
  //First, check if there is a ^ at the beginning of a character class.
  bool inverted = false;
  if ( this->cclass[ 0 ] == '^' && strlen( this->cclass ) != 1 ) {
    inverted = true;
  }
  
  if ( !inverted ) {
    for ( int begin = 0; str[ begin ]; begin++ ) {
      for ( int i = 0; this->cclass[ i ]; i++ ) {
        if ( str[ begin ] == this->cclass[ i ] ) {
          if ( str[ begin ] == '-' && this->cclass[ i ] == '-' ) {
            if ( !( i - 1 >= 1 && i + 1 <= strlen( this->cclass ) - 1 ) ) {
              this->table[ begin ][ begin + 1 ] = true;
              break;
            }
          } else {
            this->table[ begin ][ begin + 1 ] = true;
            break;
          }
        } else if ( this->cclass[ i ] == '-' ) {
          if ( i - 1 >= 0 && i + 1 <= strlen( this->cclass ) ) {
            if ( str[ begin ] >= this->cclass[ i - 1 ]
                 && str[ begin ] <= this->cclass[ i + 1 ] ) {
              this->table[ begin ][ begin + 1 ] = true;
              break;
            }
          }
        }
      }
    }
  } else {
    for ( int begin = 0; str[ begin ]; begin++ ) {
      bool foundMatch = false;
      for ( int i = 1; this->cclass[ i ]; i++ ) {
        if ( str[ begin ] == this->cclass[ i ] ) {
          if ( str[ begin ] == '-' && this->cclass[ i ] == '-' ) {
            if ( this->cclass[ i - 1 ] == '^' ) {
              foundMatch = true;
              break;
            }
          } else {
            foundMatch = true;
            break;
          }
        } else if ( this->cclass[ i ] == '-' ) {
          if ( i - 1 >= 1 && i + 1 <= strlen( this->cclass ) ) {
            if ( str[ begin ] >= this->cclass[ i - 1 ]
                 && str[ begin ] <= this->cclass[ i + 1 ] ) {
              foundMatch = true;
              break;
            }
          }
        }
      }
      if ( !foundMatch ) {
        this->table[ begin ][ begin + 1 ] = true;
      }
    }
  }
}

// Documented in header.
Pattern *makeCharClassPattern( char *str )
{
  CharClassPattern *this = (CharClassPattern *) malloc( sizeof( CharClassPattern ) );
  this->table = NULL;
  this->locate = locateCharClassPattern;
  this->destroy = destroyCharClassPattern;
  
  this->cclass = (char *) malloc( strlen( str ) + 1 );
  for ( int i = 0; str[ i ]; i++ ) {
    this->cclass[ i ] = str[ i ];
  }
  this->cclass[ strlen( str ) ] = '\0';
  return (Pattern *) this;
}

/** Pattern that represents the alternation between two pattern types. */
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );
  
  /** Pointers to the two sub patterns. */
  Pattern *p1, *p2;
} AlternationPattern;

/** Destroy function used for AlternationPattern. */
static void destroyAlternationPattern( Pattern *pat )
{
  // Cast down to the struct type pat really points to.
  AlternationPattern *this = (AlternationPattern *) pat;

  // Free our table.
  freeTable( pat );
  // Free our two sub-patterns.
  this->p1->destroy( this->p1 );
  this->p2->destroy( this->p2 );
  // Free the struct representing this object.
  free( this );
}

/**
 * Overridden locate() method for an AlternationPattern. This
 * method finds a match for a given string based on the two
 * pattern types that the AlternationPattern supports.
 *
 * @param pat the alternation pattern
 * @param str the string to find a match for
 */
static void locateAlternationPattern( Pattern *pat, const char *str )
{
  // Cast down to the struct type pat really points to.
  AlternationPattern *this = (AlternationPattern *) pat;

  initTable( pat, str );
  
  this->p1->locate( this->p1, str );
  this->p2->locate( this->p2, str );
  
  bool foundMatch = false;
  for ( int begin = 0; begin <= this->len; begin++ ) {
    for ( int end = begin; end <= this->len; end++ ) {
      if ( matches( this->p1, begin, end ) ) {
        foundMatch = true;
      }
    }
  }
  
  if ( foundMatch ) {
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        if ( matches( this->p1, begin, end ) ) {
          this->table[ begin ][ end ] = true;
        }
      }
    }
  } else {
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        if ( matches( this->p2, begin, end ) ) {
          this->table[ begin ][ end ] = true;
        }
      }
    }
  }
}

// Documented in header.
Pattern *makeAlternationPattern( Pattern *p1, Pattern *p2 )
{
  // Make an instance of Binary pattern and fill in its fields.
  AlternationPattern *this = (AlternationPattern *) malloc( sizeof( AlternationPattern ) );
  this->table = NULL;
  this->p1 = p1;
  this->p2 = p2;
  
  this->locate = locateAlternationPattern;
  this->destroy = destroyAlternationPattern;
  
  return (Pattern *) this;
}

/**
 * Pattern that represents the repetition pattern. The repetition pattern
 * can either be represented as a '+', '*', or a '?'.
 */
typedef struct {
  // Fields from our superclass.
  int len;
  bool **table;
  void (*locate)( Pattern *pat, char const *str );
  void (*destroy)( Pattern *pat );
  
  /** The pointer to the pattern being matched. */
  Pattern *pat;
  /** The character representation of the repetition pattern. */
  //char rpat;
  char *rpat;
} RepetitionPattern;

/** Destroy function used for RepetitionPattern. */
static void destroyRepetitionPattern( Pattern *pat )
{
  // Cast down to the struct type pat really points to.
  RepetitionPattern *this = (RepetitionPattern *) pat;

  freeTable( pat );
  this->pat->destroy( this->pat );
  free( this->rpat );
  free( this );
}

/**
 * Overridden locate() method for a RepetitionPattern. This
 * method finds 0, 1, or multiple occurrences of a character
 * from the given string, depending on the repetition pattern.
 *
 * @param pat the repetition pattern
 * @param str the string to find a match for
 */
static void locateRepetitionPattern( Pattern *pat, char const *str )
{
  RepetitionPattern *this = (RepetitionPattern *) pat;

  // Make a fresh table for this input string.
  initTable( pat, str );
  
  if ( this->rpat[ 0 ] == '*' ) {
    this->pat->locate( this->pat, str );
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        if ( matches( this->pat, begin, end ) ) {
          this->table[ begin ][ end ] = true;
          if ( end != this->len && ( matches( this->pat, begin, end )
                 && matches( this->pat, begin + 1, end + 1 ) ) ) {
            this->table[ begin ][ end + 1 ] = true;
          }
        } else if ( begin == end ) {
          this->table[ begin ][ end ] = true;
        }
      }
    }
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        for ( int k = begin; k <= end; k++ ) {
          if ( this->table[ begin ][ k ] && this->table[ k ][ end ] ) {
            this->table[ begin ][ end ] = true;
          }
        }
      }
    }
  } else if ( this->rpat[ 0 ] == '+' ) {
    //SymbolPattern *temp = (SymbolPattern *) this->pat;
    //str[ begin ] == temp->sym
    this->pat->locate( this->pat, str );
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        if ( matches( this->pat, begin, end ) ) {
          this->table[ begin ][ end ] = true;
          if ( end != this->len && ( matches( this->pat, begin, end )
                 && matches( this->pat, begin + 1, end + 1 ) ) ) {
            this->table[ begin ][ end + 1 ] = true;
          }
        }
      }
    }
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        for ( int k = begin; k <= end; k++ ) {
          if ( this->table[ begin ][ k ] && this->table[ k ][ end ] ) {
            this->table[ begin ][ end ] = true;
          }
        }
      }
    }
  } else if ( this->rpat[ 0 ] == '?' ) {
    this->pat->locate( this->pat, str );
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        if ( matches( this->pat, begin, end ) ) {
          this->table[ begin ][ end ] = true;
        } else if ( begin == end ) {
          this->table[ begin ][ end ] = true;
        }
      }
    }
  } else {
    this->pat->locate( this->pat, str );
    //We know the first and last positions of the sequence is a '{'
    //and a '}', respectively. So, we need to first find the comma
    //and then check which types of consecutive occurrences to consider.
    int start;
    int end;
    for ( int i = 0; this->rpat[ i ]; i++ ) {
      if ( this->rpat[ i ] == ',' ) {
        if ( this->rpat[ i - 1 ] == '{' ) {
          start = 0;
        } else {
          start = (int) this->rpat[ i - 1 ];
        }
        if ( this->rpat[ i + 1 ] == '}' ) {
          //This negative value is used to represent m (infinitely many)
          //possible occurrences.
          end = -1;
        } else {
          end = (int) this->rpat[ i + 1 ];
        }
        break;
      }
    }
    
    //Once we have got the integer values (start and end), we should
    //then find any matches and keep track of the number of matches
    //we've found.
    int count = 0;
    for ( int begin = 0; begin <= this->len; begin++ ) {
      for ( int end = begin; end <= this->len; end++ ) {
        if ( matches( this->pat, begin, end ) ) {
            count++;
        }
      }
    }
    
    //Now, compare the count to the start and end values. If the count
    //value is not between the start and end values, then don't iterate
    //through the match table anymore. Otherwise, iterate through the
    //table and mark the appropriate location with the appropriate value.
    bool isValid = false;
    if ( end == -1 ) {
      if ( count >= start ) {
        isValid = true;
      }
    } else {
      if ( count >= start && count <= end ) {
        isValid = true;
      }
    }
    if ( isValid ) {
      for ( int begin = 0; begin <= this->len; begin++ ) {
        for ( int end = begin; end <= this->len; end++ ) {
          if ( matches( this->pat, begin, end ) ) {
            this->table[ begin ][ end ] = true;
            if ( end != this->len && ( matches( this->pat, begin, end )
                 && matches( this->pat, begin + 1, end + 1 ) ) ) {
              this->table[ begin ][ end + 1 ] = true;
            }
          } else if ( begin == end && start == 0 ) {
            this->table[ begin ][ end ] = true;
          }
        }
      }
      for ( int begin = 0; begin <= this->len; begin++ ) {
        for ( int end = begin; end <= this->len; end++ ) {
          for ( int k = begin; k <= end; k++ ) {
            if ( this->table[ begin ][ k ] && this->table[ k ][ end ] ) {
              this->table[ begin ][ end ] = true;
            }
          }
        }
      }
    }
  }
}

// Documented in header.
Pattern *makeRepetitionPattern( Pattern *pat, char *rpat )
{
  RepetitionPattern *this = (RepetitionPattern *) malloc( sizeof( RepetitionPattern ) );
  this->table = NULL;
  this->locate = locateRepetitionPattern;
  this->destroy = destroyRepetitionPattern;
  this->pat = pat;
  this->rpat = (char *) malloc( strlen( rpat ) + 1 );
  for ( int i = 0; rpat[ i ]; i++ ) {
    this->rpat[ i ] = rpat[ i ];
  }
  //this->rpat = rpat;
  
  return (Pattern *) this;
}
