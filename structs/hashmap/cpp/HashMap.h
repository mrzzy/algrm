#ifndef _HASHMAP_
#define _HASHMAP_
/*
 * np dsa assignment
 * hash table map 
 * by: Zhu Zhanyan S1017047
*/

#include <iostream>
#include "Vector.h"
#include "List.h"

namespace structs
{
    template <typename K, typename V> class HashMap
    {
        private:
            struct Node 
            {
                K key;
                V value;
            };
        
            List<Node>* table;
            uint32_t nAllocated;
            uint32_t nStored;
            float loadFactor;
            
            void resize(float factor);
        public:
            /* constructor/destructor */
            HashMap(uint32_t size=16, float loadFactor=0.75); // preallocate size, load factor
            HashMap(const HashMap& hm); // copy constructor
            ~HashMap();
        
            /* hashmap attributes */
            uint32_t length() const { return  this->nStored; }
            // returns true if the list is empty
            bool empty() const { return this->length() == 0; }
            // get the keys in the hashmap
            List<K> keys() const;
            // get the values in the hashmap
            List<V> values() const;
            // returns a string representation the list
            operator std::string() const;
            // check whether hash map has given key
            bool has(K key) const;

            /* hashmap operations */
            // get the value for the given key
            // throws exception on get key that does not exist
            // returns the value 
            V& get(K key) const;
            
            // store the given key value pair in the hash table
            void set(K Key, V value);
        
            // remove key value pair  for the given key
            void remove(K key);
        
            // get/set the item with the given key
            // creates a new mapping in at with the given key if does not exist
            // can be used to reassign or create new mapping
            V& operator[](K key);
    };

    template <typename K, typename V> std::ostream& operator << (std::ostream &stream, const HashMap<K, V>& l);
}

// required for templates to work properly
#include "HashMap.cpp"

#endif /* end of include guard: _HASHMAP_ */
