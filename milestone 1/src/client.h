#pragma once

#include "array.h"
#include "object.h"
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "thread.h"
#include "map.h"
#include "deserialize_message.h"
#include "directory.h"

/**
 * Represents a node which connects to a server, and recieves directories from the server
 * to connect it to other nodes. 
 **/
class Node : public Object {
    public:
        int server_sock;
        int listening_sock;
        char* ip;
        char* server_ip;
        int port;
        int server_port;
        sockaddr_in listening_addr;
        Directory dir;

         /**
         * Constructs a node with the ip/port input
         **/
        Node(char* ip, int input_port, char* server_ip, int server_port) {
            this->ip = ip;
            this->server_ip = server_ip;

            int opt=1;
            port = input_port;
            assert((listening_sock = socket(AF_INET, SOCK_STREAM, 0)) >= 0);
            assert(setsockopt(listening_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0);
            listening_addr.sin_family = AF_INET;
            listening_addr.sin_port = htons(port);

            // Convert IP addresses from text to binary form
            assert(inet_pton(AF_INET, ip, &listening_addr.sin_addr)>0);
            assert(bind(listening_sock, (struct sockaddr *)&listening_addr, sizeof(listening_addr)) >= 0);
            assert(listen(listening_sock, 10) >= 0);

            struct sockaddr_in serv;
            assert((server_sock = socket(AF_INET, SOCK_STREAM, 0)) >= 0);

            serv.sin_family = AF_INET;
            serv.sin_port = htons(server_port);
            // Convert IP addresses from text to binary form
            assert(inet_pton(AF_INET, server_ip, &serv.sin_addr)>0);
            assert(connect(server_sock, (struct sockaddr *)&serv, sizeof(serv)) >= 0);
            char* buff = new char[2048];
            sprintf(buff, "%s:%d", inet_ntoa(listening_addr.sin_addr), ntohs(listening_addr.sin_port));
            dir.addToBlacklist(new String(buff));
            char* sending = (new Register(listening_addr, 1, 1))->serialize();
            send(server_sock, sending , strlen(sending) , 0 );
            this->server_sock = server_sock;
        }

         /**
         * Sends a message to the node represented by ip
         **/
        virtual void sendMessage(char* ip, const char* message) {
            printf("%s:%d | SENDING - %s\n", inet_ntoa(listening_addr.sin_addr), ntohs(listening_addr.sin_port), message);
            char* mess = (new Status(new String(message), 1, 2))->serialize();
            send(dir.getSockForIp(ip) , mess, strlen(mess) , 0 );
        }

        /**
         * Closes all sockets
         **/
        virtual void closeSockets() {
            char* mess = (new Exit(listening_addr, 1, 1))->serialize();
            send(server_sock , mess, strlen(mess) , 0 );
            IntArray* int_arr = dir.getSockets();
            for (int x =0; x < int_arr->getSize(); x++) {
                close(int_arr->get(x));
            }
            close(server_sock);
            close(listening_sock);
        }

        /**
         * Returns the ips contained in the directory
         **/
        virtual StringArray* getIps() {
            return dir.getIps();
        }

        /**
         * Accepts new connections and reads
         **/
        void acceptAndReadMessages() {
            char buffer[1024];
            fd_set readfds;
            int valread;
            while(true) {
                FD_ZERO(&readfds);

                IntArray* sockets = dir.getSockets(); 
                sockets->add(server_sock);
                int max_nodes = sockets->getSize();
                FD_SET(listening_sock, &readfds); 
                int max_sd = listening_sock;
                for ( int i = 0; i < max_nodes; i++)   
                {   
                    int sd = sockets->get(i);    
                    if(sd > 0)   
                        FD_SET( sd , &readfds);   
                    if(sd > max_sd)   
                        max_sd = sd;   
                }   
                int activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);   
            
                if ((activity < 0) && (errno!=EINTR))   
                {   
                    printf("node exiting");
                    return; 
                }   
                    
                if (FD_ISSET(listening_sock, &readfds))   
                {   
                    int new_socket;
                    sockaddr_in addr;
                    int addrlen = sizeof(addr);
                    assert((new_socket = accept(listening_sock, (struct sockaddr *)&addr, (socklen_t*)&addrlen)) >= 0);
                    char* count = new char[32];
                    sprintf(count, "%s:%d|", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)); 
                    dir.putSocketWithoutIp(new_socket);
                }


                for (int i = 0; i < max_nodes; i++)   
                {   
                    int sd = sockets->get(i); 
                        
                    if (FD_ISSET( sd , &readfds))   
                    {   
                        if ((valread = (read( sd , buffer, 1024))) == 0)   
                        {   
                            sockaddr_in addr;
                            int addrlen = sizeof(addr);
                            getpeername(sd , (struct sockaddr*)&addr, (socklen_t*)&addrlen);   
                            printf("Host disconnected , ip %s , port %d \n", inet_ntoa(addr.sin_addr) , ntohs(addr.sin_port));   
                            char* count = new char[32];
                            sprintf(count, "%s:%d|", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)); 
                            close( sd );   
                            dir.remove(count); 
                        }   
                            
                        //Echo back the message that came in  
                        else 
                        {   
                            //set the string terminating NULL byte on the end  
                            //of the data read  
                            MessageDeserializer md;

                            Message* m = md.deserialize(buffer);
                            switch(m->getKind()) {
                                case(MsgKind::Ack): {
                                    // do nothing as we don't handle ACKS
                                    break;
                                }
                                case(MsgKind::Nack): {
                                    //sends the previous message again.
                                    break;
                                }
                                case(MsgKind::Dir): {
                                    dir.checkAddDir(m->asDir()->getIps());
                                    printf("%s:%d | RECIEVED DIR - %s\n", inet_ntoa(listening_addr.sin_addr), ntohs(listening_addr.sin_port), buffer);
                                    break;
                                }
                                case(MsgKind::Status): {
                                    printf("%s:%i | RECIEVED MESSAGE - %s\n", ip, port, m->asStatus()->getMessage()->c_str());
                                    break;
                                }
                            }
                        }   
                    }   
                } 
            }  
        }
};


/**
 *  Represents the thread of a node that accepts and reads messages
 **/
class NodeListener : public Thread {
    public:
        Node* c;

        NodeListener(Node* c) {
            this->c = c;
        }

        virtual void run() {
            c->acceptAndReadMessages();
        }
};