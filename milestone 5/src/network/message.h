#pragma once

#include "../utilities/string.h"
#include "../utilities/object.h"
#include "../utilities/helper.h"
#include "../store/key.h"
#include "../utilities/arraytemplate.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


class WaitAndGet;
class KeyValue;
class Put;
class Register;
class Directory;

/**
 * An enumeration of the current possible message kidns
 */
enum class MsgKind { WaitAndGet, KeyValue, Put, Register, Directory };

 
/**
 *  Represents a message with a target and a sender, and type
 */
class Message : public Object {
    public: 
        MsgKind kind_;  // the message kind
        size_t sender_; // the index of the sender node
        size_t target_; // the index of the receiver node

        Message() {}

        Message(size_t sender_, size_t target_) {
            this->sender_ = sender_;
            this->target_ = target_;
        }

        /**
         * returns the sender
         */
        size_t getSender() {
            return sender_;
        }

        /**
         * returns the target
         */
        size_t getTarget() {
            return target_;
        }

        /**
         * returns the kid of message
         */
        MsgKind getKind() {
            return kind_;
        }

        static Message* deserialize(char* mess);

        /**
         *  returns the msg kind as a char*
         */
        char* msgKindAsChar() {
            switch(kind_) {
                case(MsgKind::WaitAndGet): {
                    return "WaitAndGet";
                }
                case(MsgKind::KeyValue): {
                    return "KeyValue";
                }
                case(MsgKind::Put): {
                    return "Put";
                }
                case(MsgKind::Register): {
                    return "Register";
                }
                case(MsgKind::Directory): {
                    return "Directory";
                }
            }
            assert(false);
        }

        /**
         * returns a char* representation of the Object.
         * */
        virtual char* serialize() {
            StrBuff strbuff;
            strbuff = strbuff.c("{MESSAGE|kind_=");
            strbuff = strbuff.c(msgKindAsChar());
            strbuff = strbuff.c("|sender_=");
            strbuff = strbuff.c(sender_);
            strbuff = strbuff.c("|target_=");
            strbuff = strbuff.c(target_);
            strbuff = strbuff.c("|");
            strbuff = strbuff.c(this->serializeAditionalFields_());
            return strbuff.get_char();;
        }

        /**
         * helper function for serialization
         */
        virtual char* serializeAditionalFields_() {
            return "}";
        }

        /**
         * determines if two messages are equal
         */
        virtual bool equals(Object* other) {
            Message* m = dynamic_cast<Message*> (other);
            if(m) {
                return this->kind_ == m->getKind() && this->sender_ == m->getSender() && this->target_ == m->getTarget();
            }
            else {
                return false;
            }
        }

        /**
         * Returns this message or a nullptr if it is not a WaitAndGet message
         */
        virtual WaitAndGet* asWaitAndGet() {
            return nullptr;
        }

        /**
         * Returns this message or a nullptr if it is not a KeyValue message
         */
        virtual KeyValue* asKeyValue() {
            return nullptr;
        }

        /**
         * Returns this message or a nullptr if it is not a Put message
         */
        virtual Put* asPut() {
            return nullptr;
        }

        /**
         * Returns this message or a nullptr if it is not a Register message
         */
        virtual Register* asRegister() {
            return nullptr;
        }

        /**
         * Returns this message or a nullptr if it is not a Directory message
         */
        virtual Directory* asDirectory() {
            return nullptr;
        }
};

/**
 * Represents a message for finding a value for a key. 
 */
class WaitAndGet : public Message {
    public:
        Key* key; // not owned

        WaitAndGet(Key* key, size_t sender_, size_t target_) : Message(sender_, target_) {
            kind_ = MsgKind::WaitAndGet;
            this->key = key;
        }

        ~WaitAndGet() {
        }

        /**
         * deserializes a char* to a wait and get message
         */
        static WaitAndGet* deserialize(char* deserial) {
            Sys s;
            int x = 34;
            int y;
            char* c = s.substring(deserial, 0, x);
            assert(strcmp(c, "{MESSAGE|kind_=WaitAndGet|sender_=") == 0);
            delete[] c;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            size_t sender_ = strtol(c, nullptr, 10);
            delete[] c;
            x = x + y;
            x = x + 1;
            c = s.substring(deserial, x, 8);
            assert(strcmp(c, "target_=") == 0);
            x = x + 8;
            delete[] c;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            size_t target_ = strtol(c, nullptr, 10);
            delete[] c;
            x = x + y;
            x = x + 1;
            c = s.substring(deserial, x, 4);
            assert(strcmp(c, "key=") == 0);
            delete[] c;
            x = x + 4;
            y = s.parseUntilClassSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            WaitAndGet* returning =  new WaitAndGet(Key::deserialize(c), sender_, target_);
            delete[] c;
            return returning;
        }

        /**
         * returns the key
         */
        Key* getKey() {
            return key;
        }

        /**
         * serializes the key field
         */
        virtual char* serializeAditionalFields_() {
            StrBuff strbuff;
            strbuff.c("key=");
            strbuff.c(key->serialize());
            strbuff.c("|}");
            return strbuff.get_char();
        }
        
        /**
         * determines if other is a wait and get, and if all other properties between them are the same
         */
        virtual bool equals(Object* other) {
            WaitAndGet* a = dynamic_cast<WaitAndGet*> (other);
            if(a) {
                return this->kind_ == a->getKind() && this->sender_ == a->getSender() && this->target_ == a->getTarget() && a->getKey()->equals(this->getKey());
            }
            else {
                return false;
            }
        }

        /**
         *  returns this.
         */
        virtual WaitAndGet* asWaitAndGet() {
            return this;
        }
};

 

/**
 *  Represents a message that contains a key and a value for that key
 */
class KeyValue : public Message {
    public:
        Key* key; 
        char* val;

        KeyValue(Key* key, char* val, size_t sender_, size_t target_) :  Message(sender_, target_) {
            kind_ = MsgKind::KeyValue;
            this->key = key;
            this->val = val;
        }

        ~KeyValue() {
        }

        /**
         * static method for deserialziing a char* into a KeyValue
         */
        static KeyValue* deserialize(char* deserial) {
            Sys s;
            int x = 32;
            int y;
            char* c = s.substring(deserial, 0, x);
            assert(strcmp(c, "{MESSAGE|kind_=KeyValue|sender_=") == 0);
            delete[] c;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            size_t sender_ = strtol(c, nullptr, 10);
            delete[] c;
            x = x + y;
            x = x + 1;
            c = s.substring(deserial, x, 8);
            assert(strcmp(c, "target_=") == 0);
            x = x + 8;
            delete[] c;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            size_t target_ = strtol(c, nullptr, 10);
            delete[] c;
            x = x + y;
            c = s.substring(deserial, x, 5);
            assert(strcmp(c, "|key=") == 0);
            delete[] c;
            x = x + 5;
            y = s.parseUntilClassSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            Key* key = Key::deserialize(c);
            delete[] c;
            x = x + y;
            c = s.substring(deserial, x, 7);
            assert(strcmp(c, "|value=") == 0);
            delete[] c;
            x = x + 7;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            KeyValue* returning =  new KeyValue(key, c, sender_, target_);
            return returning;
        }

        /**
         * returns the key of the key value
         */
        Key* getKey() {
            return key;
        }

        /**
         * returns the value of the key value
         */
        char* getVal() {
            return val;
        }

        /**
         * Serializes the additional fields (key, value)
         */
        virtual char* serializeAditionalFields_() {
            StrBuff strbuff;
            strbuff.c("key=");
            strbuff.c(key->serialize());
            strbuff.c("|value=");
            strbuff.c(val);
            strbuff.c("|}");
            return strbuff.get_char();
        }
        
        /**
         * determines if other is a KeyValue, and if all other properties between them are the same
         */
        virtual bool equals(Object* other) {
            KeyValue* a = dynamic_cast<KeyValue*> (other);
            if(a) {
                return this->kind_ == a->getKind() && this->sender_ == a->getSender() && this->target_ == a->getTarget() && a->getKey()->equals(this->getKey()) && strcmp(getVal(), a->getVal()) == 0;
            }
            else {
                return false;
            }
        }

        /**
         * returns this
         */ 
        virtual KeyValue* asKeyValue() {
            return this;
        }
};


/**
 * Represents a message that contains a key and a value for that key, and expects the reciever to put the
 * ket value pair in their dictionary
 */
class Put : public Message {
    public:
        Key* key; 
        char* val;

        Put(Key* key, char* val, size_t sender_, size_t target_) :  Message(sender_, target_) {
            kind_ = MsgKind::Put;
            this->key = key;
            this->val = val;
        }

        ~Put() {
        }

        /**
         * deserializes deserial into a put message
         * errors if deserial is not in the format expected
         */
        static Put* deserialize(char* deserial) {
            Sys s;
            int x = 27;
            int y;
            char* c = s.substring(deserial, 0, x);
            assert(strcmp(c, "{MESSAGE|kind_=Put|sender_=") == 0);
            delete[] c;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            size_t sender_ = strtol(c, nullptr, 10);
            delete[] c;
            x = x + y;
            x = x + 1;
            c = s.substring(deserial, x, 8);
            assert(strcmp(c, "target_=") == 0);
            x = x + 8;
            delete[] c;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            size_t target_ = strtol(c, nullptr, 10);
            delete[] c;
            x = x + y;
            c = s.substring(deserial, x, 5);
            assert(strcmp(c, "|key=") == 0);
            delete[] c;
            x = x + 5;
            y = s.parseUntilClassSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            Key* key = Key::deserialize(c);
            delete[] c;
            x = x + y;
            c = s.substring(deserial, x, 7);
            assert(strcmp(c, "|value=") == 0);
            delete[] c;
            x = x + 7;
            y = s.parseUntilSeperator(deserial, x);
            c = s.substring(deserial, x, y);
            Put* returning =  new Put(key, c, sender_, target_);
            return returning;
        }

        /**
         * returns the key
         */
        Key* getKey() {
            return key;
        }

        /**
         * returns the vlaue
         */
        char* getVal() {
            return val;
        }

        /**
         * returns the serialized version of key and value
         */
        virtual char* serializeAditionalFields_() {
            StrBuff strbuff;
            strbuff.c("key=");
            strbuff.c(key->serialize());
            strbuff.c("|value=");
            strbuff.c(val);
            strbuff.c("|}");
            return strbuff.get_char();
        }
        
        /**
         * Determins if other is also a put and if it is if it has the same attributes.
         */
        virtual bool equals(Object* other){
            Put* a = dynamic_cast<Put*> (other);
            if(a) {
                return this->kind_ == a->getKind() && this->sender_ == a->getSender() && this->target_ == a->getTarget() && a->getKey()->equals(this->getKey()) && strcmp(getVal(), a->getVal()) == 0;
            }
            else {
                return false;
            }
        }

        /**
         * returns this
         */
        virtual Put* asPut() {
            return this;
        }
};


/**
 * Register message used for requesting a client to register to a server
 **/
class Register : public Message {
    public:
        sockaddr_in client;

        Register(sockaddr_in client) :  Register(client, 0, 0) { }

        Register(sockaddr_in client, size_t sender_, size_t target_) :  Message(sender_, target_) {
            kind_ = MsgKind::Register;
            this->client = client;
        }

        /**
         * interprates char* reg as a Register message
         **/
        static Register* deserialize(char* reg) {
            Sys s;
            int x = 24;
            int y;
            assert(strcmp(s.substring(reg, 0, x), "{MESSAGE|kind_=Register|") == 0);
            assert(strcmp(s.substring(reg, x, 8), "sender_=") == 0);
            x = x + 8;
            y = s.parseUntilSeperator(reg, x);
            char* c = s.substring(reg, x, y);
            size_t sender = strtol(c, nullptr, 10);
            x = x + y;
            x = x + 1;
            assert(strcmp(s.substring(reg, x, 8), "target_=") == 0);
            x = x + 8;
            y = s.parseUntilSeperator(reg, x);
            c = s.substring(reg, x, y);
            size_t target = strtol(c, nullptr, 10);
            x = x + y;
            x = x + 1;
            assert(strcmp(s.substring(reg, x, 13), "client=[port=") == 0);
            x = x + 13;
            y = s.parseUntilSeperator(reg, x);
            c = s.substring(reg, x, y);
            x = x + y + 1;
            sockaddr_in newsock;
            newsock.sin_family = AF_INET;
            newsock.sin_port = htons(strtol(c, nullptr, 10));
            assert(strcmp(s.substring(reg, x, 3), "ip=") == 0);
            x = x + 3;
            y = s.parseUntilBracketSeperator(reg, x);
            c = s.substring(reg, x, y);

            return new Register(newsock, sender, target);
        }

        /**
         * returns this
         */
        virtual Register* asReg() {
            return this;
        }

        /**
         * returns client
         */
        virtual sockaddr_in getSockAddr() {
            return client;
        } 

        /**
         * serializes client
         */
        virtual char* serializeAditionalFields_() {
            char* buff = new char[2048];
            sprintf(buff, "client=[port=%d|ip=%s]|}", ntohs(client.sin_port), inet_ntoa(client.sin_addr));
            return buff;
        }

        /**
         * determines if other is a register message and if it is the same message as this
         */
        virtual bool equals(Object* other) {
            Register* m = dynamic_cast<Register*> (other);
            if(m) {
                return this->kind_ == m->getKind() && this->sender_ == m->getSender() && this->target_ == m->getTarget();
            }
            else {
                return false;
            }
        }
};

/**
 * Directory message used for passing the ips and ports of servers
 */
class Directory : public Message {
    public:
        Array<String*>* ips_;
        Array<int>* ports_;

        Directory(Array<String*>* ips, Array<int>* ports) : Directory(ips, ports, 0 , 0) { }

        Directory(Array<String*>* ips, Array<int>* ports, size_t sender_, size_t target_) :  Message(sender_, target_) {
            kind_ = MsgKind::Directory;
            ips_ = ips;
            ports_ = ports;
        }


        /**
         * Interprates dir as a directory
         */
        static Directory* deserialize(char* dir) {
            Sys s;
            int x = 25;
            int y;
            assert(strcmp(s.substring(dir, 0, x), "{MESSAGE|kind_=Directory|") == 0);
            assert(strcmp(s.substring(dir, x, 8), "sender_=") == 0);
            x = x + 8;
            y = s.parseUntilSeperator(dir, x);
            char* c = s.substring(dir, x, y);
            size_t sender_ = strtol(c, nullptr, 10);
            x = x + y;
            x = x + 1;
            assert(strcmp(s.substring(dir, x, 8), "target_=") == 0);
            x = x + 8;
            y = s.parseUntilSeperator(dir, x);
            c = s.substring(dir, x, y);
            size_t target_ = strtol(c, nullptr, 10);
            x = x + y;
            x = x + 1;
            assert(strcmp(s.substring(dir, x, 5), "ips_=") == 0);
            x = x + 5;
            y = s.parseUntilClassSeperator(dir, x);
            c = s.substring(dir, x, y);
            Array<String*>* ips_ = Array<String*>::deserialize_stringarray(c);
            x = x + y;
            assert(strcmp(s.substring(dir, x, 8), "|ports_=") == 0);
            x = x + 8;
            y = s.parseUntilClassSeperator(dir, x);
            c = s.substring(dir, x, y);
            Array<int>* ports_ = Array<int>::deserialize_intarray(c);
            return new Directory(ips_, ports_, sender_, target_);
        }
        
        /**
         * serializes ports and ips
         */
        virtual char* serializeAditionalFields_() {
            char* buff = new char[2048];
            sprintf(buff, "ips_=%s|ports_=%s|}", ips_->serialize(), ports_->serialize()); 
            return buff;
        }

        /**
         * returns the directorys's ip's
         */
        Array<String*>* getIps() {
            return ips_;
        }

        /**
         * determines if other is a directory and if it is equal
         */
        virtual bool equals(Object* other) {
            Directory* m = dynamic_cast<Directory*> (other);
            if(m) {
                return this->kind_ == m->getKind() && this->sender_ == m->getSender() && this->target_ == m->getTarget() && this->ips_->equals(m->getIps()) && this->ports_->equals(m->ports_);
            }
            else {
                return false;
            }
        }

        /**
         * returns this
         */
        virtual Directory* asDirectory() {
            return this;
        }
};

/**
 * deserializes a message
 */
Message* Message::deserialize(char* s) {
    Sys sys;
    int x = 15;
    if(strcmp(sys.substring(s, 0, x), "{MESSAGE|kind_=") == 0) {
        int y = sys.parseUntilSeperator(s, x);
        char* c = sys.substring(s, x, y);
        if(strcmp(c, "Put") == 0) {
            return Put::deserialize(s);
        }
        else if(strcmp(c, "WaitAndGet") == 0) {
            return WaitAndGet::deserialize(s);
        }
        else if(strcmp(c, "Register") == 0) {
            return Register::deserialize(s);
        }
        else if(strcmp(c, "KeyValue") == 0) {
            return KeyValue::deserialize(s);
        }
        else if(strcmp(c, "Directory") == 0) {
            return Directory::deserialize(s);
        }
        else {
            assert(false);
        }
    }

}