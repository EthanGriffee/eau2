#pragma once
#include "helper.h"

/** Base class for all objects in the system.
 *  author: vitekj@me.com */
class Object : public Sys {
public:

    Object() { }
      
    /** Subclasses may have something to do on finalziation */
    virtual ~Object() { }

    /** Subclasses should redefine */
    virtual bool equals(Object  * other) { return this == other;  }

    /** Return a copy of the object; nullptr is considered an error */
    virtual Object* clone() { return nullptr; }

    /** Compute the hash code (subclass responsibility) */
    virtual size_t hash() { return 1; };

    /** Returned c_str is owned by the object, don't modify nor delete. */
    virtual char* c_str() { return nullptr; }

    virtual char* serialize() {
        char* buff = new char[2048];
        sprintf(buff, "{Object}"); 
        return buff;
    }
}; 
