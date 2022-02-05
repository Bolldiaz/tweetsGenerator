#include "test_suite.h"

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