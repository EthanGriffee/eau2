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

/**
 * An enumeration of the current possible message kidns
 */
enum class MsgKind { WaitAndGet, KeyValue, Put };

 
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
            strbuff.c("|key=");
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
            strbuff.c("|key=");
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
            strbuff.c("|key=");
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
