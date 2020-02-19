#ifndef __ITERATOR_H__
#define  __ITERATOR_H__
/*
 * np dsa assignment 
 * iterator interface
 * by: Zhu Zhanyan S1017047
*/

namespace structs
{
    // abstract class defining an interface to iterators
    template <typename T> class Iterator
    {
        public:
            // move iterator to next element
            // throws exception if iterator is empty
            virtual void next() = 0;
            // check iterator still has any elements to iterate
            virtual bool empty() = 0;
                
            // get the item that the iterator current points at
            // throws exception if iterator is empty
            virtual T& item() = 0;
            
            // convience operators: item()
            T& operator * () { return this->item(); }
            T* operator -> ()  { return &this->item(); }
            
    };

    // abstract class defining an interface to bidirectional iterator
    template <typename T> class TwoWayIterator: public Iterator<T>
    {
        public:
            virtual void prev() = 0;
    };
}



#endif /* end of include guard:  __ITERATOR_H__ */
