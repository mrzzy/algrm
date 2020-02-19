#ifndef _STRUCT_HEAP_H_
#define _STRUCT_HEAP_H_
/*
 * np dsa assignment
 * heap data structure
 * by: Zhu Zhanyan S1017047
*/

#include <iostream>

#include "structs/Vector.h"
#include "structs/HashMap.h"

namespace structs
{
    template <typename T> class Heap
    {
        private:
            Vector<T> items;
        
            /* utilities */
            uint32_t parent(uint32_t idx) 
            { return (idx - 1) / 2;}
            uint32_t leftChild(uint32_t idx)
            { return (idx * 2) + 1; }
            uint32_t rightChild(uint32_t idx)
            { return (idx * 2) + 2; }
            uint32_t propagateDown(uint32_t idx);
            uint32_t propagateUp(uint32_t idx);
            void swap(uint32_t firstIdx, uint32_t rightIdx);
            bool compare(uint32_t firstIdx, uint32_t secondIdx);

            bool minHeap; // whether operating in min heap mode, otherwise max heap mode
            // maps key to index
            HashMap<uint32_t, uint32_t> idxMap;
            // maps index to key
            HashMap<uint32_t, uint32_t> keyMap;
            uint32_t nKey;
        public:
            /* constructors/destructor */
            // construct heap
            // minHeap - whether to operate in min heap mode, or max heap mode
            Heap(bool minHeap=true) : minHeap(minHeap), nKey(0) {};
            Heap(const Heap& h);
            
            /* heap attributes */ 
            // returns the length of the list
            uint32_t length() const { return this->items.length(); }
            // returns true if the list is empty
            bool empty() const { return this->items.length() == 0; }
            // returns top (min/max) item in the heap 
            T& top() { return this->items[0]; }
            // returns a string representation of the heap
            operator std::string() const;
            // returns the values in the heap
            Vector<T> values() const { return this->items; }
            
            T& get(int key) { return this->items[this->idxMap.get(key)]; }
            
            /* heap operations */
            // remove item for the given key
            T remove(uint32_t key);

            // remove minimum/maximum item from the heap
            // returns the poped item
            T popTop();
            // add an item to the heap
            // returns an unique key for the item
            uint32_t add(T item);
    };

    template <typename T> std::ostream& operator << (std::ostream &stream, const Heap<T>& h);

}

// required for heap to work properly
#include "Heap.cpp"

#endif /* end of include guard: _STRUCT_HEAP_H_ */
