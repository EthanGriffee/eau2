#include "store.h"
#include "object.h"

class Application : public Object{
    public:
        Store kv;
        size_t node;
 
        Application(size_t idx) {
            node = idx;
        }

        size_t this_node() {
            return node;
        }

        virtual void run_() {}
 
};