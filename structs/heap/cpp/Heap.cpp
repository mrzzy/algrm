#ifndef _STRUCT_HEAP_CPP_
#define _STRUCT_HEAP_CPP_
/*
 * np dsa assignment
 * heap data structure
 * by: Zhu Zhanyan S1017047
*/

#include "Heap.h"

using namespace std;

namespace structs
{
    /* constrctors/destructors */
    template <typename T> Heap<T>::Heap(const Heap& h)
    {
        for(auto itemIter = h.items.begin(); !itemIter.empty(); itemIter.next())
        {
            this->items.append(*itemIter);
        }
    }
        
    /* utilities */
    // return true in min heap mode if first element is greater than second element 
    // return true in max heap mode if first element is smaller than second element
    template <typename T> bool Heap<T>::compare(uint32_t firstIdx, uint32_t secondIdx)
    {
        if(this->minHeap) { return this->items[firstIdx] > this->items[secondIdx]; }
        else { return this->items[firstIdx] < this->items[secondIdx]; }
    }
    
    // swap values
    template <typename T> void Heap<T>::swap(uint32_t firstIdx, uint32_t secondIdx)
    {
        T temp = this->items[secondIdx];
        this->items[secondIdx] = this->items[firstIdx];
        this->items[firstIdx] = temp;
        
        // update key index maps 
        uint32_t firstKey = this->keyMap.get(firstIdx);
        uint32_t secondKey = this->keyMap.get(secondIdx);
        this->keyMap[firstIdx] = secondKey;
        this->keyMap[secondIdx] = firstKey;
        this->idxMap[firstKey] = secondIdx;
        this->idxMap[secondKey] = firstIdx;
    }

    // propagate the given idx,restoring the heap property
    template <typename T> uint32_t Heap<T>::propagateDown(uint32_t idx)
    {
        uint32_t parentIdx = idx;
        // check if left child is smaller/larger
        if(this->leftChild(idx) < this->length() 
                && this->compare(parentIdx, this->leftChild(idx)))
        {
            parentIdx = this->leftChild(idx);
        }
        // check if right child is smaller/larger
        if(this->rightChild(idx) < this->length() 
                && this->compare(parentIdx, this->rightChild(idx)))
        {
            parentIdx = this->rightChild(idx);
        }

        // swap with parent idx if smaller/larger
        if(parentIdx != idx)
        {
            this->swap(idx, parentIdx);
            idx = parentIdx;
            // recursively propagate down 
            return this->propagateDown(idx);
        }
        
        return idx;
    }
    
    // propagate up the given idx, restoring the heap property
    template <typename T> uint32_t Heap<T>::propagateUp(uint32_t idx)
    {
        // stop: root node: no way to continue propagate up
        if(idx <= 0) return idx;
        if(!this->compare(idx, this->parent(idx)))
        {
            // swap with parent idx
            this->swap(idx, this->parent(idx));
            idx = this->parent(idx);
        
            // recursively propagate up
            return this->propagateUp(idx);
        }

        return idx;
    }
    
    template <typename T> Heap<T>::operator string() const
    {
        return (string) this->items;
    }
    
    
    /* heap operations */
    template <typename T> T Heap<T>::remove(uint32_t key)
    {
        uint32_t swapIdx = this->idxMap.get(key);
        // swap item with last item to remove item
        this->swap(swapIdx, this->length() - 1);
        
        // remove from key and idx map
        key = this->keyMap[this->length() - 1];
        uint32_t idx = this->idxMap.get(key);
        this->idxMap.remove(key);
        this->keyMap.remove(idx);
    
        // remove item
        T popItem = this->items.popEnd();

        // move swapped last item back into position
        this->propagateDown(swapIdx);
        
        return popItem;
    }
    
    template <typename T> T Heap<T>::popTop()
    {
        return this->remove(this->keyMap[0]);
    }

    template <typename T> uint32_t Heap<T>::add(T item)
    {
        // create idx key mapping for the item so that we can find it again
        uint32_t key = this->nKey;
        uint32_t idx = this->length();
        this->idxMap[key] = idx;
        this->keyMap[idx] = key;
        this->nKey ++;

        // add item
        idx = this->items.append(item);
        // move new item into place
        idx = this->propagateUp(idx);
        
        return key;
    }
    
    template <typename T> std::ostream& operator << (std::ostream &stream, const Heap<T>& h)
    {
        stream << (string) h;
        return stream;
    }
}

#endif /* end of include guard: _STRUCT_HEAP_CPP_ */
