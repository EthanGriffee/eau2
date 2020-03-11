#pragma once
#include "assert.h"

/**
 * Interface for serializeing and desserializing data
 * */
class Serialize {

    public:

        /**
         * returns a char* representation of the Object.
         * */
        virtual char* serialize() {assert(-1);}
};