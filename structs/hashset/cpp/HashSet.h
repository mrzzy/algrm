#ifndef __HASHSET_H_
#define __HASHSET_H_
/*
 * np dsa assignment
 * hash table set
*/


#include <iostream>
#include "structs/List.h"

namespace structs
{
    template <typename T> class HashSet
    {
        private:
            List<T>* table;
            uint32_t nAllocated;
            uint32_t nStored;
            float loadFactor;
            
            void resize(float factor);

        public:
            /* constructor/destructor */
            HashSet(uint32_t size=16, float loadFactor=0.75); // preallocate size, load factor
            HashSet(const HashSet& hs); // copy constructor
            ~HashSet();
        
            /* hashmap attributes */
            uint32_t length() const { return  this->nStored; }
            // returns true if the list is empty
            bool empty() const { return this->length() == 0; }
            // get the values in the hashset
            List<T> values() const;
            // check whether set has value
            bool has(const T& value) const;
            // returns a string representation the hashset 
            operator std::string() const;
            // convience convert to list: same as values()
            operator List<T>() const { return this->values(); }

            /* hashmap operations */
            // add the given value to the set
            // if set also has value, does nothing
            void add(T value);

            // remove the given value from the given set
            // throws exception if value not in set
            void remove(T value);
    };

    template <typename T> std::ostream& operator << (std::ostream &stream, const HashSet<T>& hs);
    
}

// require for templates to work properly
#include "structs/HashSet.cpp"

#endif /* end of include guard: __HASHSET_H_ */
