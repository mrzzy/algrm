#ifndef _ALGO_GRAPH_H_
#define _ALGO_GRAPH_H_
/*
 * np dsa assignment
 * graph algorithms
 * by: Zhu Zhanyan S1017047
*/

#include <iostream>

#include "structs/Graph.h"
#include "structs/HashSet.h"

namespace algo
{
    // perform depth first search on the given graph using the lambda
    // graph - graph to search
    // start - the vertex to start 
    // traversedSet - set of traversed vertexs
    // maxDepth - maximum depth of search
    // currentPath - current path explored
    // lambda(path, vertex) - predicate lambda returning true to continue search, false otherwise
    template <typename T, typename F>
        bool dfsSearch(
                const structs::Graph<T>& graph, T start, 
                structs::HashSet<T> traversedSet,
                int32_t maxDepth,
                structs::List<T> path,
                F && lambda);
}

#include "algorithms/graph.cpp"

#endif /* end of include guard: _ALGO_GRAPH_H_ */
