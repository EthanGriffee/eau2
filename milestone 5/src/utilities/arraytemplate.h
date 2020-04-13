#pragma once
#include "object.h"
#include "string.h"
#include <stdio.h>
#include <math.h> 
#include <limits>
#include <type_traits>

/**
 * Authors : Ethan Griffee and Divit Koradia
 * Array template class used for creating many different
 * types of arrays. Methods like serealize and equal defined
 * first for specific types used in the certain arrays.
 **/


class Column;

/**
 * uses the == operator when comparing equality between two integral types.
 */
template <typename IntegralType>
typename std::enable_if<std::is_integral<IntegralType>::value, bool>::type
equal(const IntegralType& a, const IntegralType& b) {
        return a == b;
}

/**
 * uses an epsilon operator when comparing equality between two floating types.
 */
template <typename FloatingType>
typename std::enable_if<std::is_floating_point<FloatingType>::value, bool>::type
equal(const FloatingType& a, const FloatingType& b) {
        return fabs(a-b) < std::numeric_limits<FloatingType>::epsilon();
}

/**
 * uses the object method equals() when comparing two object types
 **/
bool equal(Object* a, Object* b) {
        return a->equals(b);
}

/**
 * uses stcmp when comparing two char*s 
 **/
bool equal(char* a, char* b) {
    return strcmp(a, b) == 0;
}

/**
 * Serializes a floating type by storing it inside a buffer and returning the buffer.
 */
template <typename FloatingType>
typename std::enable_if<std::is_floating_point<FloatingType>::value, char*>::type
serialize_element(const FloatingType& f) {
    char* a =  new char[1024];
    sprintf(a, "{%f}", f);
    return a;
}

/**
 * Serializes a integral type by storing it inside a buffer and returning the buffer.
 */
char* serialize_element(char i) {
    char* a =  new char[4];
    sprintf(a, "{%c}", i);
    return a;
}

/**
 * Serializes a integral type by storing it inside a buffer and returning the buffer.
 */
char* serialize_element(int i) {
    char* a =  new char[1024];
    sprintf(a, "{%i}", i);
    return a;
}

/**
 * Serializes a char* by just returning the char*.
 */
char* serialize_element(char* a) {
    return a; 
}

/**
 * Serializes a boolean by returning a 0 if true and 1 if false.
 */
char* serialize_element(bool b) {
    char* a =  new char[4];
    sprintf(a, "{%i}", b ? 0 : 1);  
    return a; 
}

/**
 * Uses the Serialize method in object to serialize an object.
 */
char* serialize_element(Object* a) {
    return a->serialize(); 
}



/**
 * A template for creating arrays.
 */
template <class arrayClass>
class Array : public Object{
public:
    arrayClass* array;
    size_t size;
    size_t max_capacity;

    /**
     * Default constructor which will set the initial max-capacity to the array to 10. 
     **/
    Array() : Array(10) { }

    Array(size_t max) {
        max_capacity = max;
        size = 0;
        array = new arrayClass[max];
    }

    /**
     * Destructor which will free the memory of the underlying array as well. 
     **/
    ~Array() {
        delete[] array;
    }

    /**
     * Will return the Object pointer at the provided index. if the index is invalid, 
     * then the method will return NULL.
     * @arg index Location to get the value in the array at. 
     **/
    arrayClass get(size_t index) {
        assert(index < size);
        return array[index];
    }

    /**
     * Resizing the underlying array. And then copying over the elements to a new array with
     * the updated size. 
     * Default is doubling the array size when the max capacity of the 
     * underlying array less the number of elements is 2. 
     **/
    void resize() {
        max_capacity = max_capacity * 2;
        arrayClass* new_array = new arrayClass[max_capacity];
        for (size_t x = 0; x < size; x++) {
            new_array[x] = array[x];
        }
        delete[] array;
        array = new_array;
    }

    /**
     *  returns the most recent element and removes it from the array
     **/
    arrayClass pop() {
        assert(size > 0);
        arrayClass returning = this->get(this->getSize() - 1);
        size -= 1;
        return returning;
    }

    /**
     * adds to_push the beginning of the array
     **/ 
    void push_back(arrayClass to_push) {
        add(to_push, 0);
    }

    /**
     * Returns the first index of the of given Object pointer. 
     * If the object pointer is not found then -1 is returned.
     * @arg to_find Object to find the index of. 
     **/
    virtual int indexOf(arrayClass to_find) {
        for (int x = 0; x < size; x++) {
            if(equal(array[x], to_find)) {
                return x;
            }
        }
        return -1;
    }

    /**
     * Adds the element provided to the end of the list,
     * The size is incremented by 1. If resizing the array is necessary, 
     * then that should be done.
     * @arg to_add Object to be added to the array. 
     **/
    virtual void add(arrayClass to_add) {
        if (size + 2 >= max_capacity) {
            resize();
        }
        array[size] = to_add;
        size += 1;
    }

    /**
     * Adds the provided array to the end of the list, unless the given array is NULL, 
     * then it will not be added. 
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array. If resizing the array is necessary, then that should be done.
     **/
    virtual void addAll(Array<arrayClass>* to_add) {
        size_t add_length = to_add->getSize();
        while (size + add_length + 2 >= max_capacity) {
            resize();
        }
        for (size_t i = 0; i < add_length; i++) {
            add(to_add->get(i));
        }
    }

    /**
     * Adds the provided object at the given index, unless the given object is NULL, 
     * then it will not be added. Otherwise, all elements previously at the index and
     * to the right will be shifted accordingly. 
     * If the index is invalid, then nothing will be added/shifted. 
     * The size of this array is incremented by 1. 
     * If resizing the array is necessary, then that should be done. 
     * @arg index Location to add the Object at
     **/
    virtual void add(arrayClass to_add, size_t index) {
        if (index > size) {
            return;
        }
        if (size + 2 >= max_capacity) {
            resize();
        }
        size += 1;
        for (size_t x = size; x > index; x--){
            array[x] = array[x - 1];
        }
        array[index] = to_add;
    }

    /**
     * Sets the element at position index to to_add
     */
    virtual void set(arrayClass to_add, size_t index) {
        if (index > size) {
            return;
        }
        if (index == size) {
            add(to_add);
            return;
        }
        if (size + 2 >= max_capacity) {
            resize();
        }
        array[index] = to_add;
    }

    /**
     * Adds all the elements of the provided array at the given index, 
     * unless the given array is NULL, then it will not be added. Otherwise, 
     * all elements previously at the index and
     * to the right will be shifted accordingly by the size of the procided array, 
     * If the index is invalid, then nothing will be added/shifted.
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array.  If resizing the array is necessary, then that should be done.
     * @arg index Location to add the objects to the array at
     **/
    virtual void addAll(Array<arrayClass>* to_add, size_t index) {
        if (index > size) {
            return;
        }
        size_t add_length = to_add->getSize();
        while (size + add_length + 2 >= max_capacity) {
            resize();
        }
        size += add_length;
        for (size_t x = size - 1; x >= index + add_length; x--){
            array[x] = array[x - add_length];
        }
        for (size_t x = 0; x < add_length; x++){
            array[index + x] = to_add->get(x);
        }
    }

    /**
     * Removes the first instance of the given Object in this array. If nothing 
     * is found, then no action will occur. The size reduces by 1 if the 
     * element is found.
     * @arg to_remove Object to be removed from the array
     **/
    virtual void remove(arrayClass to_remove) {
        for (size_t i = 0; i < size; i ++) {
            if (equal(this->get(i), (to_remove))) {
                erase(i);
                return;
            }
        }
    }

    /**
     * removes the index from the array
     */
    virtual void erase(int index) {
        size -= 1;
        for (size_t x = index; x < size; x++) {
            array[x] = array[x + 1];
        }
    }
    /**
     * Removes all instances of the given Object in this array. If nothing 
     * is found, then no action will occur. The size reduces the number of found
     * elements there are.
     * @arg to_remove Object that all instances in the array will be removed of
     **/
    virtual void removeAll(arrayClass to_remove) {
        for (size_t i = 0; i < size; i ++) {
            if (equal(this->get(i), to_remove)) {
                size -= 1;
                for (int x = i; x < size; x++) {
                    array[x] = array[x + 1];
                }
                i-=1;
            }
        }
    }

    /**
     * Returns number of elements in the array. 
     **/
    virtual size_t getSize() {
        return size;
    }

    /**
     * returns the length of the headers of the deserialized array, and checks to make sure
     * it matches
     **/
    static int assert_correct_deserial_(char* check, const char* className) {
        StrBuff strbuff;
        strbuff = strbuff.c("{Array|type=");
        strbuff = strbuff.c(className);
        strbuff = strbuff.c("|array=");
        char* val = strbuff.get_char();
        int x = strbuff.size_;
        char* c = strbuff.substring(check, 0, x);
        assert(strcmp(c, val) == 0);
        delete[] val;
        delete[] c;
        return x;
    }

    /**
     * Serializes an array by calling serialize_element on each element of the array.
     */
    char* serialize() {
        StrBuff strbuff;
        strbuff = strbuff.c("{Array|type=");
        strbuff = strbuff.c(typeid(arrayClass).name());
        strbuff = strbuff.c("|array=");
        for(int x = 0; x < size; x++) {
            char* element = serialize_element(this->get(x));
            strbuff.c(element);
            delete[] element;
        } 
        strbuff.c("|}");
        return strbuff.get_char();
    }

    /**
     * static method for deserializing a double array.
     * @arg s the char* beign deserialized.
     **/
    static Array<double>* deserialize_doublearray(char* s) {
        int y;
        Array<double>* returning = new Array<double>();
        const char* className = typeid(double).name();
        int x = Array<double>::assert_correct_deserial_(s, className);
        while(s[x] == '{') {
            y = returning->parseUntilClassSeperator(s, x);
            char* c = returning->substring(s, x + 1, y - 1);
            returning->add(atof(c));
            x += y;
            delete[] c;
        }
        return returning;
    }

     /**
     * static method for deserializing a bool array.
     * @arg s the char* beign deserialized.
     **/
    static Array<bool>* deserialize_boolarray(char* s) {
        int y;
        Array<bool>* returning = new Array<bool>();
        const char* className = typeid(bool).name();
        int x = Array<bool>::assert_correct_deserial_(s, className);
   
        while(s[x] == '{') {
            y = returning->parseUntilClassSeperator(s, x);
            char* c = returning->substring(s, x + 1, y - 2);
            returning->add(strcmp(c, "0") == 0);
            x += y;
            delete[] c;
        }
        return returning;
    }

    /**
     * static method for deserializing an int array.
     * @arg s the char* beign deserialized.
     **/
    static Array<int>* deserialize_intarray(char* s) {
        int y;
        Array<int>* returning = new Array<int>();
        const char* className = typeid(int).name();
        int x = Array<int>::assert_correct_deserial_(s, className);
   
        while(s[x] == '{') {
            y = returning->parseUntilClassSeperator(s, x);
            char* c = returning->substring(s, x + 1, y - 2);
            returning->add(atoi(c));
            x += y;
            delete[] c;
        }
        return returning;
    }

    /**
     * static method for deserializing a string array.
     * @arg s the char* beign deserialized.
     **/
    static Array<String*>* deserialize_stringarray(char* s) {
        int y;
        Array<String*>* returning = new Array<String*>();
        const char* className = typeid(String*).name();
        int x = Array<bool>::assert_correct_deserial_(s, className);
   
        while(s[x] == '{') {
            y = returning->parseUntilClassSeperator(s, x);
            char* c = returning->substring(s, x, y);
            returning->add(String::deserialize(c));
            x += y;
            delete[] c;
        }
        return returning;
    }

    /**
     * static method for deserializing a chararray.
     * @arg s the char* beign deserialized.
     **/
    static Array<char>* deserialize_chararray(char* s) {
        int y;
        Array<char>* returning = new Array<char>();
        const char* className = typeid(char).name();
        int x = Array<bool>::assert_correct_deserial_(s, className);
   
        while(s[x] == '{') {
            y = returning->parseUntilClassSeperator(s, x);
            char* c = returning->substring(s, x + 1, y - 1);
            returning->add(c[0]);
            x += y;
            delete[] c;
        }
        return returning;
    }

    /**
     * static method for deserializing a column array.
     * @arg s the char* beign deserialized.
     **/
    static Array<Column*>* deserialize_columnarray(char* s);

    /**
     * Determines if a object is equal to this by casting it to a array of this class
     * and comparing each elemnt of the array
     * @arg other the object* being compared.
     **/
    bool equals(Object* other) {
        Array<arrayClass>* arr = dynamic_cast<Array<arrayClass> *>(other);
        if (!arr || arr->getSize() != size) {
            return false;
        }
        for (size_t x = 0; x < size; x++) {
            if (!equal(array[x], arr->get(x))) {
                return false;
            }
        }
        return true;
    }

    /**
     * deletes all elements of the array and sets the size to 0
     **/
    void delete_contents() {
        for (size_t x = 0; x < this->getSize(); x++) {
            delete array[x];
        }
        this->size = 0;
    }
};