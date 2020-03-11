#pragma once

// lang::CwC

#include "object.h"
#include "string.h"
#include "array.h"
#include "helper.h"


class StringToCharMapNode : public Object {
    public:
        String* key;
        char value;

        StringToCharMapNode(String* key, char value) {
            this->key = key;
            this->value = value;
        }

        ~StringToCharMapNode() {
            
        }

        void changeValue(char new_val) {
            value = new_val;
        }

        String* getKey() {
            return key;
        }

        char getValue() {
            return value;
        }

        bool equals(Object* other) {
            return other->equals(key);
        }
};

/**
 * A dictionary of string keys and String values.  All keys and values are owned
 * by the caller, and none of the map's methods will modify them.  Keys which
 * are .equals are equal, i.e. the map will never contain two keys which are
 * extensionally equivalent at the same time.
 */

class StringToCharMap : public Object
{
    public:
        Array** hashmap;
        size_t num_elements;
        const size_t MAX_HASH_LENGTH = 100;

        StringToCharMap() {
            hashmap = new  Array*[MAX_HASH_LENGTH];
            for (size_t x = 0; x < MAX_HASH_LENGTH; x++) {
                hashmap[x] = new Array();
            }
            num_elements = 0;
        }

        StringToCharMap(StringToCharMap& m) {
            hashmap = new  Array*[MAX_HASH_LENGTH];
            for (size_t x = 0; x < MAX_HASH_LENGTH; x++) {
                hashmap[x] = new Array();
            }
            num_elements = m.size();
        }

        virtual ~StringToCharMap(){
            delete_map_nodes_();
            for (size_t x = 0; x < MAX_HASH_LENGTH; x++) {
                delete hashmap[x];
            }
            delete hashmap;
        };

        /**
         * private method that returns the array that 
         * would contain key.
         */
        Array* findArray_(String* key){
            return hashmap[key->hash() % MAX_HASH_LENGTH];
        }

        /**
         * Returns the value which was set for this key.
         * exits  if not in map.
         */
        virtual char get(String *key) {
            Array* arr = findArray_(key);
            int x = arr->indexOf(key);
            if (x == -1) {
                exit(1);
            }
            else {
                return static_cast<StringToCharMapNode*> (arr->get(x))->getValue(); 
            }
        }

        /**
         * Set the value at the given key in this map.
         */
        virtual void set(String *key, char value) {
            Array* arr = findArray_(key);
            int x = arr->indexOf(key);
            if (x == -1) { 
                num_elements += 1;
                arr->add(new StringToCharMapNode(key, value));
            }
            else {
                StringToCharMapNode* m = static_cast<StringToCharMapNode*> (arr->get(x));
                m->changeValue(value);
            }

        }

        /**
         * Remove the value at the given key in this map. No-op if value not in map.
         */
        virtual void remove(String *key) {
            Array* arr = findArray_(key);
            if (arr->indexOf(key) != -1) {
                arr->remove(key);
                num_elements -= 1;
            }
        }

        /**
         * Determine if the given key is in this map.
         */
        virtual bool has(String *key) {
            Array* arr = findArray_(key);
            return arr->indexOf(key) != -1;
        }

        /**
         * private method used to delete map nodes
         */
        void delete_map_nodes_() {
            String** arr_keys = new String*[size()];
            keys(arr_keys);
            for (int i = 0; i < size(); i++) {
                Array* arr = findArray_(arr_keys[i]);
                delete arr->get(arr->indexOf(arr_keys[i]));
            }
        }

        /**
         * Remove all keys from this map.
         */
        virtual void clear() {
            delete_map_nodes_();
            for (int i = 0; i < MAX_HASH_LENGTH ; i++) {
                hashmap[i]->clear();
            }
            num_elements = 0;
        }

        /**
         * Return the number of entries in this map.
         */
        virtual size_t size() {
            return num_elements;
        }

        /**
         * Store keys in the given array. Caller responsible for allocating at least
         * Map::size() elements.
         */
        virtual void keys(String **dest) {
            size_t count = 0;
            for (int i = 0; i < MAX_HASH_LENGTH; i++) {
                Array* arr = hashmap[i];
                for (int x = 0; x < arr->getSize(); x++) {
                    dest[count] = static_cast<StringToCharMapNode*> (arr->array[x])->getKey();
                    count += 1;
                }
            }
        }

        virtual bool equals(String* other) {
            return other->equals(this);
        }

        virtual bool equals(StringToCharMap* other) {
            if (other->size() != num_elements) {
                return false;
            }
            String** other_keys = new String*[other->size()];
            other->keys(other_keys);
            for (int i = 0; i < other->size(); i++) {
                if (!(other->get(other_keys[i]) == (this->get(other_keys[i])))) {
                    delete[] other_keys;
                    return false;
                }
            }
            delete[] other_keys;
            return true;
        }

        size_t hash() {
            size_t count = 0;
            for (int i = 0; i < MAX_HASH_LENGTH; i++) {
                count += hashmap[i]->hash();
            }
            return count;
        }

    
};