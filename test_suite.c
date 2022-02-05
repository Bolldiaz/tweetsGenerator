
#include "test_pairs.h"
#include "hash_funcs.h"
#include "test_suite.h"
#include "hashmap.h"
#include <stdlib.h>
#include <assert.h>

#define FAIL 0
#define SUCCESS 1

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert (void)
{
  // initializing hash map
  hashmap *map = hashmap_alloc (hash_char);
  assert (map != NULL && "INSERTION-TEST: Failed to allocate hash map");

  // Create Pairs:
  void *pairs[26];
  for (int j = 0; j < 26; ++j)
    {
      // keys are capital letters, values are {0, ... ,25}
      char key = (char) (j + 65);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      assert (pairs[j] != NULL && "INSERTION-TEST: Failed to allocate pair");
    }

  // generic insertion test1:
  for (int j = 0; j < 13; ++j)
    {
      int insertion_flag = hashmap_insert (map, pairs[j]);
      assert (insertion_flag == SUCCESS
              && "INSERTION-TEST: Failed to insert pair.");
    }
  // ensure capacity updated, and rehashing took place
  assert (map->capacity == HASH_MAP_INITIAL_CAP * 2
          && "INSERTION-TEST: Table size didn't resize.");

  // generic insertion test2:
  for (int j = 13; j < 26; ++j)
    {
      int insertion_flag = hashmap_insert (map, pairs[j]);
      assert (insertion_flag == SUCCESS
              && "INSERTION-TEST: Failed to insert pair.");
    }
  // ensure capacity updated, and rehashing took place
  assert (map->capacity == HASH_MAP_INITIAL_CAP * 4
          && "INSERTION-TEST: Table size didn't resize.");

  // ensure double insertion not working
  int insertion_flag = hashmap_insert (map, pairs[0]);
  assert (insertion_flag == FAIL && "INSERTION-TEST: Hashed 2 pairs with "
                                    "same keys.");

  // clear hash-map
  hashmap_free (&map);
  assert (map == NULL && "INSERTION-TEST: Failed to free the hash-map.");

  // frees pairs[16:25]:
  for (int j = 16; j < 26; ++j)
    {
      pair_free (&(pairs[j]));
      assert (pairs[j] == NULL && "INSERTION-TEST: Failed to free pair.");
    }

  // alloc a new map, use a const hashing function -> send all the keys to 1:
  map = hashmap_alloc (hash_const);

  // generic insertion test1:
  for (int j = 0; j < 16; ++j)
    {
      int insertion_with_const_flag = hashmap_insert (map, pairs[j]);
      assert (insertion_with_const_flag == SUCCESS
              && "INSERTION-TEST: Failed to insert pair using const func.");
    }

  // ensure capacity updated, and rehashing took place
  assert (map->capacity == HASH_MAP_INITIAL_CAP * 2
          && "INSERTION-TEST: Table size didn't resized.");

  // ensure capacity of specific vector was updated:
  assert (map->buckets[1]->capacity == VECTOR_INITIAL_CAP * 2
          && "INSERTION-TEST: Vector size didn't resized.");

  // clear hash-map
  hashmap_free (&map);
  assert (map == NULL && "INSERTION-TEST: Failed to free the hash-map.");

  // frees pairs[0:15]:
  for (int j = 0; j < 16; ++j)
    {
      pair_free (&(pairs[j]));
      assert (pairs[j] == NULL && "INSERTION-TEST: Failed to free pair.");
    }

}

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at (void)
{
  // initializing hash map
  hashmap *map = hashmap_alloc (hash_char);
  assert (map != NULL && "HASHMAP-AT-TEST: Failed to allocate hash map");

  // Create char-to-int Pairs:
  void *pairs[26];
  for (int j = 0; j < 26; ++j)
    {
      // keys are capital letters, values are {0, ... ,25}
      char key = (char) (j + 65);
      int value = j;
      pairs[j] = pair_alloc (&key, &value, char_key_cpy, int_value_cpy,
                             char_key_cmp, int_value_cmp, char_key_free,
                             int_value_free);
      assert (
          pairs[j] != NULL && "HASHMAP-AT-TEST: Failed to allocate pair");
    }

  // ensure correct value return for valid keys:
  for (int j = 0; j < 26; ++j)
    {
      int insertion_flag = hashmap_insert (map, pairs[j]);
      assert (insertion_flag == SUCCESS
              && "HASHMAP-AT-TEST: Failed to insert pair.");

      pair *cur_pair = (pair *) (pairs[j]);
      assert (int_value_cmp (hashmap_at (map, cur_pair->key), cur_pair->value)
              && "HASHMAP-AT-TEST: Wrong value returned for inserted key.");
    }

  // ensure after-erase not found:
  pair *cur_pair = (pair *) (pairs[0]);
  int action_flag = hashmap_erase (map, cur_pair->key);
  assert (action_flag == SUCCESS
          && "HASHMAP-AT-TEST: Failed to erase pair.");

  assert (hashmap_at (map, cur_pair->key) == NULL
          && "HASHMAP-AT-TEST: ERROR-> Found value in map, for invalid key.");

  // ensure NULL return for invalid keys
  char c = (char) 0;
  const_keyT invalid_key = (void *) &c;
  assert (hashmap_at (map, invalid_key) == NULL
          && "HASHMAP-AT-TEST: ERROR-> Found value in map, for invalid key.");

  // clear hash-map
  hashmap_free(&map);
  assert (map == NULL && "INSERTION-TEST: Failed to free the hash-map.");

  // frees pairs:
  for (int j = 0; j < 26; ++j)
    {
      pair_free (&(pairs[j]));
      assert (pairs[j] == NULL && "INSERTION-TEST: Failed to free pair.");
    }

}

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase (void)
{
  // initializing hash map
  hashmap *map = hashmap_alloc (hash_char);
  assert (map != NULL && "ERASE-TEST: Failed to allocate hash map");

  void *pairs[16];
  char *keys[] = {"dog", "home", "liverpool", "machine", "seat", "hi",
                  "forest", "brain", "dave", "?*&^", "bond", "story",
                  "long-term", "bolldiaz", "mutable", "thiago"};
  double values[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0,
                     10.0, 11.0, 12.0, 13.0, 14.0, 15.0};

  // allocate the pairs:
  for (int j = 0; j < 16; ++j)
    {
      pairs[j] = pair_alloc (keys[j], &values[j], str_key_cpy,
                             double_value_cpy, str_key_cmp,
                             double_value_cmp, str_key_free, double_value_free);
      assert (pairs[j] != NULL && "ERASE-TEST: Failed to allocate pair");
    }

  // check rehashing functionality for decreasing the table
  for (int j = 0; j < 3; ++j)
    {
      size_t prev_capacity = map->capacity;
      pair *cur_pair = (pair *) (pairs[j]);

      int action_flag = hashmap_insert (map, cur_pair);
      assert (action_flag == SUCCESS
              && "ERASE-TEST: Failed to insert pair.");

      action_flag = hashmap_erase (map, cur_pair->key);
      assert (action_flag == SUCCESS
              && "ERASE-TEST: Failed to erase pair.");

      // ensure rehash took place:
      assert (map->capacity == prev_capacity / 2
              && "ERASE-TEST: Failed to rehash map.");
    }

  for (int i = 0; i < 16; ++i)
    {
      int action_flag = hashmap_insert (map, pairs[i]);
      assert (action_flag == SUCCESS
              && "ERASE-TEST: Failed to insert pair.");
    }

  for (int i = 7; i < 16; ++i)
    {
      pair *cur_pair = (pair *) (pairs[i]);
      int action_flag = hashmap_erase (map, cur_pair->key);
      assert (action_flag == SUCCESS
              && "ERASE-TEST: Failed to erase pair.");
    }

  // ensure last-rehash took place:
  assert (map->capacity == HASH_MAP_INITIAL_CAP
          && "ERASE-TEST: Failed to rehash map.");

  // ensure double erasing failed -> 0
  pair *cur_pair = (pair *) (pairs[7]);
  int action_flag = hashmap_erase (map, cur_pair->key);
  assert (action_flag == FAIL
          && "ERASE-TEST: Double-erasing took place.");

  // ensure erasing invalid key failed -> 0
  const_keyT invalid_key = (void *) ("invalid-key");
  action_flag = hashmap_erase (map, invalid_key);
  assert (action_flag == FAIL
          && "ERASE-TEST: Erasing invalid-key took place.");

  // clear hash-map
  hashmap_free(&map);
  assert (map == NULL && "INSERTION-TEST: Failed to free the hash-map.");

  // frees pairs:
  for (int j = 0; j < 16; ++j)
    {
      pair_free (&(pairs[j]));
      assert (pairs[j] == NULL && "INSERTION-TEST: Failed to free pair.");
    }
}

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor (void)
{
  // base case:
  hashmap *map = NULL;
  assert (hashmap_get_load_factor (map) == -1
          && "TEST-LOAD_FACTOR: hashmap=NULL, yet -1 not returned.");

  // base case:
  map = hashmap_alloc (hash_const);
  map->capacity = 0;
  assert (hashmap_get_load_factor (map) == -1
          && "TEST-LOAD_FACTOR: capacity=0, yet -1 not returned.");

  // initial case:
  map->capacity = HASH_MAP_INITIAL_CAP;
  assert (hashmap_get_load_factor (map) == 0.0
          && "TEST-LOAD_FACTOR: size=0, yet 0.0 not returned.");

  // min range case:
  map->size = 4;
  assert (hashmap_get_load_factor (map) == HASH_MAP_MIN_LOAD_FACTOR
          && "TEST-LOAD_FACTOR: capacity=16, size=4 yet 0.25 not returned.");

  // max range case:
  map->size = 12;
  assert (hashmap_get_load_factor (map) == HASH_MAP_MAX_LOAD_FACTOR
          && "TEST-LOAD_FACTOR: capacity=16, size=8 yet 0.75 not returned.");

  // clear hash-map:
  hashmap_free(&map);
  assert (map == NULL && "INSERTION-TEST: Failed to free the hash-map.");
}

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if ()
{
  // ensure for hashmap == NULL the counter would be 0.
  int apply_if_counter = hashmap_apply_if (NULL, is_digit, double_value);
  assert (apply_if_counter == FAIL
          && "APPLY-IF-TEST: NULL map was input, yet 0 not returned.");

  // initializing hash map
  hashmap *map = hashmap_alloc (hash_char);
  assert (map != NULL && "APPLY-IF-TEST: Failed to allocate hash map");

  // ensure for key-func == NULL the counter would be 0.
  apply_if_counter = hashmap_apply_if (map, NULL, double_value);
  assert (apply_if_counter == FAIL
          && "APPLY-IF-TEST: NULL key-func was input, yet 0 not returned.");

  // ensure for val-func == NULL the counter would be 0.
  apply_if_counter = hashmap_apply_if (map, is_digit, NULL);
  assert (apply_if_counter == FAIL
          && "APPLY-IF-TEST: NULL val-func was input, yet 0 not returned.");

  // alloc string-int pairs:
  void *pairs[16];
  char *keys[] = {"liverpool", "machine", "forest", "long-term", "bolldiaz",
                  "mutable", "thiago", "dog", "home", "seat", "hi",
                  "brain", "dave", "?*&^", "bond", "story"};
  int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  for (int j = 0; j < 16; ++j)
    {
      pairs[j] = pair_alloc (keys[j], &values[j], str_key_cpy,
                             int_value_cpy, str_key_cmp, int_value_cmp,
                             str_key_free, int_value_free);
      assert (pairs[j] != NULL && "APPLY-IF-TEST: Failed to allocate pair");
    }

  for (int i = 0; i < 16; ++i)
    {
      int action_flag = hashmap_insert (map, pairs[i]);
      assert (action_flag == SUCCESS
              && "APPLY-IF-TEST: Failed to insert pair.");
    }

  // ensure 7 changes will took place:
  assert (hashmap_apply_if (map, longer_then_6, power_value) == 7
          && "APPLY-IF-TEST: ERROR -> Expected to 7 changes.");

  // ensure all the value changes took-place as expected:
  for (int j = 0; j < 7; ++j)
    {
      const_valueT in_map = hashmap_at(map, ((pair *) pairs[j])->key);
      int expected_value = j * j;
      const_valueT exp_val = (void *) (&expected_value);

      // ensure the value change is correct
      assert (int_value_cmp (in_map, exp_val)
              && "APPLY-IF TEST: Value change didn't worked properly.");
    }

  // ensure no values changes was made for unsuitable keys -> shorter then 6:
  for (int j = 7; j < 16; ++j)
    {
      const_valueT in_map = hashmap_at(map, ((pair *) pairs[j])->key);
      int expected_value = j;
      const_valueT exp_val = (void *) (&expected_value);

      // ensure all the value changes is correct
      assert (int_value_cmp (in_map, exp_val)
              && "APPLY-IF TEST: Value changed for an unsuitable key.");
    }

  // clear hash-map
  hashmap_free(&map);
  assert (map == NULL && "INSERTION-TEST: Failed to free the hash-map.");

  // frees pairs:
  for (int j = 0; j < 16; ++j)
    {
      pair_free (&(pairs[j]));
      assert (pairs[j] == NULL && "INSERTION-TEST: Failed to free pair.");
    }
}




