#ifndef _VECTOR_H_
#define _VECTOR_H_
/*
 * np dsa assignment
 * vector - resizable array data structure
 * by: Zhu Zhanyan S1017047
*/

#include <exception>
#include <iostream>

#include "Iterator.h"

namespace structs
{
    // forward declare list 
    template <typename T> class List;

    template <typename T> class Vector
    {
        public:
            /* constructor/destructor */
            Vector(uint32_t size=0, T value=T()); // preallocate size & value
            Vector(const Vector& v); // copy constructor
            // type conversions
            Vector(const List<T>& l); 
            ~Vector();
            
            /* iterator */
            class VectorIterator: public TwoWayIterator<T>
            {
                public:
                    VectorIterator(T *items, uint32_t size, uint32_t pos):
                        items(items), size(size), currentIdx(pos) {};
                    void next();
                    void prev();
                    bool empty() 
                    { return this->currentIdx < 0 || this->currentIdx >= this->size; }
                    T& item();
                private:
                    T* items;
                    int64_t currentIdx;
                    uint32_t size;
            };

            // return an iterator to the start the vector
            VectorIterator begin() const 
            { return VectorIterator(this->items, this->nStored, 0); }
            // return an iterator to the end the vector
            VectorIterator end() const
            { return VectorIterator(this->items, this->nStored, this->nStored-1); }


            /* vector attributes */
            // returns the length of the vector
            uint32_t length() const { return this->nStored; }
        
            // returns true if the vector is empty
            bool empty() const { return this->length() == 0; }
            
            // returns a string representation the vector
            operator std::string() const;
        
            /* vector operations */
            // append an item to the end of the vector
            // returns the index of the item in the vector
            uint32_t append(T item);
            
            // remove the item at the end of the vector 
            // returns the poped item
            T popEnd();
            
            // get/set the item at the given index idx
            // throws exception if idx not an index in the the vector
            // can be used to reassign item
            T& operator[](uint32_t idx);

        private:
            T* items; // array used to store items
            uint32_t nStored; // no. of items stored in the array
            uint32_t nAllocated; // no. of items that the vector has allocated space for
            void upsize();
            void downsize();
        
    };
    
    template <typename T> std::ostream& operator << (std::ostream &stream, const Vector<T>& l);
}


// required for template to work properly
#include "Vector.cpp"

#endif /* end of include guard: _VECTOR_H_ */
