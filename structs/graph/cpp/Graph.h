#ifndef _STRUCT_GRAPH_H_
#define  _STRUCT_GRAPH_H_
/*
 * np dsa assignment
 * graph data structure
 * by: Zhu Zhanyan S10170407
*/

#include <exception>
#include <iostream>

#include "algorithms/string.h"
#include "structs/HashMap.h"
#include "structs/HashSet.h"

namespace structs
{
    template <typename T> class Graph 
    {
        public:
            struct Edge;
            // a node/vertex in the graph
            struct Node
            {
                T item;
                List<Edge> edges;
            };
            // edges cannecting nodes in the graph
            struct Edge
            {
                Node* target;
                double weight;
            };
            
            /* constructor/desctructor */
            Graph() {};
            Graph(const Graph& g); // copy constructor
            ~Graph();

            /* graph attributes */
            // returns no. of vertices in graph
            uint32_t length() const { return this->nodeMap.length(); }
            // returns true if the graph is empty
            bool empty() const { return this->length() == 0; }
            // returns true if the graph has the given vertex
            bool has(const T& vertex) const { return this->nodeMap.has(vertex); }
            // return the vertices in the graph
            List<T> vertices() const { return this->nodeMap.keys(); }

            // returns true if the given from and to vertices 
            // are connected by an edge
            // throws exception if vertex does not already exist in the graph
            bool adjacent(T from, T to) const;
            
            // returns the weight of edge connecting the given from and 
            // to vertices
            // throws exception if vertex does not already exist in the graph
            // throws exception if given vertices are not connected by an edge
            double weight(T from, T to) const;

            // collects the list of connected vertices to the given vertex
            // returns the list of connected vertices
            HashSet<T> neighbours(T vertex) const;
    
            // compute & return the distance travarsed path
            // path - list of vertex traversed on the path
            double distance(const List<T>& path) const;
        
            /* graph operations */
            // add a new vertex to the graph
            // throws exception if vertex already exists in the graph
            void add(T vertex);
        
            // remove the given vertex from the graph
            // removes any edges connected to vertex
            // throws exception if vertex does not already exist in the graph
            void remove(const T& vertex);
            
            // link the given from and to vertices with 
            // a weighted edge with the given weight
            // throws exception if vertices does not already exist in the graph
            // or if vertices are already connected by an edge
            void link(T from, T to, double weight=1.0);
            
            // unlink the given from and to vertices by 
            // removing the connecting edge 
            // throws exception if vertices does not already exist in the graph
            // does nothing if edge does not  already exist 
            void unlink(T from, T to);
            

        private:
            HashMap<T, Node*> nodeMap;
            Node* getNode(T vertex) const { return this->nodeMap.get(vertex); }

    };
}

// reuquired for templates to work properly
#include "structs/Graph.cpp"

#endif /* end of include guard:  _STRUCT_GRAPH_H_ */
