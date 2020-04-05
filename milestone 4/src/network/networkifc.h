#pragma once
#include "../utilities/object.h"

class Message;


/**
 * An Abstract network for sending and recieving messages
 */
class NetworkIfc : public Object {
    public:

        /**
         * Adds a node into the system
         */
        virtual void register_node(size_t idx) { }

        /**
         * Returns the index this network is running on
         */
        virtual size_t index() { assert(false); }

        /**
         * Sends mess to the target specified by mess
         */
        virtual void send_message(Message* mess) = 0;

        /**
         * waits to recieve a message
         */ 
        virtual Message* recv_message() = 0;
};