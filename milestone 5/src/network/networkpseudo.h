#pragma once

#include "../utilities/arraytemplate.h"
#include "../utilities/object.h"
#include "message.h"
#include "../utilities/thread.h"
#include "networkifc.h"
#include "../utilities/simap.h"
#include "../utilities/wrapper.h"


/**
 * Represents a queue of messages that needs to be locked when accessing
 */
class MessageQueue : public Object {
    public:
        Array<Message*>* messageQueue_;
        Lock lock_;

        MessageQueue() {
            messageQueue_ = new Array<Message*>();
        }

        /**
         * locks, pushes onto @param msg an Array of messsage and unlocks
         */
        void push(Message* msg) {
            lock_.lock();
            messageQueue_->push_back(msg);
            lock_.notify_all();
            lock_.unlock();
        }

        /**
         * Waits for the size of the queue to be > 0 then pops.
         */
        Message* pop() {
            lock_.lock();
            while (messageQueue_->getSize() == 0) lock_.wait();
            Message* result = messageQueue_->pop();
            lock_.unlock();
            return result;
        }
};

/**
 * Map of string to a size_t wrapper
 */
class StringToUnsignedMap : public Object {
    public:
        SIMap* m;
        Lock lock_;

        StringToUnsignedMap() { 
            m = new SIMap();
        }

        ~StringToUnsignedMap() {
            delete m;
        }

        /**
         * locks, adds the key and value to the map, c
         */
        void put(String* key, size_t val) {
            lock_.lock();
            m->set(*key, new Num(val));
            lock_.unlock();
        }

        /**
         * locks, gets the value corresponding to @param key from the map, unlocks
         */
        size_t get(String* key) {
            lock_.lock();
            Num* r = m->get(*key);
            lock_.unlock();
            return r->v;
        }
};

/**
 * Represents a Network where all messages are stored in an array of Messagequeues and each mesage queue 
 * represents a node recieving messages
 */
class NetworkPseudo : public NetworkIfc {
    public:
        Array<MessageQueue*> mqArray_;
        StringToUnsignedMap tidToIndex_;

        NetworkPseudo(size_t num_nodes) {
            for (size_t i = 0; i < num_nodes; i++) {
                mqArray_.add(new MessageQueue());
            }
        }

        /**
         *  Registers a node to the network by placing it in the map
         */
        void register_node(size_t idx) override {
            String*(tid) = Thread::thread_id();
            tidToIndex_.put(tid, idx);
        }

        /**
         *  Sends a message using but putting it into the appropriate message queue
         */
        void send_message(Message* msg) override {
            mqArray_.get(msg->getTarget())->push(msg);
        }

        /**
         * Waits for and recieves a message.
         */
        Message* recv_message() override {
            String* tid = Thread::thread_id();
            size_t i = tidToIndex_.get(tid);
            delete tid;
            return mqArray_.get(i)->pop();
        }
};