#pragma once
#include <stdlib.h>
#include "object.h"
#include "deserialize.h"
#include "string.h"

// Wrapper for a Double
class DoubleObj : public Object {
    public:
        double f;

        // sets the value of f to input_f
        DoubleObj(double input_f) {
            f = input_f;
        }

        DoubleObj(DoubleObj& f) {
            f = f.getDouble();
        }

        // returns if other is a double that has the same f
        bool equals(Object *other) {
            double epsilon = .01;
            DoubleObj * f2 = dynamic_cast<DoubleObj*> (other);
            if (f2 && (f2->getDouble() - f < epsilon)  && (f2->getDouble() - f > -epsilon)){
                return true;
            }
            return false;
        }

        // returns f
        double getDouble() {
            return f;
        }
};

// Wrapper for a Boolean
class BoolObj : public Object {
    public:
        bool b;

        // sets the value of b to input_b
        BoolObj(bool input_b) {
            b = input_b;
        }

        BoolObj(BoolObj& b) {
            b = b.getBool();
        }

        // returns if other is a boolean that has the same b
        bool equals(Object *other){
            BoolObj * b2 = dynamic_cast<BoolObj*> (other);
            if (b2 && b2->getBool() == b ){
                return true;
            }
            return false;
        }

        // returns b
        bool getBool() {
            return b;
        }
};

// Wrapper for a Integer
class IntObj : public Object{
    public:
        int i;

        // sets the value of i to input_i
        IntObj(int input_i) {
            i = input_i;
        }

        // returns if other is a integer that has the same i
        bool equals(Object *other){
            IntObj * i2 = dynamic_cast<IntObj*> (other);
            if (i2 && i2->getInt() == i ){
                return true;
            }
            return false;
            
        }

        
        IntObj(IntObj& i) {
            i = i.getInt();
        }

        // returns i
        int getInt() {
            return i;
        }
};



/**
 * This class represents an Array, more akin to a dynamic ArrayList type
 * versus a fixed-sized array, which may already be represented or built-in in CwC.
 * This interface offers the overall main functionalities:
 *      Addition of elements
 *      Addition of Arrays
 *      Removal of elements
 *      Indexing by Element
 *      Get Element by Index
 *      Equality
 *      Replacing elements at an Index
 * This class CAN be implemented using an underlying array structure that is dynamically resized. 
 * Note that the inital memory outlay is an array of pointers of size 10. We wish to reduce
 * the amount of resizing of the array, which would make several operations have O(n) time instead
 * of O(1) time. The below fields exist to serve as an example of how to implement. 
 * There are several STUB implementations in this API: In order to write a StringArray
 * you will need to extend Array and correctly override the virtual functions. 
 **/
class Array : public Object {
public:
    Object** array;
    size_t size;
    size_t max_capacity;

    /**
     * Default constructor which will set the initial max-capacity to the array to 10. 
     **/
    Array() {
        max_capacity = 10;
        size = 0;
        array = new Object*[max_capacity];
    }

    Array(Array& arr) {
        size = arr.getSize();
        max_capacity = arr.max_capacity;
        array = new Object*[max_capacity];
        for (size_t i = 0; i < arr.getSize(); i++) {
            Object obj(*(arr.get(i)));
            array[i] = &obj;
        }
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
    virtual Object* get(size_t index) {
        if (index > size) {
            return nullptr;
        }
        return array[index];
    }

    /**
     * Resizing the underlying array. And then copying over the elements to a new array with
     * the updated size. 
     * Default is doubling the array size when the max capacity of the 
     * underlying array less the number of elements is 2. 
     **/
    virtual void resize() {
        max_capacity = max_capacity * 2;
        Object** new_array = new Object*[max_capacity];
        for (size_t x = 0; x < size; x++) {
            new_array[x] = array[x];
        }
        delete[] array;
        array = new_array;
        
    }

    /**
     * Returns the first index of the of given Object pointer. 
     * If the object pointer is not found then -1 is returned.
     * @arg to_find Object to find the index of. 
     **/
    virtual int indexOf(Object* to_find) {
        for (int x = 0; x < size; x++) {
            if(array[x]->equals(to_find)) {
                return x;
            }
        }
        return -1;
    }

    /**
     * Adds the element provided to the end of the list, unless the given element is NULL, 
     * then it will not be added. The size is incremented by 1. If resizing the array is necessary, 
     * then that should be done.
     * @arg to_add Object to be added to the array. 
     **/
    virtual void add(Object* to_add) {
        if (!to_add) {
            return;
        }
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
     * @arg to_add Array of Objects that all need to be added to this array. 
     **/
    virtual void addAll(Array* to_add) {
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
     * @arg to_add Object to be added to the array
     * @arg index Location to add the Object at
     **/
    virtual void add(Object* to_add, size_t index) {
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

    virtual void set(Object* to_add, size_t index) {
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
     * @arg to_add Array of Objects to be added to the array
     * @arg index Location to add the objects to the array at
     **/
    virtual void addAll(Array* to_add, size_t index) {
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
     * Returns the subarray starting from the provided index to the end of the array. 
     * If the index is invalid, then the method returns NULL
     * @arg index Starting place for the subarray
     **/
    virtual Array* subArray(size_t index) {
        return subArray(index, size);
    }

    /**
     * Returns the subarray starting from the provided index to the ending index
     * The starting index must always be greater than the ending index. If either index is invalid, 
     * then NULL is returned. The set is [start, end)
     * @arg startIndex starting place for the subarray
     * @arg endIndex location of the last object to be put in the subarray
     **/
    virtual Array* subArray(size_t startIndex, size_t endIndex) {
        if (startIndex > endIndex || endIndex > size) {
            return nullptr;
        }
        Array* arr = new Array();
        while (startIndex < endIndex) {
            arr->add(this->get(startIndex));
            startIndex += 1;
        }
        return arr;
    }

    /**
     * Removes the first instance of the given Object in this array. If nothing 
     * is found, then no action will occur. The size reduces by 1 if the 
     * element is found.
     * @arg to_remove Object to be removed from the array
     **/
    virtual void remove(Object* to_remove) {
        for (size_t i = 0; i < size; i ++) {
            if (this->get(i)->equals(to_remove)) {
                size -= 1;
                for (size_t x = i; x < size; x++) {
                    array[x] = array[x + 1];
                }
                return;
            }
        }
    }

    /**
     * Removes all instances of the given Object in this array. If nothing 
     * is found, then no action will occur. The size reduces the number of found
     * elements there are.
     * @arg to_remove Object that all instances in the array will be removed of
     **/
    virtual void removeAll(Object* to_remove) {
        for (size_t i = 0; i < size; i ++) {
            if (this->get(i)->equals(to_remove)) {
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
     * Returns this Array as a char* 
     **/
    virtual char* serialize() {
        char* buff = new char[2048];
        sprintf(buff, "{Array|array=");
        for(int x = 0; x < size; x++) {
             sprintf(&buff[strlen(buff)], "%s", this->get(x)->serialize());
        } 
        sprintf(&buff[strlen(buff)], "|}");
        return buff;
    }

    /**
     * Overriding the Object equals method. 
     * Returns if all the elements in this array and the given object are equal and 
     * in the same other. 
     * If the given object is NULL or not an array, then false will be returned.
     * @arg other Object to check if this array is equal to
     **/
    bool equals(Object* other) {
        Array * arr = dynamic_cast<Array*>(other);
        if (arr == nullptr || arr->getSize() != size) {
            return false;
        }
        for (size_t x = 0; x < size; x++) {
            if (!array[x]->equals(arr->get(x))) {
                return false;
            }
        }
        return true;
    }

    /**
     * Overriding the Object hash_me() method. 
     * Hashes the array based on user specifications. Default implementation is
     * to hash all internal elements and sum them up. 
     **/
    size_t hash_me_() {
        size_t n = 0;
        for(int x = 0; x < size; x++) {
            n += array[x]->hash();
        }
        return n;
    }
};
/**
 * Incomplete implementation of String Array. No methods overriden from
 * Array. Created for testing purposes, so we can design tests. 
 **/
class StringArray : public Object {
    public:
        Array* str_arr;

        /**
         * Default constructor which will set the initial max-capacity to the array to 10. 
         **/
        StringArray() {
            str_arr = new Array();
        }
        
        StringArray(Array* arr) {
            this->str_arr = arr;
        }

        /**
         * Initalized this array with the characteristics of the passed in array.
         * @arg arr Array containing values to be used in initialization. 
         **/
        StringArray(StringArray* arr) {
            str_arr = arr->str_arr;
        }

        /**
         * Destructor which will free the memory of the underlying array as well. 
         **/
        ~StringArray() {
            delete str_arr;
        }

        /**
         * Will return the String pointer at the provided index. if the index is invalid, 
         * then the method will return NULL.
         * @arg index Location to get the value in the array at. 
         **/
        String* get(size_t index) {
            return static_cast <String*> (str_arr->get(index));
        }

        /**
         * Resizing the underlying array. And then copying over the elements to a new array with
         * the updated size. 
         * Default is doubling the array size when the max capacity of the 
         * underlying array less the number of elements is 2. 
         **/
        void resize() {
            str_arr->resize();
        }

        /**
         * Returns the first index of the of given String pointer. 
         * If the pointer is not a String, return -1. 
         * If the string pointer is not found then -1 is returned.
         * @arg to_find String to find the index of. 
         **/
        int indexOf(Object* to_find) {
            return str_arr->indexOf(to_find);
        }

        /**
         * Adds the element provided to the end of the list, unless the given element is NULL, 
         * then it will not be added. The size is incremented by 1. If resizing the array is necessary, 
         * then that should be done.
         * @arg to_add Object to be added to the array. 
         **/
        void add(String* to_add) {
            return str_arr->add(to_add);
        }

    /**
     * Adds the provided array to the end of the list, unless the given array is NULL, 
     * then it will not be added. 
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array. If resizing the array is necessary, then that should be done.
     * @arg to_add Array of String that all need to be added to this array. 
     **/
     void addAll(StringArray* to_add) {
         return str_arr->addAll(to_add->str_arr);
     }

    /**
     * Adds the provided String at the given index, unless the given object is NULL, 
     * then it will not be added. Otherwise, all elements previously at the index and
     * to the right will be shifted accordingly. 
     * If the index is invalid, then nothing will be added/shifted. 
     * The size of this array is incremented by 1. 
     * If resizing the array is necessary, then that should be done.
     * If the object provided is not a String, then do nothing.  
     * @arg to_add Object to be added to the array
     * @arg index Location to add the Object at
     **/
     void add(String* to_add, size_t index) {
         return str_arr->add(to_add, index);
     }

    virtual void set(String* to_add, size_t index) {
        return str_arr->add(to_add, index);
    }

    /**
     * Adds all the elements of the provided array at the given index, 
     * unless the given array is NULL, then it will not be added. Otherwise, 
     * all elements previously at the index and
     * to the right will be shifted accordingly by the size of the procided array, 
     * If the index is invalid, then nothing will be added/shifted.
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array.  If resizing the array is necessary, then that should be done.
     * @arg to_add Array of Objects to be added to the array
     * @arg index Location to add the objects to the array at
     **/
     void addAll(StringArray* to_add, size_t index) {
        str_arr->addAll(to_add->str_arr, index);
     }

    /**
     * Returns the subarray starting from the provided index to the end of the array. 
     * If the index is invalid, then the method returns NULL
     * @arg index Starting place for the subarray
     **/
     StringArray* subArray(size_t index) {
        StringArray* returning = new StringArray();
        if (index > getSize()) {
            return nullptr;
        }
        while(index < getSize()) {
            returning->add(get(index));
            index += 1;
        }
        return returning;
     }

    /**
     * Returns the subarray starting from the provided index to the ending index
     * The starting index must always be greater than the ending index. If either index is invalid, 
     * then NULL is returned. The set is [start, end)
     * @arg startIndex starting place for the subarray
     * @arg endIndex location of the last object to be put in the subarray
     **/
     StringArray* subArray(size_t startIndex, size_t endIndex) {
        StringArray* returning = new StringArray();
        if (endIndex > getSize() || startIndex > endIndex) {
            return nullptr;
        }
        while(startIndex < endIndex) {
            returning->add(get(startIndex));
            startIndex += 1;
        }
        return returning;
     }

    /**
     * Removes the first instance of the given Object in this array. If nothing 
     * is found, then no action will occur. The size reduces by 1 if the 
     * element is found.
     * If the object to be removed is not a string, do nothing. 
     * @arg to_remove String to be removed from the array
     **/
     void remove(Object* to_remove) {
         str_arr->remove(to_remove);
     }

    /**
     * Removes all instances of the given Object in this array. If nothing 
     * is found, then no action will occur. The size reduces the number of found
     * elements there are.
     * If the object to remove is not a string, do nothing. 
     * @arg to_remove String that all instances in the array will be removed of
     **/
     void removeAll(Object* to_remove) {
         str_arr->removeAll(to_remove);
     }

    /**
     * Returns number of elements in the array. 
     **/
     size_t getSize() {
         return str_arr->getSize();
     }

    /**
     * Overriding the String equals method. 
     * Returns if all the elements in this array and the given object are equal and 
     * in the same other. 
     * If the given object is NULL or not an array, then false will be returned.
     * @arg other Object to check if this array is equal to
     **/
    bool equals(Object* other) {
        StringArray* o = dynamic_cast<StringArray*> (other);
        return o->str_arr->equals(this->str_arr);
    }

    /**
     * Overriding the Object hash_me() method. 
     * Hashes the array based on user specifications. Default implementation is
     * to hash all internal elements and sum them up. 
     **/
    size_t hash_me_() {
        return str_arr->hash() + 1;
    }

    char* serialize() {
        char* buff = new char[2048];
        sprintf(buff, "{StringArray|str_arr=%s|}", str_arr->serialize()); 
        return buff;
    }

};


class IntArray : public Object {
    public:
        Array* int_arr;
        /**
         * Default constructor which will set the initial max-capacity to the array to 10. 
         **/
        IntArray() {
            int_arr = new Array();
        }

    /**
     * Initalized this array with the characteristics of the passed in array.
     * @arg arr Array containing values to be used in initialization. 
     **/
    IntArray(IntArray* arr) {
        int_arr = arr->int_arr;
    }

    /**
     * Destructor which will free the memory of the underlying array as well. 
     **/
    ~IntArray() {
        delete int_arr;
    }

    /**
     * Will return the int pointer at the provided index. if the index is invalid, 
     * then the method will return NULL.
     * @arg index Location to get the value in the array at. 
     **/
    int get(size_t index) {
        Object* obj = int_arr->get(index); 
        if (obj) {
            return static_cast <IntObj*> (obj)->getInt();
        }
        perror("attempted index out of bounds");
        exit(1);
    }


    virtual void set(int to_add, size_t index) {
        IntObj* obj = new IntObj(to_add);
        return int_arr->set(obj, index);
    }   

    /**
     * Resizing the underlying array. And then copying over the elements to a new array with
     * the updated size. 
     * Default is doubling the array size when the max capacity of the 
     * underlying array less the number of elements is 2. 
     **/
    void resize() {
        int_arr->resize();
    }

    /**
     * Returns the first index of the of given Object pointer. 
     * If the object pointer is not found then -1 is returned.
     * @arg to_find Object to find the index of. 
     **/
    int indexOf(int to_find) {
        IntObj* to_find_obj = new IntObj(to_find);
        int returning = int_arr->indexOf(to_find_obj);
        delete to_find_obj;
        return returning;
    }

    /**
     * Adds the element provided to the end of the list.
     *  The size is incremented by 1. If resizing the array is necessary, 
     * then that should be done.
     * @arg to_add int to be added to the array. 
     **/
    void add(int to_add) {
        IntObj* int_obj = new IntObj(to_add);
        int_arr->add(int_obj);
    }

    /**
     * Adds the provided array to the end of the list, unless the given array is NULL, 
     * then it will not be added. 
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array. If resizing the array is necessary, then that should be done.
     * @arg to_add Array of Objects that all need to be added to this array. 
     **/
    void addAll(IntArray* to_add) {
        int_arr->addAll(to_add->int_arr);
    }

    /**
     * Adds the provided int at the given index. All elements previously at the index and
     * to the right will be shifted accordingly. 
     * If the index is invalid, then nothing will be added/shifted. 
     * The size of this array is incremented by 1. 
     * If resizing the array is necessary, then that should be done. 
     * @arg to_add Object to be added to the array
     * @arg index Location to add the Object at
     **/
    void add(int to_add, size_t index) {
        IntObj* int_obj = new IntObj(to_add);
        int_arr->add(int_obj, index);
    }

    /**
     * Adds all the elements of the provided array at the given index, 
     * unless the given array is NULL, then it will not be added. Otherwise, 
     * all elements previously at the index and
     * to the right will be shifted accordingly by the size of the procided array, 
     * If the index is invalid, then nothing will be added/shifted.
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array.  If resizing the array is necessary, then that should be done.
     * @arg to_add Array of Objects to be added to the array
     * @arg index Location to add the objects to the array at
     **/
    void addAll(IntArray* to_add, size_t index) {
        int_arr->addAll(to_add->int_arr, index);
    }

    /**
     * Returns the subarray starting from the provided index to the end of the array. 
     * If the index is invalid, then the method returns NULL
     * @arg index Starting place for the subarray
     **/
    IntArray* subArray(size_t index) {
        IntArray* returning = new IntArray();
        if (index > getSize()) {
            return nullptr;
        }
        while(index < getSize()) {
            returning->add(get(index));
            index += 1;
        }
        return returning;
        
    }

    /**
     * Returns the subarray starting from the provided index to the ending index
     * The starting index must always be greater than the ending index. If either index is invalid, 
     * then NULL is returned. The set is [start, end)
     * @arg startIndex starting place for the subarray
     * @arg endIndex location of the last object to be put in the subarray
     **/
    IntArray* subArray(size_t startIndex, size_t endIndex) {
        IntArray* returning = new IntArray();
        if (endIndex > getSize() || startIndex > endIndex) {
            return nullptr;
        }
        while(startIndex < getSize()) {
            returning->add(get(startIndex));
            startIndex += 1;
        }
        return returning;
    }

    /**
     * Removes the first instance of the given int in this array. If nothing 
     * is found, then no action will occur. The size reduces by 1 if the 
     * element is found.
     * @arg to_remove int to be removed from the array
     **/
     void remove(int to_remove) {
         IntObj* int_obj = new IntObj(to_remove);
         int_arr->remove(int_obj);
         delete int_obj;
     }

    /**
     * Removes all instances of the given int in this array. If nothing 
     * is found, then no action will occur. The size reduces the number of found
     * elements there are.
     * @arg to_remove int that all instances in the array will be removed of
     **/
     void removeAll(int to_remove) {
         IntObj* int_obj = new IntObj(to_remove);
         int_arr->removeAll(int_obj);
         delete int_obj;
     }

    /**
     * Returns number of elements in the array. 
     **/
    size_t getSize() {
        return int_arr->getSize();
    }

    /**
     * Overriding the Object equals method. 
     * Returns if all the elements in this array and the given object are equal and 
     * in the same other. 
     * If the given object is NULL or not an array, then false will be returned.
     * @arg other Object to check if this array is equal to
     **/
    bool equals(Object* other) {
        IntArray* o = dynamic_cast<IntArray*> (other);
        if (o)
            return o->int_arr->equals(this->int_arr);
        else {
            return false;
        }
    }

    /**
     * Overriding the Object hash_me() method. 
     * Hashes the array based on user specifications. Default implementation is
     * to hash all internal elements and sum them up. 
     **/
    size_t hash_me_() {
        return int_arr->hash_me_() + 2;
    }
};



class DoubleArray : public Object {
    public:
        Array* double_arr;

        /**
         * Default constructor which will set the initial max-capacity to the array to 10. 
         **/
        DoubleArray() {
            double_arr = new Array();
        };

        /**
         * Initalized this array with the characteristics of the passed in array.
         * @arg arr Array containing values to be used in initialization. 
         **/
        DoubleArray(DoubleArray* arr) {
            double_arr = arr->double_arr;
        }

        /**
         * Destructor which will free the memory of the underlying array as well. 
         **/
        ~DoubleArray() {
            delete double_arr;
        }

        /**
         * Will return the double pointer at the provided index. if the index is invalid, 
         * then the method will return -1
         * @arg index Location to get the value in the array at. 
         **/
        double get(size_t index) {
            Object* obj = double_arr->get(index); 
            if (obj) {
                return static_cast <DoubleObj*> (obj)->getDouble();
            }
            perror("attempted to get index out of bounds");
            exit(1);
        }

        /**
         * Clears the array and reinitializes the underlying array to 10 spots with no elements. 
         * Reinitializes the size to 0. 
         **/
        void clear() {
            double_arr->clear();
        }

        /**
         * Resizing the underlying array. And then copying over the elements to a new array with
         * the updated size. 
         * Default is doubling the array size when the max capacity of the 
         * underlying array less the number of elements is 2. 
         **/
        void resize() {
            double_arr->resize();
        }

        /**
         * Returns the first index of the of given double pointer. 
         * If the object pointer is not found then -1 is returned.
         * @arg to_find Object to find the index of. 
         **/
        int indexOf(double to_find) {
            DoubleObj* to_find_obj = new DoubleObj(to_find);
            int returning = double_arr->indexOf(to_find_obj);
            delete to_find_obj;
            return returning;
        }

        /**
         * Adds the element provided to the end of the list.
         *  The size is incremented by 1. If resizing the array is necessary, 
         * then that should be done.
         * @arg to_add int to be added to the array. 
         **/
        void add(double to_add) {
            DoubleObj* double_obj = new DoubleObj(to_add);
            double_arr->add(double_obj);
        }

        virtual void set(double to_add, size_t index) {
            DoubleObj* obj = new DoubleObj(to_add);
            double_arr->set(obj, index);
        }   

        /**
         * Adds the provided array to the end of the list, unless the given array is NULL, 
         * then it will not be added. 
         * Assuming a valid move, the size of this array is incremented by the size of the 
         * added array. If resizing the array is necessary, then that should be done.
         * @arg to_add Array of Objects that all need to be added to this array. 
         **/
        void addAll(DoubleArray* to_add) {
            double_arr->addAll(to_add->double_arr);
        }

        /**
         * Adds the provided double at the given index. All elements previously at the index and
         * to the right will be shifted accordingly. 
         * If the index is invalid, then nothing will be added/shifted. 
         * The size of this array is incremented by 1. 
         * If resizing the array is necessary, then that should be done. 
         * @arg to_add Object to be added to the array
         * @arg index Location to add the Object at
         **/
        void add(double to_add, size_t index) {
            DoubleObj* double_obj = new DoubleObj(to_add);
            double_arr->add(double_obj, index);
        }

        /**
         * Adds all the elements of the provided array at the given index, 
         * unless the given array is NULL, then it will not be added. Otherwise, 
         * all elements previously at the index and
         * to the right will be shifted accordingly by the size of the procided array, 
         * If the index is invalid, then nothing will be added/shifted.
         * Assuming a valid move, the size of this array is incremented by the size of the 
         * added array.  If resizing the array is necessary, then that should be done.
         * @arg to_add Array of Objects to be added to the array
         * @arg index Location to add the objects to the array at
         **/
        void addAll(DoubleArray* to_add, size_t index) {
            double_arr->addAll(to_add->double_arr, index);
        }

        /**
         * Returns the subarray starting from the provided index to the end of the array. 
         * If the index is invalid, then the method returns NULL
         * @arg index Starting place for the subarray
         **/
        DoubleArray* subArray(size_t index) {
            DoubleArray* returning = new DoubleArray();
            if (index > getSize()) {
                return nullptr;
            }
            while(index < getSize()) {
                returning->add(get(index));
                index += 1;
            }
            return returning;
        }

        /**
         * Returns the subarray starting from the provided index to the ending index
         * The starting index must always be greater than the ending index. If either index is invalid, 
         * then NULL is returned. The set is [start, end)
         * @arg startIndex starting place for the subarray
         * @arg endIndex location of the last object to be put in the subarray
         **/
        DoubleArray* subArray(size_t startIndex, size_t endIndex) {
            DoubleArray* returning = new DoubleArray();
            if (endIndex > getSize() || startIndex > endIndex) {
                return nullptr;
            }
            while(startIndex < getSize()) {
                returning->add(get(startIndex));
                startIndex += 1;
            }
            return returning;
        }

        /**
         * Removes the first instance of the given double in this array. If nothing 
         * is found, then no action will occur. The size reduces by 1 if the 
         * element is found.
         * @arg to_remove double to be removed from the array
         **/
        void remove(double to_remove) {
            DoubleObj* double_obj = new DoubleObj(to_remove);
            double_arr->remove(double_obj);
            delete double_obj;
        }

        /**
         * Removes all instances of the given double in this array. If nothing 
         * is found, then no action will occur. The size reduces the number of found
         * elements there are.
         * @arg to_remove int that all instances in the array will be removed of
         **/
        void removeAll(double to_remove) {
            DoubleObj* double_obj = new DoubleObj(to_remove);
            double_arr->removeAll(double_obj);
            delete double_obj;
        }

        /**
         * Returns number of elements in the array. 
         **/
        size_t getSize() {
            return double_arr->getSize();
        }

        /**
         * Overriding the Object equals method. 
         * Returns if all the elements in this array and the given object are equal and 
         * in the same other. 
         * If the given object is NULL or not an array, then false will be returned.
         * @arg other Object to check if this array is equal to
         **/
        bool equals(Object* other) {
            DoubleArray* o = dynamic_cast<DoubleArray*> (other);
            if (o)
                return o->double_arr->equals(this->double_arr);
            else {
                return false;
            }

        }

    /**
     * Overriding the Object hash_me() method. 
     * Hashes the array based on user specifications. Default implementation is
     * to hash all internal elements and sum them up. 
     **/
    size_t hash_me_() {
        return double_arr->hash_me_() + 3;
    }
};


class BoolArray : public Object {
    public:
        Array* bool_arr;

        /**
         * Default constructor which will set the initial max-capacity to the array to 10. 
         **/
        BoolArray() {
            bool_arr = new Array();
        }

        /**
         * Initalized this array with the characteristics of the passed in array.
         * @arg arr Array containing values to be used in initialization. 
         **/
        BoolArray(BoolArray* arr) {
            bool_arr = arr->bool_arr;
        }

        /**
         * Destructor which will free the memory of the underlying array as well. 
         **/
        ~BoolArray() {
            delete bool_arr;
        }

        /**
         * Will return the bool at the provided index. if the index is invalid, 
         * then the method will return -1
         * @arg index Location to get the value in the array at. 
         **/
        bool get(size_t index) {
            Object* obj = bool_arr->get(index); 
            if (obj) {
                return static_cast <BoolObj*> (obj)->getBool();
            }
            else {
                perror("attempted to get index out of bounds");
                exit(1);
            }
        }


        /**
         * Clears the array and reinitializes the underlying array to 10 spots with no elements. 
         * Reinitializes the size to 0. 
         **/
        void clear() {
            bool_arr->clear();
        }

        /**
         * Resizing the underlying array. And then copying over the elements to a new array with
         * the updated size. 
         * Default is doubling the array size when the max capacity of the 
         * underlying array less the number of elements is 2. 
         **/
        void resize() {
            bool_arr->resize();
        }

        /**
         * Returns the first index of the of given bool. 
         * If the object pointer is not found then -1 is returned.
         * @arg to_find Object to find the index of. 
         **/
        int indexOf(bool to_find) {
            BoolObj* to_find_obj = new BoolObj(to_find);
            int returning = bool_arr->indexOf(to_find_obj);
            delete to_find_obj;
            return returning;
        }

    /**
     * Adds the element provided to the end of the list.
     *  The size is incremented by 1. If resizing the array is necessary, 
     * then that should be done.
     * @arg to_add int to be added to the array. 
     **/
    void add(bool to_add) {
        BoolObj* bool_obj = new BoolObj(to_add);
        bool_arr->add(bool_obj);
    }

    /**
     * Adds the provided array to the end of the list, unless the given array is NULL, 
     * then it will not be added. 
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array. If resizing the array is necessary, then that should be done.
     * @arg to_add Array of Objects that all need to be added to this array. 
     **/
    void addAll(BoolArray* to_add) {
        bool_arr->addAll(to_add->bool_arr);
    }


    
    virtual void set(bool to_add, size_t index) {
        BoolObj* obj = new BoolObj(to_add);
        return bool_arr->set(obj, index);
    }   

    /**
     * Adds the provided bool at the given index. All elements previously at the index and
     * to the right will be shifted accordingly. 
     * If the index is invalid, then nothing will be added/shifted. 
     * The size of this array is incremented by 1. 
     * If resizing the array is necessary, then that should be done. 
     * @arg to_add Object to be added to the array
     * @arg index Location to add the Object at
     **/
    void add(bool to_add, size_t index) {
        BoolObj* bool_obj = new BoolObj(to_add);
        bool_arr->add(bool_obj, index);
    }

    /**
     * Adds all the elements of the provided array at the given index, 
     * unless the given array is NULL, then it will not be added. Otherwise, 
     * all elements previously at the index and
     * to the right will be shifted accordingly by the size of the procided array, 
     * If the index is invalid, then nothing will be added/shifted.
     * Assuming a valid move, the size of this array is incremented by the size of the 
     * added array.  If resizing the array is necessary, then that should be done.
     * @arg to_add Array of Objects to be added to the array
     * @arg index Location to add the objects to the array at
     **/
    void addAll(BoolArray* to_add, size_t index) {
        bool_arr->addAll(to_add->bool_arr, index);
    }

    /**
     * Returns the subarray starting from the provided index to the end of the array. 
     * If the index is invalid, then the method returns NULL
     * @arg index Starting place for the subarray
     **/
     BoolArray* subArray(size_t index) {
        BoolArray* returning = new BoolArray();
        if (index > getSize()) {
            return nullptr;
        }
        while(index < getSize()) {
            returning->add(get(index));
            index += 1;
        }
        return returning;
     }

    /**
     * Returns the subarray starting from the provided index to the ending index
     * The starting index must always be greater than the ending index. If either index is invalid, 
     * then NULL is returned. The set is [start, end)
     * @arg startIndex starting place for the subarray
     * @arg endIndex location of the last object to be put in the subarray
     **/
    BoolArray* subArray(size_t startIndex, size_t endIndex) {
        BoolArray* returning = new BoolArray();
        if (endIndex > getSize() || startIndex > endIndex) {
            return nullptr;
        }
        while(endIndex < getSize()) {
            returning->add(get(startIndex));
            startIndex += 1;
        }
        return returning;
    }

    /**
     * Removes the first instance of the given bool in this array. If nothing 
     * is found, then no action will occur. The size reduces by 1 if the 
     * element is found.
     * @arg to_remove double to be removed from the array
     **/
     void remove(bool to_remove) {
        BoolObj* bool_obj = new BoolObj(to_remove);
        bool_arr->remove(bool_obj);
        delete bool_obj;
     }

    /**
     * Removes all instances of the given bool in this array. If nothing 
     * is found, then no action will occur. The size reduces the number of found
     * elements there are.
     * @arg to_remove int that all instances in the array will be removed of
     **/
     void removeAll(bool to_remove) {
        BoolObj* bool_obj = new BoolObj(to_remove);
        bool_arr->removeAll(bool_obj);
        delete bool_obj;
     }

    /**
     * Returns number of elements in the array. 
     **/
    size_t getSize() {
        return bool_arr->getSize();
    }

    /**
     * Overriding the Object equals method. 
     * Returns if all the elements in this array and the given object are equal and 
     * in the same other. 
     * If the given object is NULL or not an array, then false will be returned.
     * @arg other Object to check if this array is equal to
     **/
    bool equals(Object* other) {
        BoolArray* o = dynamic_cast<BoolArray*> (other);
        if (o)
            return o->bool_arr->equals(this->bool_arr);
        else {
            return false;
        }

    }

    /**
     * Overriding the Object hash_me() method. 
     * Hashes the array based on user specifications. Default implementation is
     * to hash all internal elements and sum them up. 
     **/
    size_t hash_me_() {
        return bool_arr->hash_me_() + 4;
    }
};
