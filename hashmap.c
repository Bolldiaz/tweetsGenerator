#include <stdlib.h>
#include "hashmap.h"

/**
 * Allocates dynamically new hash map element.
 * @param func a function which "hashes" keys.
 * @return pointer to dynamically allocated hashmap.
 * @if_fail return NULL.
 */
hashmap *hashmap_alloc (hash_func func)
{
  if (func == NULL)
    return NULL;

  hashmap *hm = malloc (sizeof (*hm));
  if (hm == NULL)
    return NULL;

  hm->buckets = malloc (sizeof (void *) * HASH_MAP_INITIAL_CAP);
  if (hm->buckets == NULL)
    {
      free (hm);
      return NULL;
    }

  //
  hm->size = 0;
  hm->capacity = HASH_MAP_INITIAL_CAP;
  hm->hash_func = func;
  for (size_t i=0; i < hm->capacity; i++)
    hm->buckets[i] = NULL;
  return hm;
}

/**
 * Frees a hash map and the elements the hash map itself allocated.
 * @param p_hash_map pointer to dynamically allocated pointer to hash_map.
 */
void hashmap_free (hashmap **p_hash_map)
{
  if (p_hash_map != NULL && *p_hash_map != NULL)
    {
      for (size_t i = 0; i < (*p_hash_map)->capacity; i++)
        // free the vector which held in each bucket
        vector_free (&(*p_hash_map)->buckets[i]);

      // free the buckets array, and the hash map itself
      free ((*p_hash_map)->buckets);
      free (*p_hash_map);
      *p_hash_map = NULL;
    }
}

/**
 * The function check if the given key already inserted to the hash map.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return pointer to the pair associated with key if exists, NULL
 * otherwise (a pointer to the pair itself, not a copy of it).
 */
pair *key_in_hashmap (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL)
    return NULL;

  size_t ind = (hash_map->hash_func (key)) & (hash_map->capacity -1);

  if (hash_map->buckets[ind] == NULL)
    return NULL;

  for (size_t i = 0; i < hash_map->buckets[ind]->size; i++)
    {
      pair *cur_pair = hash_map->buckets[ind]->data[i];
      if (cur_pair->key_cmp (cur_pair->key, key))
        return cur_pair;
    }
  return NULL;

}

/**
 * Inserts a new in_pair to the buckets-array.
 * @param buckets an array of pointers to vectors
 * @param in_pair a in_pair the buckets-array would contain
 * @param index the index of the vector which will contain the pair.
 * @return 1 if the process has succeeded, 0 else
 */
int buckets_insert (vector **buckets, const pair *in_pair, size_t index)
{
  if (buckets == NULL || in_pair == NULL)
    return 0;

  // if its first key to get inserted into the ind bucket
  if (buckets[index] == NULL)
    {
      buckets[index] = vector_alloc (pair_copy, pair_cmp, pair_free);
    }
  return vector_push_back (buckets[index], in_pair);
}

/**
 * rehashing the map, and resizing it's capacity, by allocing a new buckets
 * list, and hashing all the elements form the old one to it.
 * if a problem occurred in the process, no changes to be made.
 * @param hash_map the hash map to be resized.
 * @param new_capacity the new capacity of the buckets array
 * @return 1 if the process has succeeded, 0 else
 */
int hashmap_resize (hashmap *hash_map, size_t new_capacity)
{
  size_t old_capacity = hash_map->capacity;
  vector **old = hash_map->buckets;
  vector **new = malloc (new_capacity * sizeof (void *));
  if (new == NULL)
    return 0;

  // initialize the new-buckets:
  for (size_t i = 0; i < new_capacity; ++i)
    new[i] = NULL;

  hash_map->capacity = new_capacity;

  // for each vector in the old buckets list, all its elements will got
  // rehashed into the *new* buckets list
  for (size_t i = 0; i < old_capacity; i++) // scan vectors

    if (old[i] != NULL)
      for (size_t j = 0; j < old[i]->size; j++) // scan pairs
        {
          pair *cur_pair = (pair *) (old[i]->data[j]);
          size_t ind = hash_map->hash_func (cur_pair->key) & (new_capacity-1);

          // ensure the insertion succeeded, if not - undo the hole process
          if (!buckets_insert (new, cur_pair, ind))
            {
              for (size_t k = 0; k < new_capacity; k++)
                {
                  vector_free (&new[k]);
                }
              free (new);
              hash_map->capacity = old_capacity;
              return 0;
            }
        }

  // rehashing worked successfully, free the old list & update the hash-map:
  for (size_t i = 0; i < old_capacity; i++)
    {
      vector_free (&old[i]);
    }
  free (old);
  hash_map->buckets = new;
  return 1;
}

/**
 * Inserts a new in_pair to the hash map.
 * The function inserts *new*, *copied*, *dynamically allocated* in_pair,
 * NOT the in_pair it receives as a parameter.
 * @param hash_map the hash map to be inserted with new element.
 * @param in_pair a in_pair the hash map would contain.
 * @return returns 1 for successful insertion, 0 otherwise.
 */
int hashmap_insert (hashmap *hash_map, const pair *in_pair)
{
  if (hash_map == NULL || in_pair == NULL)
    return 0;

  // ensure the key not in hash map:
  if (key_in_hashmap (hash_map, in_pair->key))
    return 0;

  size_t ind = (hash_map->hash_func (in_pair->key)) & (hash_map->capacity -1);

  // make pair insertion to the buckets array, if failed - return 0
  if (!buckets_insert (hash_map->buckets, in_pair, ind))
    return 0;

  hash_map->size++;
  // check if the load factor out of the max range, resize and rehash the map
  double load_factor = hashmap_get_load_factor (hash_map);
  if (load_factor > HASH_MAP_MAX_LOAD_FACTOR)
    return hashmap_resize (hash_map, hash_map->capacity *
                                     HASH_MAP_GROWTH_FACTOR);
  return 1;

}

/**
 * The function returns the value associated with the given key.
 * @param hash_map a hash map.
 * @param key the key to be checked.
 * @return the value associated with key if exists, NULL otherwise (the
 * value itself, not a copy of it).
 */
valueT hashmap_at (const hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL)
    return NULL;

  pair *assoc_pair = key_in_hashmap (hash_map, key);
  // check if key in hash map
  if (assoc_pair == NULL)
    return NULL;

  return assoc_pair->value;
}

/**
 * The function erases the pair associated with key.
 * @param hash_map a hash map.
 * @param key a key of the pair to be erased.
 * @return 1 if the erasing was done successfully, 0 otherwise. (if key not
 * in map, considered fail).
 */
int hashmap_erase (hashmap *hash_map, const_keyT key)
{
  if (hash_map == NULL)
    return 0;

  pair *assoc_pair = key_in_hashmap (hash_map, key);

  // make sure the key in hash map:
  if (assoc_pair == NULL)
    return 0;

  size_t buc_ind = (hash_map->hash_func (key)) & (hash_map->capacity -1);
  vector *cur_vec = hash_map->buckets[buc_ind];

  int elem_ind = vector_find (cur_vec, assoc_pair);
  if (elem_ind == -1)
    return 0;

  // erase the pair from the hash-map
  if (!vector_erase (cur_vec, elem_ind))
    return 0;

  hash_map->size--;
  // check if the load factor out of the min range, resize the map
  double load_factor = hashmap_get_load_factor (hash_map);
  if (load_factor < HASH_MAP_MIN_LOAD_FACTOR && hash_map->capacity > 1)
    return hashmap_resize (hash_map, hash_map->capacity /
                                     HASH_MAP_GROWTH_FACTOR);
  return 1;
}

/**
 * This function returns the load factor of the hash map.
 * @param hash_map a hash map.
 * @return the hash map's load factor, -1 if the function failed.
 */
double hashmap_get_load_factor (const hashmap *hash_map)
{
  if (hash_map == NULL || hash_map->capacity == 0)
    return -1;

  return (double) hash_map->size / (double) hash_map->capacity;
}

/**
 * This function receives a hashmap and 2 functions, the first checks a
 * condition on the keys, and the seconds apply some modification on the
 * values. The function should apply the modification only on the values
 * that are associated with keys that meet the condition.
 *
 * Example: if the hashmap maps char->int, keyT_func checks if the char is a
 * capital letter (A-Z), and val_t_func multiples the number by 2,
 * hashmap_apply_if will change the map:
 * {('C',2),('#',3),('X',5)}, to: {('C',4),('#',3),('X',10)},
 * and the return value will be 2.
 * @param hash_map a hashmap
 * @param keyT_func a function that checks a condition on keyT
 *        and return 1 if true, 0 else
 * @param valT_func a function that modifies valueT, in-place
 * @return number of changed values
 */
int hashmap_apply_if (const hashmap *hash_map, keyT_func keyT_func,
                      valueT_func valT_func) //const
{
  int counter = 0;
  if (hash_map == NULL || keyT_func == NULL || valT_func == NULL)
    return counter;

  // scan vectors
  for (size_t vec_idx = 0; vec_idx < hash_map->capacity; vec_idx++)
    {
      vector *cur_vec = hash_map->buckets[vec_idx];
      if (cur_vec != NULL)
        // scan pairs:
        for (size_t pair_idx = 0; pair_idx < cur_vec->size; pair_idx++)
          {
            pair *cur_pair = (pair *) (cur_vec->data[pair_idx]);
            // value change:
            if (keyT_func (cur_pair->key))
              {
                valT_func (cur_pair->value);
                counter++;
              }
          }
    }
  return counter;
}

