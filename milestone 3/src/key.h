#pragma once

#include "string.h"
#include "object.h"


class Key : public Object {
    public:
        char* name;
        size_t node;

        Key(char* name, size_t node) {
            this->name = name;
            this->node = node;
        }

        ~Key() {
        }

        char* serialize() {
            StrBuff strbuff;
            strbuff = strbuff.c("{KEY|name=");
            strbuff = strbuff.c(name);
            strbuff = strbuff.c("|node=");
            strbuff = strbuff.c(node);
            strbuff = strbuff.c("|}");
            return strbuff.get_char();;
        }

        static Key* deserialize(char* s) {
            Sys sys;
            int x = 10;
            char* c = sys.substring(s, 0, x);
            assert(strcmp(c, "{KEY|name=") == 0);
            delete[] c;
            int y = sys.parseUntilSeperator(s, x);
            char* name = sys.substring(s, x, y);
            x = x + y; 
            c = sys.substring(s, x, 6);
            assert(strcmp(c, "|node=") == 0);
            x = x + 6;
            y = sys.parseUntilSeperator(s, x);
            c = sys.substring(s, x, y);
            size_t node = strtol(c, nullptr, 10);
            delete[] c;
            return new Key(name, node);
        }

        void changeNode(size_t node) {
            this->node = node;
        }

        char* getKey() {
            return name;
        }

        size_t getNode() {
            return node;
        }

        virtual bool equals(Object* other) {
            Key* other_key = dynamic_cast<Key*>(other);
            if (other_key) {
                return this->getNode() == other_key->getNode() && (strcmp(name, other_key->getKey()) == 0);
            }
            return false;
        }
};
