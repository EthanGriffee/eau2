#pragma once
#include "object.h"
#include <math.h> 
#include <limits>
#include <type_traits>

template <typename IntegralType>
typename std::enable_if<std::is_integral<IntegralType>::value, bool>::type
equal(const IntegralType& a, const IntegralType& b) {
        return a == b;
}

template <typename FloatingType>
typename std::enable_if<std::is_floating_point<FloatingType>::value, bool>::type
equal(const FloatingType& a, const FloatingType& b) {
        return fabs(a-b) < std::numeric_limits<FloatingType>::epsilon();
}

bool equal(Object* a, Object* b) {
        return a->equals(b);
}



template <class arrayClass>
class Array {
public:
    arrayClass* array;
    size_t size;
    size_t max_capacity;

    /**
     * Default constructor which will set the initial max-capacity to the array to 10. 
     **/
    Array() {
        max_capacity = 10;
        size = 0;
        array = new arrayClass[max_capacity];
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
        if (index > size) {
            exit(1);
        }
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
     * @arg to_add Array of Objects that all need to be added to this array. 
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
     * @arg to_add Object to be added to the array
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
     * @arg to_add Array of Objects to be added to the array
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