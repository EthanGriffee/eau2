#pragma once

#include "../utilities/arraytemplate.h"
#include "../utilities/object.h"
#include "../utilities/thread.h"
#include "networkifc.h"
#include "message.h"
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
};


/**
 * Class for networks using ips
 */
class NetworkIP : public NetworkIfc {
    public:
        Array<NodeInfo*> nodes_;
        size_t this_node_;
        size_t num_nodes;
        int sock_;
        unsigned port;
        char* server_ip;
        unsigned server_port;
        sockaddr_in ip_;

        NetworkIP(size_t num_nodes, unsigned port, char* server_ip, unsigned server_port) {
            for (int x = 0; x < num_nodes; x++) {
                nodes_.add(new NodeInfo(x));
            }
            this->num_nodes = num_nodes;
            this->server_port = port;
            this->server_ip = server_ip;
        }

        ~NetworkIP() { close(sock_); }


        /**
         * returns the index of the network ip
         */
        size_t index() override { return this_node_; }

        /**
         * initializes the server node
         */
        void server_init() { 
            nodes_.get(0)->address = ip_;
            for (int x = 1; x < num_nodes; x++) {
                Register* msg = dynamic_cast<Register*> (recv_message());
                nodes_.get(msg->getSender())->id = msg->getSender();
                nodes_.get(msg->getSender())->address.sin_family = AF_INET;
                nodes_.get(msg->getSender())->address.sin_addr = msg->client.sin_addr;
                nodes_.get(msg->getSender())->address.sin_port = htons(msg->client.sin_port);
            }

            Array<int>* ports = new Array<int>();
            Array<String*>* addresses = new Array<String*>();

            for (size_t i = 0; i < num_nodes - 1; i++) {
                ports->add(ntohs(nodes_.get(i + 1)->address.sin_port));
                addresses->add(new String(inet_ntoa(nodes_.get(i + 1)->address.sin_addr)));

            }

            Directory ipd(addresses, ports);
            for (int x  = 1; x < num_nodes; x++) {
                ipd.target_ = x;
                send_message(&ipd);
            }

        }


        /**
         * initializes the server node
         */
        void client_init() { 
            nodes_.get(0)->id = 0;
            nodes_.get(0)->address.sin_family = AF_INET;
            nodes_.get(0)->address.sin_port = htons(server_port);
            if(inet_pton(AF_INET, server_ip, &nodes_.get(0)->address.sin_addr) <= 0)
                assert(false && "invalid server IP address format");
            Register msg(ip_);
            send_message(&msg);
            Directory* ipd = dynamic_cast<Directory*>(recv_message());
            for (int x = 1; x < num_nodes - 1; x++) {
                nodes_.get(x)->id = x;
                nodes_.get(x)->address.sin_family = AF_INET;
                nodes_.get(x)->address.sin_port = htons(ipd->ports_->get(x));
                if(inet_pton(AF_INET, ipd->ips_->get(x)->c_str(), &nodes_.get(x)->address.sin_addr) <= 0)
                    assert(false && "invalid server IP address format");
            }
            delete ipd;
        }

        /**
         * initializes the socket to the port
         */
        void init_sock_(unsigned port) {
            assert((sock_ = socket(AF_INET, SOCK_STREAM, 0)) >= 0);
            int opt = 1;
            assert(setsockopt(sock_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0);
            ip_.sin_family = AF_INET;
            if (this_node_ == 0) {
                assert(inet_pton(AF_INET, server_ip, &ip_.sin_addr)>0);
            }
            else {
                ip_.sin_addr.s_addr = INADDR_ANY;
            }
            ip_.sin_port = htons(port);
            assert(bind(sock_, (sockaddr*)&ip_, sizeof(ip_)) >= 0);
            assert(listen(sock_, 100) >= 0);
        }

        /**
         * registers the node, if its a client, initializes it as a client, or starts up the server
         */
        void register_node(size_t idx) override {
            this->this_node_ = idx;
            if (this_node_ == 0) {
                init_sock_(server_port);
                server_init();
            }
            else {
                init_sock_(port);
                client_init();
            }
        }

        /**
         * sends the message over the network to the target specified
         */
        void send_message(Message* msg) override {
            msg->sender_ = this->this_node_;
            NodeInfo* tgt = nodes_.get(msg->getTarget());
            int connection = socket(AF_INET, SOCK_STREAM, 0);
            assert( connection >=0 );
            if (connect (connection, (sockaddr*)&tgt->address, sizeof(tgt->address))< 0) assert("Unable to connect to remote node");
            char* buf = msg->serialize();
            size_t size = strlen(buf);
            send(connection, &size, sizeof(size_t), 0);
            send(connection, buf, size, 0);
        }


        /**
         * waits and recieves a message
         */
        Message* recv_message() override {
            sockaddr_in sender;
            socklen_t addrlen = sizeof(sender);
            int req = accept(sock_, (sockaddr*)&sender, &addrlen);
            size_t size = 0;
            if (read(req, &size, sizeof(size_t)) == 0) assert(false);
            char* buf = new char[size];
            int rd = 0;
            while (rd != size) rd += read(req, buf + rd, size - rd);
            return Message::deserialize(buf);
        }

        
};