#pragma once

#include "array.h"
#include "object.h"
#include "string.h"
#include "map.h"
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * Class that keeps track of ips and the sockets opens connecting to
 * these ips.
 * */
class Directory : Object {
    public: 
        // Maps a string to a Int Wrapper containing a socket
        Map* ip_to_sock;
        IntArray* sockets_without_ips;
        StringArray* black_list;
        char* this_ip;
        int this_port;


        /**
         * Creates a empty directory
         **/
        Directory() {
            ip_to_sock = new Map();
            sockets_without_ips = new IntArray();
            black_list = new StringArray();
        }


        /**
         * Creates a directory with the ip entered as blacklisted
         **/
        Directory(char* ip) {
            ip_to_sock = new Map();
            sockets_without_ips = new IntArray();
            black_list = new StringArray();
            black_list->add(new String(ip));
        }


        /**
         * Creates a directory with the stringarray entered
         **/
        Directory(StringArray* dir) {
            ip_to_sock = new Map();
            sockets_without_ips = new IntArray();
            black_list = new StringArray();

            for (int x = dir->getSize() - 1; x >= 0; x--) {
                ip_to_sock->set(dir->get(x), new IntObj(-1));
            }
        }


        /**
         * Adds the ips to the directory
         **/
        void checkAddDir(StringArray* ips) {
            for(int x = 0; x < ips->getSize(); x++) {
                checkAddDir(ips->get(x));
            }
        }

        /**
         * Adds a socket that we do not know the IP for.
         **/
        void putSocketWithoutIp(int sock) {
            sockets_without_ips->add(sock);
        }

        /**
         * Adds the string tot he directory with a socket equal to -1.
         **/
        void checkAddDir(String* ip) {
            checkAddDir(ip, -1);
        }

        /**
         * Checks if the string ip is in the map, and if it is not in the blacklist.
         * If both of these are true the ip, socket is added to the black_list.
         **/
        void checkAddDir(String* ip, int sock) {
            if ((!(ip_to_sock->has(ip))) and (black_list->indexOf(ip) == -1))
            {
                ip_to_sock->set(ip, new IntObj(sock));
                int index = sockets_without_ips->indexOf(sock);
                if (index != -1) {
                    sockets_without_ips->remove(index);
                }
            }
        }

        /**
         *  Adds ip to the blacklist.
         **/
        void addToBlacklist(String* ip) {
            black_list->add(ip);
        }

        /**
         *  Returns the number of IPs known
         **/
        size_t getSize() {
            return ip_to_sock->size();
        }


        /**
         *  Removes the ip from the map
         **/
        void remove(char* ip) {
            return ip_to_sock->remove(new String(ip));
        }

        /**
         *  Removes the Sockets in the map, and the sockets where we do not know the ip
         **/
        IntArray* getSockets() {
            IntArray* returning = new IntArray();
            String** keys = ip_to_sock->keys();
            for (int x = 0; x < getSize(); x++) {
                IntObj* iobj = dynamic_cast <IntObj*> (ip_to_sock->get(keys[x]));
                if (iobj->getInt() != -1) {
                    returning->add(iobj->getInt());
                }
            }
            returning->addAll(sockets_without_ips);
            return returning;
        }

        /**
         *  Removes the IPS stored in the map.
         **/
        StringArray* getIps() {
            StringArray* returning = new StringArray();
            String** keys = ip_to_sock->keys();
            for (int x = 0; x < getSize(); x++) {
                returning->add(keys[x]);
            }
            return returning;
        }


         /**
         *  Returns the sock for the given ip
         **/
        int getSockForIp(char* ip) {
            IntObj* intobj =  dynamic_cast<IntObj*> (ip_to_sock->get(new String(ip)));
            int n =  intobj->getInt();
            if (n == -1) {
                char* connecting_ip = strtok(ip, ":");
                int connecting_port = atoi(strtok(NULL, ":"));
                
                struct sockaddr_in connecting;
                assert((n = socket(AF_INET, SOCK_STREAM, 0)) >= 0);

                connecting.sin_family = AF_INET;
                connecting.sin_port = htons(connecting_port);
                // Convert IP addresses from text to binary form
                assert(inet_pton(AF_INET, connecting_ip, &connecting.sin_addr)>0);
                assert(connect(n, (struct sockaddr *)&connecting, sizeof(connecting)) >= 0);
                ip_to_sock->set(new String(ip), new IntObj(n));
            }
            return n;
        }
};