#pragma once

#include "arraytemplate.h"
#include "object.h"
#include "thread.h"
#include "networkifc.h"
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>


/**
 * Node Info class for holding ids and addresses
 */
class NodeInfo : public Object {
    public:
        unsigned id;
        sockaddr_in address;
};



/**
 * Class for networks using ips
 */
class NetworkIP : public NetworkIfc {
    public:
        Array<NodeInfo*> nodes_;
        size_t this_node_;
        int sock_;
        sockaddr_in ip_;

        ~NetworkIP() { close(sock_); }

        size_t index() override { return this_node_; }

        void server_init(unsigned idx, unsigned port) { }

        
}