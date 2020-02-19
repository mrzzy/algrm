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
    // explore the current vertex if not already traversed
    template <typename T, typename F> 
        bool explore(T current, HashSet<T>& traversedSet, List<T>& path, F&& lambda)
        {
            // mark vertex as explored
            path.append(current);
            traversedSet.add(current);

            // call user defined functionality in lambda
            return lambda(path, current);
        }


    // perform depth first search
    template <typename T, typename F> 
        bool dfsSearch(
                const Graph<T>& graph, T current, HashSet<T> traversedSet,
                int32_t maxDepth, List<T> path, F && lambda)

        {
            // stop exploring if exceeded max depth
            if(maxDepth < 0) return true;
            // stop exploring if already explored
            if(traversedSet.has(current)) return true;
            
            // explore current vertex
            bool shouldContinue = explore(current, traversedSet, path, lambda);
            if(!shouldContinue) return shouldContinue;

            // traverse neighbour vertices
            List<T> neighbours = graph.neighbours(current);
            for(auto neighIter = neighbours.begin(); !neighIter.empty(); neighIter.next())
            {
                T neighbour = *neighIter;
                // recursive explore neighbour children
                shouldContinue = dfsSearch(
                        graph, neighbour, traversedSet, maxDepth - 1, path, lambda);
                if(!shouldContinue) return shouldContinue;
            }
        
            // not found: coutinue  to search
            return true;
        }
    
}


#endif /* end of include guard: _ALGO_GRAPH_CPP_ */
