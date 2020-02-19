#ifndef __HASHSET_CPP_
#define __HASHSET_CPP_
/*
 * np dsa assignment
 * hash table set
 * by: Zhu Zhanyan S1017047
*/

#include <iostream>
#include <string>
#include <sstream>

#include "HashSet.h"
#include "algorithms/hashing.h"

using namespace std;

namespace structs
{
    /* constructor/destructor */
    // preallocate size, load factor
    template <typename T> HashSet<T>::HashSet(uint32_t size, float loadFactor)
        : table(nullptr), nAllocated(0), nStored(0), loadFactor(loadFactor)
    {
        // check hashmap size
        size = (size > 0) ? size : 16;
        this->table = new List<T>[size];
        this->nAllocated = size;
    }

    // copy constructor 
    template <typename T> HashSet<T>::HashSet(const HashSet& hm)
        : HashSet(hm.length())
    {
        List<T> values = hm.values();
        for(auto valueIter = values.begin(); !valueIter.empty(); valueIter.next())
        {
            this->add(*valueIter);
        }
    }

    // desctructor
    template <typename T> HashSet<T>::~HashSet()
    {
        delete [] this->table;
    }

    /* hashmap attributes */
    template <typename T> HashSet<T>::operator string() const
    {
        ostringstream strStream;
        List<T> values = this->values();
        
        strStream << "(" << endl;
        List<string> strValues = algo::apply<List<string>, List<T>>(values, [](T value)
        {
            ostringstream strStream;
            strStream <<  "  " << value;
            return strStream.str();
        });
        strStream << algo::join<List<T>>(strValues, ",\r\n") << endl;
        strStream << ")" << endl;
        
        return strStream.str();
    }

    // returns keys of the hashmap as a list 
    template <typename T> List<T> HashSet<T>::values() const
    {
        List<T> values;
        for(uint32_t i = 0; i < this->nAllocated; i++)
        {
            List<T>& bucket = this->table[i];
            for(auto valueIter = bucket.begin(); !valueIter.empty(); valueIter.next())
            {
                values.append(*valueIter);
            }
        }

        return values;
    }
    
    // check whether set has value
    template <typename T> bool HashSet<T>::has(const T &value) const 
    {
        for(uint32_t i = 0; i < this->nAllocated; i++)
        {
            List<T>& bucket = this->table[i];
            for(auto valueIter = bucket.begin(); !valueIter.empty(); valueIter.next())
            {
                if(*valueIter == value) return true;
            }
        }
    
        return false;
    }
    
    /* hashmap operations */
    template <typename T> void HashSet<T>::resize(float factor) { 
        // collect values
        List<T> values = this->values();
    
        // reallocate hash table with updated size
        uint32_t newSize = this->nAllocated * factor;
        newSize = (newSize > 0) ? newSize : 1;
        delete [] this->table;
        this->table = new List<T>[newSize];
        this->nAllocated = newSize;
        
        // repopulate hash with key values
        auto valueIter = values.begin();
        for(uint32_t i = 0; i < this->nStored; i++)
        {
            T& value = *valueIter;
            uint32_t idx = algo::hash<T>(value) % this->nAllocated;
            List<T>& bucket = this->table[idx];
            bucket.append(value);

            // advance iterator
            valueIter.next();
        }
    }

    // add the given value to the set
    // if set also has value, does nothing
    template <typename T> void HashSet<T>::add(T value)
    {
        if(this->has(value)) return;
       
        // check whether we need to resize the hash table
        if((float)this->nStored / (float)this->nAllocated > this->loadFactor) {
            this->resize(2.0); }

        // add item to set
        uint32_t idx = algo::hash<T>(value) % this->nAllocated;
        List<T>& bucket = this->table[idx];
        bucket.append(value);
        this->nStored ++;
    }
    
    // remove the given value from the given set
    // throws exception if value not in set
    template <typename T> void HashSet<T>::remove(T value)
    {
        if(!this->has(value))
        {
            // not found: throw exception
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Attempted to remove non existent value " << value;
            strStream << " from HashSet";
            throw out_of_range(strStream.str());
        }
        
        // search bucket for node with key
        uint32_t idx = algo::hash<T>(value) % this->nAllocated;
        List<T>& bucket = this->table[idx];
        auto valueIter = bucket.begin();
        uint32_t removeIdx = UINT32_MAX;
        for(uint32_t i = 0; i < bucket.length(); i++)
        {
            if(*valueIter == value)
            {
                removeIdx = i;
                break;
            }
        
            // advance iterator
            valueIter.next();
        }
        
        // perform remove
        bucket.remove(removeIdx);
        this->nStored --;
        
        // check if we need to resize the hash table to fit
        if(this->nStored < this->nAllocated / 2)
        { this->resize(0.5); }
    }

    template <typename T> std::ostream& operator << (std::ostream &stream, const HashSet<T>& hs)
    {
        stream << (string)hs;
        return stream;
    }
}


#endif /* end of include guard: __HASHSET_CPP_ */
