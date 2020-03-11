#pragma once 

// lang::CwC

#include "object.h"
#include "string.h"
#include "array.h"


class Map_Node : public Object {
    public:
        String* key;
        Object* value;

        Map_Node(String* key, Object* value) {
            this->key = key;
            this->value = value;
        }

        ~Map_Node() {
            
        }

        void changeValue(Object* new_val) {
            this->value = new_val;
        }

        String* getKey() {
            return key;
        }

        Object* getValue() {
            return value;
        }

        bool equals(Object* other) {
            return key->equals(other);
        }

        size_t hash() {
            return key->hash() + value->hash();
        }
};

/**
 * A dictionary of string keys and object values.  All keys and values are owned
 * by the caller, and none of the map's methods will modify them.  Keys which
 * are .equals are equal, i.e. the map will never contain two keys which are
 * extensionally equivalent at the same time.
 */

class Map : public Object
{
    public:
        Array** hashmap;
        size_t num_elements;
        const size_t MAX_HASH_LENGTH = 100;

        Map() {
            hashmap = new  Array*[MAX_HASH_LENGTH];
            for (size_t x = 0; x < MAX_HASH_LENGTH; x++) {
                hashmap[x] = new Array();
            }
            num_elements = 0;
        }

        virtual ~Map(){
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
        Array* findArray_(Object* key){
            return hashmap[key->hash() % MAX_HASH_LENGTH];
        }

        /**
         * Returns the value which was set for this key.
         * Returns nullptr if not in map.
         */
        virtual Object *get(Object *key) {
            Array* arr = findArray_(key);
            int x = arr->indexOf(key);
            if (x == -1) {
                return nullptr;
            }
            else {
                return static_cast<Map_Node*> (arr->get(x))->getValue(); 
            }
        }

        /**
         * Set the value at the given key in this map.
         */
        virtual void set(Object *key, Object *value) {
            String* str_key = dynamic_cast<String*> (key);
            if (str_key) {
                Array* arr = findArray_(str_key);
                int x = arr->indexOf(str_key);
                if (x == -1) { 
                    num_elements += 1;
                    arr->add(new Map_Node(str_key, value));
                }
                else {
                    Map_Node* m = static_cast<Map_Node*> (arr->get(x));
                    m->changeValue(value);
                }
            }
        }

        /**
         * Remove the value at the given key in this map. No-op if value not in map.
         */
        virtual void remove(Object *key) {
            String* str_key = dynamic_cast<String*> (key);
            if (str_key) {
                Array* arr = findArray_(str_key);
                if (arr->indexOf(str_key) != -1) {
                    arr->remove(str_key);
                    num_elements -= 1;
                }
            }
        }

        /**
         * Determine if the given key is in this map.
         */
        virtual bool has(Object *key) {
            String* str_key = dynamic_cast<String*> (key);
            if (!str_key) {
                return false;
            }
            Array* arr = findArray_(str_key);
            return arr->indexOf(str_key) != -1;
        }

        /**
         * private method used to delete map nodes
         */
        void delete_map_nodes_() {
            String** arr_keys = keys();
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
        virtual String** keys() {
            String** dest = new String*[num_elements];
            size_t count = 0;
            for (int i = 0; i < MAX_HASH_LENGTH; i++) {
                Array* arr = hashmap[i];
                for (int x = 0; x < arr->getSize(); x++) {
                    dest[count] = static_cast<Map_Node*> (arr->get(x))->getKey();
                    count += 1;
                }
            }
            return dest;
        }

        virtual bool equals(Object* obj) {
            Map* other = dynamic_cast<Map*> (obj);
            if (!other || other->size() != num_elements) {
                return false;
            }
            String** other_keys = other->keys();
            for (int i = 0; i < other->size(); i++) {
                if (!other->get(other_keys[i])->equals(this->get(other_keys[i]))) {
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