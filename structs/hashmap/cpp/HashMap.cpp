#ifndef _HASHMAP_CPP_
#define _HASHMAP_CPP_
/*
 * np dsa assignment
 * hash table map 
 * by: Zhu Zhanyan S1017047
*/

#include <exception>
#include <string>
#include <sstream>

#include "algorithms/hashing.h"
#include "HashMap.h"


using namespace std;
using namespace algo;

namespace structs 
{
    /* constructor/destructor */
    // preallocate space for hashtable
    template <typename K, typename V> HashMap<K, V>::HashMap(uint32_t size, float loadFactor)
        : table(nullptr), nAllocated(0), nStored(0), loadFactor(loadFactor)
    {
        // check hashmap size
        size = (size > 0) ? size : 16;
    
        this->table = new List<Node>[size];
        this->nAllocated = size;
    }
    
    // copy constructor
    template <typename K, typename V> HashMap<K, V>::HashMap(const HashMap& hm)
        : HashMap(hm.length())
    {
        List<K> keys = hm.keys();
        for(auto keyIter = keys.begin(); !keyIter.empty(); keyIter.next())
        {
            K key = *keyIter;
            this->set(key, hm.get(key));
        }
    }

    template <typename K, typename V> HashMap<K, V>::~HashMap()
    {
        delete[] this->table;
    }

    
    /* hashmap attributes */
    // returns keys of the hashmap as a vector
    template <typename K, typename V> List<K> HashMap<K, V>::keys() const
    {
        List<K> keys;
        for(uint32_t i = 0; i < this->nAllocated; i++)
        {
            List<Node>& bucket = this->table[i];
            for(auto nodePtr = bucket.begin(); !nodePtr.empty(); nodePtr.next())
            {
                keys.append(nodePtr->key);
            }
        }
        
        return keys;
    }
    
    // returns values of the hashmap as a vector
    template <typename K, typename V> List<V> HashMap<K, V>::values() const
    {
        List<V> values;
        for(uint32_t i = 0; i < this->nAllocated; i++)
        {
            List<Node>& bucket = this->table[i];
            for(auto nodePtr = bucket.begin(); !nodePtr.empty(); nodePtr.next())
            {
                values.append(nodePtr->value);
            }
        }
        
        return values;
    }
    
    // string representation of hashmap
    template <typename K, typename V> HashMap<K, V>::operator string() const
    {
        ostringstream strStream;
        // collect keys and values
        List<K> keys = this->keys();
        List<V> values = this->values();
        
        strStream << "{" << endl;
        // combine key and value with newlew
        auto keyIter = keys.begin();
        auto valueIter = values.begin();
        List<string> strPairs;
        for(uint32_t i = 0; i < this->nStored; i++)
        {
            ostringstream pairStream;
            pairStream << "  " <<  *keyIter << ": " << *valueIter;
            strPairs.append(pairStream.str());

            // advance iterators
            keyIter.next();
            valueIter.next();
        }
        strStream << join<List<string>>(strPairs, ",\r\n") << endl;
        strStream << "}" << endl;
        
        return strStream.str();
    }
    
    /* hashmap operations */
    template <typename K, typename V> void HashMap<K, V>::resize(float factor) {
        // collect keys and values
        List<K> keys = this->keys();
        List<V> values = this->values();
    
        // reallocate hash table with new size to fit items
        uint32_t newSize = this->nAllocated * factor;
        newSize = (newSize > 0) ? newSize : 1;
        delete [] this->table;
        this->table = new List<Node>[newSize];
        this->nAllocated = newSize;
        
        // repopulate hash with key values
        auto keyIter = keys.begin();
        auto valueIter = values.begin();
        for(uint32_t i = 0; i < this->nStored; i++)
        {
            K& key = *keyIter;
            V& value = *valueIter;
            uint32_t idx = algo::hash<K>(key) % this->nAllocated;
            Node node = Node{ key, value };
            List<Node>& bucket = this->table[idx];
            bucket.append(node);

            // advance iterators
            keyIter.next();
            valueIter.next();
        }
    }
    
    // store the given key value pair in the hash table
    template <typename K, typename V> void HashMap<K, V>::set(K key, V value)
    {
        // check whether we need to resize the hash table
        if((float)this->nStored / (float)this->nAllocated > this->loadFactor)
        { this->resize(2.0); }
    
        // store item in hash table
        uint32_t idx = algo::hash<K>(key) % this->nAllocated;
        Node node = Node{ key, value };
        List<Node>& bucket = this->table[idx];
        bucket.append(node);
        
        
        this->nStored ++;
    }

    template <typename K, typename V> bool HashMap<K, V>::has(K key) const
    {
        for(uint32_t i = 0; i < this->nAllocated; i++)
        {
            List<Node> bucket = this->table[i];
            for(auto nodePtr = bucket.begin(); !nodePtr.empty(); nodePtr.next())
            {
                if(key == nodePtr->key) return true;
            }
        }
        return false;
    }
    
    // get the value forthe given key
    template <typename K, typename V> V& HashMap<K, V>::get(K key) const
    {
        if(!this->has(key))
        {
            // not found: throw exception
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Attempted to get non existent key " << key;
            strStream << " from hashmap";
            throw out_of_range(strStream.str());
        }

        // search bucket for node with key
        uint32_t idx = algo::hash<K>(key) % this->nAllocated;
        List<Node>& bucket = this->table[idx];
        for(auto nodePtr = bucket.begin(); !nodePtr.empty(); nodePtr.next())
        {
            if(key == nodePtr->key) return nodePtr->value;
        }
    }
    
    // remove key value pair  for the given key
    template <typename K, typename V> void HashMap<K, V>::remove(K key)
    {
        if(!this->has(key))
        {
            // not found: throw exception
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Attempted to remove non existent key " << key;
            strStream << " from hashmap";
            throw out_of_range(strStream.str());
        }

        // search bucket for node with key
        uint32_t idx = algo::hash<K>(key) % this->nAllocated;
        List<Node>& bucket = this->table[idx];
        auto nodePtr = bucket.begin();
        uint32_t removeIdx = UINT32_MAX;
        for(uint32_t i = 0; i < bucket.length(); i++)
        {
            if(nodePtr->key == key)
            {
                removeIdx = i;
                break;
            }
        
            // advance iterator
            nodePtr.next();
        }
        
        // perform remove
        bucket.remove(removeIdx);
        this->nStored --;
        
        // check if we need to resize the hash table to fit
        if(this->nStored < this->nAllocated / 2)
        { this->resize(0.5); }
    }

    // get/set key value pair
    template <typename K, typename V> V& HashMap<K, V>::operator [] (K key)
    {
        if(this->has(key))
        { return this->get(key); }
        else
        {
            this->set(key, V());
            return this->get(key);
        }
    }
    
    template <typename K, typename V> ostream& operator << (ostream &stream, const HashMap<K, V>& hm)
    {
        stream << (string) hm;
        return stream;
    }
    
}


#endif /* end of include guard: _HASHMAP_CPP_ */
