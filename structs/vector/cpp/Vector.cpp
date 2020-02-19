#ifndef _VECTOR_CPP_
#define _VECTOR_CPP_
/*
 * np dsa assignment
 * vector - resizable array data structure
 * by: Zhu Zhanyan S1017047
*/

#include <string>
#include <sstream>

#include "algorithms/string.h"
#include "algorithms/functional.h"

#include "Vector.h"
#include "structs/List.h"

using namespace std;
using namespace algo;

namespace structs
{
    /* constructor/destructor */
    template <typename T> Vector<T>::Vector(uint32_t size, T value):
        items(nullptr), nStored(0), nAllocated(0)
    {
        // preallocate size & init with value
        this->items = new T[size];
        for(uint32_t i = 0; i < size; i ++)
        {
            this->items[i] = value;
        }
        this->nAllocated = size;
        this->nStored = size;
    }
    
    // type conversions
    template <typename T> Vector<T>::Vector(const List<T>& l)
        : Vector()
    {
        for(auto iter = l.begin(); !iter.empty(); iter.next())
        {
            this->append(*iter);
        }
    }
    
    // copy constructor 
    template <typename T> Vector<T>::Vector(const Vector& v)
        : Vector()
    {
        for(auto iter = v.begin(); !iter.empty(); iter.next())
        {
            this->append(*iter);
        }
    }

    template <typename T> Vector<T>::~Vector()
    {
        if(this->items) delete[] this->items;
    }
    
    /* vector  attributes */
    template <typename T> Vector<T>::operator std::string() const 
    {
        ostringstream strStream;
        strStream << "[" << endl;
        Vector<string> strValues = apply<Vector<string>, Vector<T>>(*this, [](T item) 
        {
            ostringstream strStream;
            strStream << "  " << item;
            return strStream.str();
        });
        strStream << join<Vector<string>>(strValues, ",\r\n");

        strStream << endl << "]" << endl;
        
        return strStream.str();
    }

    /*  vector operators */
    // double the capacity of the vector
    template <typename T> void Vector<T>::upsize()
    {
        // allocate buffer with larger size to fit items
        uint32_t newSize = this->nAllocated * 2;
        newSize = (newSize > 0) ? newSize : 1;
    
        T *newItems = new T[newSize];

        // copy items over to new array
        for(uint32_t i = 0; i < this->nStored; i ++)
        {
            newItems[i] = this->items[i];
        }
        delete[] this->items;
        this->items = newItems;

        this->nAllocated = newSize;
    }
    
        
    // append an item to the end of the vector 
    template <typename T> uint32_t Vector<T>::append(T item)
    {
        // check vector capacity
        if(this->nAllocated < (this->nStored + 1)) this->upsize();
    
        uint32_t idx = this->nStored;
        this->items[idx] = item;
        this->nStored ++;
        
        return idx;
    }
    
    // half the capacity of the vector
    // throws an exception of if operation would destroy data
    template <typename T> void Vector<T>::downsize()
    {
        if(this->nAllocated / 2 < this->nStored)
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Downsize will destroy data";
            throw logic_error(strStream.str());
        }
    
        // allocate buffer with smaller size to fit items
        uint32_t newSize = this->nAllocated / 2;
        newSize = (newSize > 0) ? newSize : 1;
        T *newItems = new T[newSize];

        // copy items over to new array
        for(uint32_t i = 0; i < this->nStored; i ++)
        {
            newItems[i] = this->items[i];
        }
        delete[] this->items;
        this->items = newItems;

        this->nAllocated = newSize;
    }
    
    // pop an item from the end of the vector 
    template <typename T> T Vector<T>::popEnd()
    {
        // remove last element
        T item = this->items[this->nStored - 1];
        this->nStored --;
        
        // downsize if less then half used
        if(this->nAllocated / 2 > this->nStored) this->downsize();

        return item;
    }
    
    // get the item at the given path 
    template <typename T> T& Vector<T>::operator [](uint32_t idx) 
    {
        // check idx within bounds
        if(idx >= this->nStored)
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Index " << idx << " Out of Bounds";
            throw (strStream.str());
        }
    
        return this->items[idx];
    }
    
    // support for output streams
    template <typename T> ostream& operator << (ostream& stream, const Vector<T>& v)
    {
        stream << (string)v;
        return stream;
    }

    /* iterators */
    // advance iterator
    template <typename T> void Vector<T>::VectorIterator::next() 
    {
        if(this->empty()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "next() called on empty empty iterator";
            throw out_of_range(strStream.str());
        }
        this->currentIdx ++;
    }
    
    // retreat iterator
    template <typename T> void Vector<T>::VectorIterator::prev() 
    {
        if(this->empty()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "prev() called on empty empty iterator";
            throw out_of_range(strStream.str());
        }
        this->currentIdx --;
    }

    
    // get iterator item
    template <typename T> T& Vector<T>::VectorIterator::item() 
    {
        if(this->empty()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "prev() called on empty empty iterator";
            throw out_of_range(strStream.str());
        }

        return this->items[this->currentIdx];
    }
}



#endif /* end of include guard: _VECTOR_CPP_ */
