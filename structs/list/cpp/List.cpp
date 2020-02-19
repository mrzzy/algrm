#ifndef _LIST_CPP_
#define _LIST_CPP_
/*
 * np dsa assignment
 * doublely linked list data structure
 * by: Zhu Zhanyan S1017047
*/

#include <sstream>
#include <string>

#include "structs/List.h"
#include "Vector.h"

#include "algorithms/string.h"
#include "algorithms/functional.h"

using namespace std;
using namespace algo;


namespace structs 
{
    using namespace std;

    /* constructor/desctructor */
    // preallocate list the given size & value
    template <typename T> List<T>::List(uint32_t size, T value) :
        size(0), beginNode(nullptr), endNode(nullptr)
    {
        // allocate to specified size
        Node *prevNode = nullptr;
        Node *node = nullptr;
        for(uint32_t i = 0; i < size; i++) 
        {
            node = new Node { value, prevNode, nullptr };
            // edge case: first node becomes begin node
            if(i == 0) this->beginNode = node;
            // link prev node to next node
            if(prevNode) prevNode->next = node;
            prevNode = node;
            this->size ++;
        }
        this->endNode = node;
    }

    // conversion constructor
    template <typename T> List<T>::List(const Vector<T>& v) : List()
    {
        for(auto iter = v.begin(); !iter.empty(); iter.next())
        {
            this->append(*iter);
        }
    }
    
    // copy constrctor 
    template <typename T> List<T>::List(const List& l) : List()
    {
        for(auto iter = l.begin(); !iter.empty(); iter.next())
        {
            this->append(*iter);
        }
    }
    
    // destroy list
    template <typename T> List<T>::~List() 
    {
        Node* removeNode  = this->beginNode;
        for(uint32_t i = 0; i < this->length(); i++)
        {
            Node* nextNode = removeNode->next;
            delete removeNode;
            removeNode = nextNode;
        }
    }

    /* list attributes */
    // return size of list
    template <typename T> List<T>::operator string() const
    {
        ostringstream strStream;
        strStream << "[" << endl;

        List<string> strValues = apply<List<string>, List<T>>(*this, [](T item) 
        {
            ostringstream strStream;
            strStream << "  " << item;
            return strStream.str();
        });
        strStream << join<List<string>>(strValues, ",\r\n");

        strStream << endl << "]" << endl;
        
        return strStream.str();
    }
    
    /* list operations */
    // add element to the end of the list 
    template <typename T> uint32_t List<T>::append(T item)
    {
        Node *newNode = new Node { item, this->endNode, nullptr };
        
        // link node with the rest of the list
        if(this->endNode) this->endNode->next = newNode;
        
        // update ptrs
        this->endNode = newNode;
        // edge case: first node in list becomes begin node
        if(!newNode->prev) this->beginNode = newNode;
        this->size ++;

        return this->size - 1; //0 index
    }

    // remove element at the end of list and return it
    template <typename T> T List<T>::popEnd()
    {
        // check op valid 
        if(this->size < 1) 
            throw out_of_range("Cannot popEnd() with list with no elements");
        // perform pop back
        T item = this->endNode->item;
        Node *popNode = this->endNode;
        // unlink element form list
        Node *newEnd = popNode->prev;
        if(newEnd) newEnd->next = nullptr;

        // update ptrs
        this->endNode = newEnd;
        // edge case: last node causes begin to be set to null
        if(!popNode->prev) this->beginNode = nullptr;
        this->size --;
    
        delete popNode;
        return item;
    }
    
    // push an item to the start of the list
    template <typename T> void List<T>::push(T item)
    {
        this->insert(0, item);
    }

    // pop an item from the start of the list
    template <typename T> T List<T>::pop()
    {
        return this->remove(0);
    }

    // insert element at index
    template <typename T> void List<T>::insert(uint32_t idx, T item) 
    {
        // check op valid
        if(idx > this->length()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Insert at index " << idx << " out of list bounds";
            throw out_of_range(strStream.str());
        }
    
        // find nextnode
        Node *nextNode = this->beginNode;
        Node *prevNode = nullptr;
        for(int i = 0; i < idx; i ++) 
        {
            nextNode = nextNode->next;
            prevNode = nextNode->prev;
        }

        // link node with the rest of the list
        Node *newNode = new Node { item, prevNode, nextNode };
        if(!prevNode) this->beginNode = newNode;
        else prevNode->next = newNode;
        if(!nextNode) this->endNode = newNode;
        else nextNode->prev = newNode; 
        
        // update state
        this->size ++;
    }

    // remove element at index
    template <typename T> T List<T>::remove(uint32_t idx)
    {
        // check op valid
        if(idx >= this->length()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Remove at index " << idx << " out of list bounds";
            throw out_of_range(strStream.str());
        }
    
        // find nextnode & prevNode & node to remove
        Node *removeNode = this->beginNode;
        Node *prevNode = nullptr;
        Node *nextNode = removeNode->next;
        for(int i = 0; i < idx; i ++) 
        {
            removeNode = removeNode->next;
            prevNode = removeNode->prev;
            nextNode = removeNode->next;
        }
        
        // unlink node with the rest of the list
        if(!prevNode) this->beginNode = nextNode;
        else prevNode->next = nextNode;
        if(!nextNode) this->endNode = prevNode;
        else nextNode->prev = prevNode; 
        
        // update state
        this->size --;

        T removeItem = removeNode->item;
        delete removeNode;
        return removeItem;
    }

    // get/set element at index
    template <typename T> T& List<T>::operator [](uint32_t idx)
    {
        // check op valid
        if(idx >= this->length()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "Index " << idx << " out of list bounds";
            throw out_of_range(strStream.str());
        }
        
        Node *node = this->beginNode;
        for(uint32_t i = 0; i < idx; ++i) { node = node->next; }
        return node->item;
    }

    // support for output streams
    template <typename T> ostream& operator << (ostream &stream, const List<T>& l)
    {
        stream << (string)l;
        return stream;
    }
    
    /* iterators */
    // advance iterator
    template <typename T> void List<T>::ListIterator::next()
    {
        if(this->empty()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "next() called on empty list iterator";
            throw out_of_range(strStream.str());
        }
        
        this->currentNode = this->currentNode->next;
    }
    
    // retreat  iterator
    template <typename T> void List<T>::ListIterator::prev()
    {
        if(this->empty()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "next() called on empty list iterator";
            throw out_of_range(strStream.str());
        }
        
        this->currentNode = this->currentNode->prev;
    }
    
    
    // get the item pointed by the iterator 
    template <typename T> T& List<T>::ListIterator::item()
    {
        
        if(this->empty()) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "item() called on empty list iterator";
            throw out_of_range(strStream.str());
        }
    
        return this->currentNode->item;
    }
}

#endif /* end of include guard: _LIST_CPP_ */
