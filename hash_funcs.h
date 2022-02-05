#ifndef HASHFUNCS_H_
#define HASHFUNCS_H_

#include <stdlib.h>
#include <string.h>
#define CONST 1

/**
 * Integers simple hash func.
 */
size_t hash_int(const void *elem){
    size_t hash = *((int *) elem);
    return hash;
}

/**
 * Chars simple hash func.
 */
size_t hash_char(const void *elem){
    size_t hash = *((char *) elem);
    return hash;
}

/**
 * Doubles simple hash func.
 */
size_t hash_double(const void *elem){
    size_t hash = *((double *) elem);
    return hash;
}

/**
 * String simple hash func.
 */
size_t hash_string(const void *elem){
  size_t hash = 0;
  char *s = (char *) elem;
  for (size_t i = 0; i < strlen(s); i++)
    hash += s[i];
  hash += strlen(s);
  return hash;
}

/**
 * Hash all the elements to constant key
 */
size_t hash_const(const void *elem){
  return *((int *) elem) * 0 + CONST;
}

#endif // HASHFUNCS_H_