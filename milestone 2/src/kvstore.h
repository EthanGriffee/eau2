#pragma once

#include "object.h"
#include "dataframe.h"
#include "string.h"
#include "dataframe.h"


class Key : public Object {
    public:
        char* name;
        size_t node;

        Key(char* name, size_t node) {
            this->name = name;
            this->node = node;
        }

        ~Key() {
            
        }

        void changeNode(size_t node) {
            this->node = node;
        }

        char* getKey() {
            return name;
        }

        size_t getNode() {
            return node;
        }

        bool equals(Object* other) {
            Key* other_key = dynamic_cast<Key*>(other);
            if (other_key) {
                return (strcmp(name, other_key->getKey()) == 0);
            }
            return false;
        }
};


class KVStore : public Object {
    public:
        size_t node;
        Array<Key*> keys;
        Array<char*> vals;

        KVStore() {}

        void put(Key* key, char* value) {
            keys.add(key);
            vals.add(value);
        }
    
        char*  get(Key key) {
            return vals.get(keys.indexOf(&key));
        }

        char* waitAndGet(Key key) {

        }

        ~KVStore() {
            for (int x = 0; x < keys.getSize(); x++) {
                delete[] vals.get(x);
            }
        }

};

class KDStore {
    public:
        KVStore kv;

        KDStore() {}

        void put(Key* key, DataFrame* value) {
            kv.put(key, value->serialize());
        }

        DataFrame* get(Key key) {
            return DataFrame::deserialize(kv.get(key));
        }

        DataFrame* waitAndGet(Key key) {
            return DataFrame::deserialize(kv.waitAndGet(key));
        }

};

DataFrame* DataFrame::fromArray(Key* key, KDStore* kv, size_t size, Array<double>* vals) {
    Schema s("D");
    DataFrame* n = new DataFrame(s);
    Row r(s);
    for (int x = 0; x < vals->getSize(); x++) {
        r.set_double(0, vals->get(x));
        n->add_row(r);
    }
    kv->put(key, n);
    return n;
}