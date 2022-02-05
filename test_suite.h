#ifndef TESTSUITE_H_
#define TESTSUITE_H_

#include "hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * This function checks the hashmap_insert function of the hashmap library.
 * If hashmap_insert fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_insert(void);

/**
 * This function checks the hashmap_at function of the hashmap library.
 * If hashmap_at fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_at(void);

/**
 * This function checks the hashmap_erase function of the hashmap library.
 * If hashmap_erase fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_erase(void);

/**
 * This function checks the hashmap_get_load_factor function of the hashmap library.
 * If hashmap_get_load_factor fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_get_load_factor(void);

/**
 * This function checks the HashMapGetApplyIf function of the hashmap library.
 * If HashMapGetApplyIf fails at some points, the functions exits with exit code 1.
 */
void test_hash_map_apply_if();

int main()
{
  test_hash_map_insert();
  printf("TEST-INSERTION SUCCEED!\n");

  test_hash_map_erase();
  printf("TEST-ERASE SUCCEED!\n");

  test_hash_map_get_load_factor();
  printf("TEST-LOAD-FACTOR SUCCEED!\n");

  test_hash_map_at();
  printf("TEST-HASHMAP-AT SUCCEED!\n");

  test_hash_map_apply_if();
  printf("TEST-APPLY-IF SUCCEED!\n");

}

#endif //TESTSUITE_H_
