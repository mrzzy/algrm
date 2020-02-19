#ifndef _STRUCT_GRAPH_CPP_
#define _STRUCT_GRAPH_CPP_
/*
 * np dsa assignment
 * graph data structure
 * by: Zhu Zhanyan S10170407
*/


#include "structs/Graph.h"

namespace structs
{
    /* constructors & destructors */
    template <typename T> Graph<T>::Graph(const Graph& g)
    {
        List<T> vertices = g.nodeMap.keys();
    
        // copy vertices
        for(auto vertIter = vertices.begin(); !vertIter.empty(); vertIter.next())
        {
            this->add(*vertIter);
        }
        
        // copy edges
        for(auto vertIter = vertices.begin(); !vertIter.empty(); vertIter.next())
        {
            T& vertex = *vertIter;
            List<T> neighbours = g.neighbours(vertex).values();
            for(auto neighIter = neighbours.begin(); !neighIter.empty(); neighIter.next())
            {
                T& neighbour = *neighIter;
                double weight = g.weight(vertex, neighbour);
                if(!this->adjacent(vertex, neighbour))
                {
                    this->link(vertex, neighbour, weight);
                }
            }
        }
    }

    template <typename T> Graph<T>::~Graph()
    {
        List<Node *> nodes = this->nodeMap.values();
        for(auto nodeIter = nodes.begin(); !nodeIter.empty(); nodeIter.next())
        {
            Node* node = *nodeIter;
            delete node;
        }
    }

    /* graph attributes */
    template <typename T> bool Graph<T>::adjacent(T from, T to) const
    {
        // check if vertices exist in graph
        if(!this->has(from)  || !this->has(to))
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "adjacent() called on nonexistent vertices"; 
            throw runtime_error(strStream.str());
        }
        
        // find for edge connecting from node to to node
        Node* fromNode = this->getNode(from);
        Node* toNode  = this->getNode(to);
        for(auto edgeIter = fromNode->edges.begin(); !edgeIter.empty(); edgeIter.next())
        {
            if(edgeIter->target == toNode) { return true; }
        }
        return false;
    }
    
    template <typename T> double Graph<T>::weight(T from, T to) const
    {
        // check if vertices exist in graph
        if(!this->has(from)  || !this->has(to))
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "adjacent() called on nonexistent vertices"; 
            throw runtime_error(strStream.str());
        }
        
        // find for edge connecting from node to to node
        Node* fromNode = this->getNode(from);
        Node* toNode  = this->getNode(to);
        for(auto edgeIter = fromNode->edges.begin(); !edgeIter.empty(); edgeIter.next())
        {
            if(edgeIter->target == toNode) { return edgeIter->weight; }
        }
    
        // no edge found: throw exception
        ostringstream strStream;
        strStream << __FILE__  << ":" << __LINE__ << ": ";
        strStream << "weight() call on vertices with no connecting edges"; 
        throw runtime_error(strStream.str());
    }

    template <typename T> HashSet<T> Graph<T>::neighbours(T vertex) const
    {
        // check if vertice is in the graph
        if(!this->has(vertex))
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "neighbours() called on non existent"; 
            throw runtime_error(strStream.str());
        }

        HashSet<T> neighbours;
        Node* node = this->getNode(vertex);
        List<Edge>& edges = node->edges;
        for(auto edgeIter = edges.begin(); !edgeIter.empty(); edgeIter.next())
        {
            neighbours.add(edgeIter->target->item);
        }

        return neighbours;
    }

    template <typename T> double Graph<T>::distance(const List<T>& path) const
    {
        auto vertexIter = path.begin();
        T fromVertex = *vertexIter;
        double dist = 0.0;

        vertexIter.next(); // start form next node
        for(;!vertexIter.empty(); vertexIter.next())
        {
            T toVertex = *vertexIter;
            dist  += this->weight(fromVertex, toVertex);
            fromVertex = toVertex;
        }
        
        return dist;
    }
    
    /* graph operations */
    template <typename T> void Graph<T>::add(T vertex)
    {
        if(this->has(vertex))
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "add() attempted to add duplicate vertex to graph"; 
            throw runtime_error(strStream.str());
        }

        Node *node = new Node { vertex, List<Edge>() };
        this->nodeMap[vertex] = node;
    }

    template <typename T> void Graph<T>::remove(const T &vertex)
    {
        if(this->has(vertex))
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "remove() attempted to remove vertex that is not in the graph";
            throw runtime_error(strStream.str());
        }

        // remove connecting edges
        List<T> neighbours = this->neighbours(vertex);
        for(auto neighbour = neighbours.begin(); !neighbour.empty(); !neighbour.next())
        {
            this->unlink(vertex, neighbour);
        }


        Node* node = this->nodeMap.remove(vertex);
        delete node;
    }

    template <typename T> void Graph<T>::link(T from, T to, double weight)
    {
        // check if vertices exist in graph
        if(!this->has(from)  || !this->has(to))
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "link() called on nonexistent vertices"; 
            throw runtime_error(strStream.str());
        }
        
        // check if vertices are already linked
        if(this->adjacent(from, to)) 
        {
            ostringstream strStream;
            strStream << __FILE__  << ":" << __LINE__ << ": ";
            strStream << "link(): duplicate link on vertices"; 
            throw runtime_error(strStream.str());
        }


        // add edges to graph to create link
        Node* fromNode = this->nodeMap[from];
        Node* toNode  = this->nodeMap[to];
        
        fromNode->edges.append(Edge{ toNode, weight });
        toNode->edges.append(Edge{ fromNode, weight });
    }

    template <typename T> void Graph<T>::unlink(T from, T to)
    {
        Node* fromNode = this->nodeMap[from];
        Node* toNode  = this->nodeMap[to];

        // find idxs of edgets to remove 
        List<uint32_t> fromRemoveIdxs;
        List<uint32_t> toRemoveIdxs;
        
        auto fromEdgeIter = fromNode->edges.begin();
        for(uint32_t i = 0; i < fromNode->edges.length(); i ++)
        {
            if(fromEdgeIter->target == toNode)
            {
                fromRemoveIdxs.append(i);
            }
            // advance iterator
            fromEdgeIter.next();
        }

        auto toEdgeIter = toNode->edges.begin();
        for(uint32_t i = 0; i < toNode->edges.length(); i ++)
        {
            if(toEdgeIter->target == fromNode)
            {
                toRemoveIdxs.append(i);
            }
            // advance iterator
            toEdgeIter.next();
        }

        // remove edges
        for(auto idxIter = fromRemoveIdxs.begin(); !idxIter.empty(); idxIter.next())
        { fromNode->edges.remove(*idxIter); }
        for(auto idxIter = toRemoveIdxs.begin(); !idxIter.empty(); idxIter.next())
        { toNode->edges.remove(*idxIter); }
    }
}


#endif /* end of include guard: _STRUCT_GRAPH_CPP_ */
