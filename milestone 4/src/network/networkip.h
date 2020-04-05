#pragma once

#include "../utilities/arraytemplate.h"
#include "../utilities/object.h"
#include "../utilities/thread.h"
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

        NodeInfo(unsigned id) {
            this->id = id;
        }

        void setAddress(sockaddr_in ip) {
            this->address = ip;
        }
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

        NetworkIP(size_t num_nodes) {
            for (int x = 0; x < num_nodes; x++) {
                nodes_.add(new NodeInfo(x));
            }
        }

        ~NetworkIP() { close(sock_); }

        size_t index() override { return this_node_; }

        void server_init(unsigned idx, unsigned port) { 
            this_node_ = idx;
            nodes_.get(0)->setAddress(ip_);
            
        }

        void client_init(unsigned idx, unsigned port) { 
        }

        void register_node(size_t idx) override {
            if (this_node_ == 0) {
                server_init(idx);
            }
            else {
                client_init(idx);
            }
        }

        
}