#include "object.h"
#include "dataframe.h"
#include "string.h"

class KVStore {
    public:
        KVStore() {}

        void put(Key key, Value value) {


        }
    
        DataFrame*  get(Key key) {

        }

        DataFrame* waitAndGet(Key key) {

        }

};


class Key : public Object {
    public:
        String* name;
        size_t node;

        Key(String* name, size_t node) {
            this->name = name;
            this->node = node;
        }

        ~Key() {
            
        }

        void changeNode(size_t node) {
            this->node = node;
        }

        String* getKey() {
            return name;
        }

        size_t getNode() {
            return node;
        }
};


class Value {
    public:
        char* data;
};