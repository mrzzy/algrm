#ifndef _LIST_H_
#define _LIST_H_
/*
 * np dsa assignment
 * doublely linked list data structure
 * by: Zhu Zhanyan S1017047
*/

#include <exception>
#include <iostream>

#include "structs/Iterator.h"

namespace structs
{
    // forward declare
    template <typename T> class Vector;
    
    template <typename T> class List 
    {
        public:
            struct Node
            {
                T item;
                Node *prev;
                Node *next;
            };

            /* constructor/destructor */
            List(uint32_t size=0, T value=T()); // preallocate size & value
            List(const List &l); // copy constrctor
            // conversion constructors
            List(const Vector<T>& vector);
            ~List();

            /* list attributes */
            // returns the length of the list
            uint32_t length() const { return this->size; }
            // returns true if the list is empty
            bool empty() const { return this->length() == 0; }
            // returns a string representation the list
            operator std::string() const;
        
            /* iterators */
            class ListIterator: public TwoWayIterator<T> 
            {
                public:
                    ListIterator(Node *startNode) : currentNode(startNode) {};
                    void next();
                    void prev();
                    bool empty() { return this->currentNode == nullptr; }
                    T& item();
                private:
                    Node* currentNode;
            };

        
            // returns an iterator to the start of the list
            ListIterator begin() const { return ListIterator(this->beginNode); }
            // returns an iterator to the end of the list
            ListIterator end() const { return ListIterator(this->endNode); }

            /* list operations */
            // append an item to the end of the list
            // returns the index of the item in the list
            uint32_t append(T item);
            
            // remove the item at the end of the list 
            // returns the poped item
            T popEnd();

            // push an item to the start of the list 
            // returns the index of the item in the list
            void push(T item);
            
            // pop an item from the start of the list
            // returns the poped item in the list
            T pop();
            
            // insert the item at the given index idx
            // throws exception if idx not an index in the the list
            // returns the removed item
            void insert(uint32_t idx, T item);
            
            // remove the item at the given index idx
            // throws exception if idx not an index in the the list
            // returns the removed item
            T remove(uint32_t idx);
            
            // get/set the item at the given index idx
            // throws exception if idx not an index in the the list
            // can be used to reassign item
            T& operator[](uint32_t idx);

        private:
            Node *beginNode; // begin node
            Node *endNode; // last node
            uint32_t size; // no. of nodes in the list 
            
    };
    
    template <typename T> std::ostream& operator << (std::ostream &stream, const List<T>& l);
    
}

// required for template to work properly
#include "List.cpp"

#endif /* end of include guard: _LIST_H_ */
