#ifndef _ALGO_GRAPH_CPP_
#define _ALGO_GRAPH_CPP_
/*
 * np dsa assignment
 * graph algorithms
 * by: Zhu Zhanyan S1017047
*/

#include <limits>

#include "algorithms/graph.h"
#include "structs/Heap.h"

using namespace structs;

namespace algo
{
    // min path and distance
    template <typename T> struct VertexMin
    {
        T vertex;
        double minDist;
        VertexMin* prev; // previous vertex in shortest path

        friend bool operator< (const VertexMin& lhs, const VertexMin& rhs)
        { return lhs.minDist < rhs.minDist; }
        friend bool operator> (const VertexMin& lhs, const VertexMin& rhs)
        { return rhs < lhs; }
    };
    
    // djkstra shortest path algorithm
    template <typename T> List<T> djkstra(const Graph<T>& graph, T start, T end)
    {
        // populate search queue with vertices
        List<T> vertices = graph.vertices();
        HashSet<T> traversedSet;
        HashMap<T, VertexMin<T>*> minMap; // maps vertex to vertex min 
        HashMap<T, uint32_t> keyMap; // maps vertex to heap key
        Heap<VertexMin<T>> searchHeap; // min heap
        for(auto vertIter = vertices.begin(); !vertIter.empty(); vertIter.next())
        {
            VertexMin<T> *vertMin = nullptr;
            // the current vertice has distance of 0;
            if(*vertIter == start)
            { 
                vertMin = new VertexMin<T>{ *vertIter,  0.0, nullptr };
            }
            // other vertices start with infinity distance - represented as double max
            else
            {
                vertMin = new VertexMin<T>{ 
                    *vertIter, std::numeric_limits<double>::max(), nullptr };
            }
            keyMap[*vertIter] = searchHeap.add(*vertMin);
            minMap[*vertIter] = vertMin;
        }
        
        while(searchHeap.length() > 0)
        {
            // search via shorter vertex first
            VertexMin<T> vm = searchHeap.popTop();
            VertexMin<T> *vertMin = minMap[vm.vertex];
            T vertex = vertMin->vertex;
            traversedSet.add(vertex);

            // update min dist & path for neighbours via current vertex
            List<T> neighbours = graph.neighbours(vertex);
            for(auto neighIter = neighbours.begin(); !neighIter.empty(); neighIter.next())
            {
                T neighbour = *neighIter;
                // skip if already explored
                if(traversedSet.has(neighbour)) continue;

                VertexMin<T>* neighMin = minMap[neighbour];

                // compute distance to neighbour via this vertex
                double distVia = vertMin->minDist + graph.weight(vertex, neighbour);

                if(distVia < neighMin->minDist)
                {
                    // found shorter path for neighbour
                    neighMin->prev = vertMin;
                    neighMin->minDist = distVia;
                   
                    // readd min dist & path into search heap to reflect updated min dist & path
                    searchHeap.remove(keyMap[neighbour]);
                    keyMap[neighbour] = searchHeap.add(*neighMin);
                }
            }
        }
        
        // construct min path
        List<T> minPath;
        for(VertexMin<T>* vertMin = minMap[end]; vertMin; vertMin = vertMin->prev)
        {
            minPath.push(vertMin->vertex);
        }
        
        // cleanup
        for(auto vertIter = vertices.begin(); !vertIter.empty(); vertIter.next())
        {
            delete minMap[*vertIter];
        }
    
        return minPath;
    }
}


#endif /* end of include guard: _ALGO_GRAPH_CPP_ */
