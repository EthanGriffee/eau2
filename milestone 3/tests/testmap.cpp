// lang::CwC

#include "../src/map.h"
#include "../src/object.h"
#include "../src/string.h"
#include "../src/wrapper.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void test_get() {
  Map *theMap = new Map();

  String *key = new String("Hello");
  String *value = new String("World");

  theMap->set(key, value);

  Object *str = theMap->get(key);

  assert(str == value);

  String *wrongKey = new String("Goodbye");
  Object *result = theMap->get(wrongKey);
  assert(result == nullptr);
  // str should not change on an invalid lookup
  assert(str == value);
}

void test_remove() {
  Map *theMap = new Map();

  String *key = new String("Hello");
  String *value = new String("World");

  String *key2 = new String("Design");
  String *value2 = new String("Recipe");

  theMap->set(key, value);
  theMap->set(key2, value2);

  theMap->remove(key);

  Object *str = theMap->get(key);
  Object *str2 = theMap->get(key2);

  // successfully removes one entry
  assert(str == nullptr);
  assert(str2 == value2);

  String *wrongKey = new String("Goodbye");
  theMap->remove(wrongKey); // not in map, so no-op (doesn't explode)
}

void test_has() {
  Map *theMap = new Map();

  String *key = new String("Hello");
  String *value = new String("World");

  theMap->set(key, value);

  assert(theMap->has(key));

  String *wrongKey = new String("Goodbye");
  assert(!theMap->has(wrongKey));
}

void test_clear() {
  Map *theMap = new Map();

  String *key = new String("Hello");
  String *value = new String("World");

  String *key2 = new String("Design");
  String *value2 = new String("Recipe");

  theMap->set(key, value);
  theMap->set(key2, value2);

  theMap->clear();

  Object *str = theMap->get(key);
  Object *str2 = theMap->get(key2);

  // successfully clears all entries
  assert(str == nullptr);
  assert(str2 == nullptr);
}

void test_size() {
  Map *theMap = new Map();

  assert(theMap->size() == 0);

  String *key = new String("Hello");
  String *value = new String("World");

  theMap->set(key, value);
  assert(theMap->size() == 1);

  theMap->remove(key);
  assert(theMap->size() == 0);
}

// make sure setting a key which was already in the map behaves properly
void test_overwrite() {
  Map *theMap = new Map();

  String *key = new String("Hello");
  String *value = new String("World");

  theMap->set(key, value);

  Object *str = theMap->get(key);
  assert(str == value);

  String *value2 = new String("Darkness");

  theMap->set(key, value2);

  Object *str2 = theMap->get(key);

  // the map successfully overwrites values
  assert(str2 == value2);
  // the map does not delete the old value
  assert(value);
}

void test_equals_and_hash() {
  Map *theMap1 = new Map();
  Map *theMap2 = new Map();

  assert(theMap1->equals(theMap2) && theMap2->equals(theMap1));

  String *key1 = new String("Hello");
  String *key2 = new String("Hello");

  String *value1 = new String("World");
  String *value2 = new String("World");

  String *key3 = new String("Design");
  String *key4 = new String("Design");

  String *value3 = new String("Recipe");
  String *value4 = new String("Recipe");

  theMap1->set(key1, value1);
  theMap2->set(key2, value2);

  theMap1->set(key3, value3);
  theMap2->set(key4, value4);

  // equal maps are equal
  assert(theMap1->equals(theMap2) && theMap2->equals(theMap1));
  // equal maps have equal hashes
  assert(theMap1->hash() == theMap2->hash());

  theMap1->remove(key1);
  theMap2->remove(key4);
  // unequal maps are unequal
  assert(!theMap1->equals(theMap2) && !theMap2->equals(theMap1));
}

// make sure keys that are .equals equivalent are treated the same,
// for has, get, set, remove
void test_keys_extensional() {
  Map *theMap = new Map();

  String *key = new String("Hello");
  String *value = new String("World");

  String *key2 = new String("Design");
  String *value2 = new String("Recipe");

  theMap->set(key, value);
  theMap->set(key2, value2);

  String *sameKey = new String("Hello");
  String *value3 = new String("Darkness");

  // testing has
  // make sure the map has both keys
  assert(theMap->has(key));
  assert(theMap->has(sameKey));

  // set the value at sameKey, which is extensionally equivalent to key
  theMap->set(sameKey, value3);

  // make sure the map STILL has both keys
  assert(theMap->has(key));
  assert(theMap->has(sameKey));

  Object *str = theMap->get(key);
  Object *str2 = theMap->get(sameKey);

  // testing get and set
  // the values corresponding to the .equals keys are both reference
  // equivalent strings
  assert(str == value3);
  assert(str == str2);

  String *sameKey2 = new String("Design");
  theMap->remove(sameKey2);

  Object *str3 = theMap->get(key2);
  Object *str4 = theMap->get(sameKey2);

  // testing remove
  // both .equals keys are no longer in the map
  assert(str3 == nullptr);
  assert(str4 == nullptr);
}

int main(int argc, char **argv) {
  test_get();
  printf("OK: GET\n");
  test_remove();
  printf("OK: REMOVE\n");
  test_has();
  printf("OK: HAS\n");
  test_size();
  printf("OK: SIZE\n");
  test_overwrite();
  printf("OK: OVERWRITE\n");
  test_equals_and_hash();
  printf("OK: EQHASH\n");
  test_keys_extensional();
  printf("OK: EXT\n");
  return 0;
}