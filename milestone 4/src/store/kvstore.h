#pragma once

#include "../utilities/object.h"
#include "../dataframe/dataframe.h"
#include "../utilities/string.h"
#include "../network/networkifc.h"
#include "../utilities/thread.h"
#include "../network/message.h"
#include "key.h"


/**
 * A KVStore that holds what node it is, a map of keys to values,
 * a Array of WaitAndGets which are requests that have not been fufilled
 * a Netowrk used for sending messages, and the current recieved message
 */
class KVStore : public Object, public Thread {
    public:
        size_t node;
        Array<Key*> keys;
        Array<char*> vals;
        Array<WaitAndGet*> requests;
        NetworkIfc* net;
        Lock lock_;
        KeyValue* recieved_message;

        KVStore(size_t node) {
            this->node = node;
        }

        KVStore(NetworkIfc& net, size_t node) {
            this->net = &net;
            this->node = node;
            recieved_message = nullptr;
        }

        /**
         * Responds to a wait and get, assumes that the map contains the key and val
         */
        void respondToWaitAndGet(WaitAndGet* msg) {
            Key* searching_key = msg->getKey();
            char* val = this->get(*searching_key);
            Message* mess = new KeyValue(searching_key, val, node, msg->getSender());
            net->send_message(mess);
        }

        /**
         * Thread for reasing and responding to messages
         */
        void run() {
            net->register_node(node);
            while (true) {
                Message* mess = net->recv_message();
                switch(mess->getKind()) {
                    case(MsgKind::WaitAndGet): {
                        WaitAndGet* wgm = mess->asWaitAndGet();
                        Key* searching_key = wgm->getKey();
                        lock_.lock();
                        if (this->contains(searching_key)) {
                            respondToWaitAndGet(wgm);
                        }
                        else {
                            requests.add(wgm);
                        }
                        lock_.unlock();
                        break;
                    }
                    case(MsgKind::KeyValue): {  
                        if (mess->getTarget() == this->node) {
                            lock_.lock();
                            recieved_message = mess->asKeyValue();
                            lock_.notify_all();
                            lock_.unlock();
                        }
                        break;
                    }
                    case(MsgKind::Put): {
                        Put* pm = mess->asPut();
                        this->put(pm->getKey(), pm->getVal());
                    }
                }
            }
        }

        /**
         * Puts a key into the map, if the key expects to be in a different node, sends the key ot that node
         */
        void put(Key* key, char* value) {
            if (key->getNode() != node) {
                Put* k = new Put(key, value, node, key->getNode());
                net->send_message(k); 
            }
            else {
                lock_.lock();
                keys.add(key);
                vals.add(value);
                for (int x = 0; x < requests.getSize(); x++) {
                    if(requests.get(x)->getKey()->equals(key)) {
                        respondToWaitAndGet(requests.get(x));
                        requests.remove(requests.get(x));
                    }
                }
                lock_.unlock();
            }
        }

        /**
         * Determines if this node contains key k
         */
        bool contains(Key* k) {
            return keys.indexOf(k) != -1;
        }
    
        /**
         * Returns the value associated with the key input
         */
        char*  get(Key key) {
            return vals.get(keys.indexOf(&key));
        }

        /**
         * Sends a message requesting key k and waits for another KVStore to return a KeyValue message
         */
        virtual char* waitAndGet(Key key) {
            if (this->contains(&key)) {
                return this->get(key);
            }
            else {
                requestKey(&key);
                lock_.lock();
                while(!recieved_message) lock_.wait();
                char* mess = recieved_message->getVal();
                lock_.unlock();
                recieved_message = nullptr;
                return mess;
            }
        }

        /**
         * Sends a WaitAndGet to the node in the key.
         */
        void requestKey(Key* k) {
            Message* mess = new WaitAndGet(k, node, k->getNode());
            net->send_message(mess);
        }

        ~KVStore() {
            for (int x = 0; x < keys.getSize(); x++) {
                delete[] vals.get(x);
            }
        }

};

/**
 * Wrapper around a KKVStore with values as serialized dataframes.
 */
class KDStore : public Object {
    public:
        KVStore kv;

        KDStore(size_t node) : kv(node) {}

        KDStore(NetworkIfc & net, size_t node) : kv(net, node){
            kv.start();
        }

        ~KDStore() {
        }

        /**
         * Serializes the dataframe and puts it into the KVStore.
         */
        void put(Key* key, DataFrame* value) {
            kv.put(key, value->serialize());
        }

        /**
         * Gets the char* corresponding to the key in the kv store and deserializes it into a dataframe.
         */
        DataFrame* get(Key key) {
            return DataFrame::deserialize(kv.get(key));
        }


        /**
         * calls wait and get on the kvstore, and deserializes it into a dataframe
         */
        DataFrame* waitAndGet(Key key) {
            return DataFrame::deserialize(kv.waitAndGet(key));
        }

};


/**
 * Returns a single double column dataframe with rows specified by the arraylist vals
 */
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

/**
 * Returns a single double column single row dataframe using VAL
 */
DataFrame* DataFrame::fromScalar(Key* key, KDStore* kv, double val) {
    Schema s("D");
    DataFrame* n = new DataFrame(s);
    Row r(s);
    r.set_double(0, val);
    n->add_row(r);
    kv->put(key, n);
    return n;
}