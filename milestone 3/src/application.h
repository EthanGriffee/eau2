#pragma once

#include "kvstore.h"
#include "object.h"
#include "networkifc.h"


/**
 * Represents an eau2 application. It contains a KDStore which keeps track of keys and dataframes, and
 * runs across multiple nodes.
 */
class Application : public Object{
    public:
        KDStore kv;
        size_t node;
 
        Application(size_t idx) : kv(node) {
            node = idx;
        }

        Application(size_t idx, NetworkIfc &net) : kv(net, idx){
            node = idx;
        }

        /**
         * Represents the node this application is being run on
         */
        size_t this_node() {
            return node;
        }

        /**
         * Starts the application on the node specified.
         */
        virtual void run_() {}
 
};