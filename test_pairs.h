#ifndef _TEST_HASH_H_
#define _TEST_HASH_H_

#include "pair.h"
#include <stdlib.h>
#include <string.h>

/**
 * Copies the char key of the pair.
 */
void *char_key_cpy (const_keyT key)
{
  char *new_char = malloc (sizeof (char));
  *new_char = *((char *) key);
  return new_char;
}

/**
 * Copies the int value of the pair.
 */
void *int_value_cpy (const_valueT value)
{
  int *new_int = malloc (sizeof (int));
  *new_int = *((int *) value);
  return new_int;
}

/**
 * Compares the char key of the pair.
 */
int char_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return *(char *) key_1 == *(char *) key_2;
}

/**
 * Compares the int value of the pair.
 */
int int_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *(int *) val_1 == *(int *) val_2;
}

/**
 * Frees the char key of the pair.
 */
void char_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the int value of the pair.
 */
void int_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

/**
 * Copies the String key of the pair.
 */
void *str_key_cpy (const_keyT key)
{
  char *new_str = malloc ((strlen ((char *) key) + 1)* sizeof (char));
  strcpy (new_str, (char *) key);
  return new_str;
}

/**
 * Copies the double value of the pair.
 */
void *double_value_cpy (const_valueT value)
{
  double *new_double = malloc (sizeof (double));
  *new_double = *((double *) value);
  return new_double;
}

/**
 * Compares the string key of the pair.
 */
int str_key_cmp (const_keyT key_1, const_keyT key_2)
{
  return !strcmp ((char *) key_1, (char *) key_2);
}

/**
 * Compares the double value of the pair.
 */
int double_value_cmp (const_valueT val_1, const_valueT val_2)
{
  return *((double *) val_1) == *((double *) val_2);
}

/**
 * Frees the string key of the pair.
 */
void str_key_free (keyT *key)
{
  if (key && *key)
    {
      free (*key);
      *key = NULL;
    }
}

/**
 * Frees the double value of the pair.
 */
void double_value_free (valueT *val)
{
  if (val && *val)
    {
      free (*val);
      *val = NULL;
    }
}

/**
 * @param elem pointer to a char (keyT of pair_char_int)
 * @return 1 if the char represents a digit, else - 0
 */
int is_digit (const_keyT elem)
{
  char c = *((char *) elem);
  return (c > 47 && c < 58);
}

/**
 * doubles the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void double_value (valueT elem)
{
  *((int *) elem) *= 2;
}

/**
 * @param elem pointer to a string
 * @return 1 if the string length greater then 6, else - 0
 */
int longer_then_6 (const_keyT elem)
{
  return (strlen((char *) elem) >= 6);
}

/**
 * power the value pointed to by the given pointer
 * @param elem pointer to an integer (valT of pair_char_int)
 */
void power_value (valueT elem)
{
  *((int *) elem) *=  *((int *) elem);
}
#endif

