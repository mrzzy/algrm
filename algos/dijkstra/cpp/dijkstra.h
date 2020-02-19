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
    // perform djkstra algorihm to derive the shortest path
    // graph - graph to search
    // start - the vertex to start 
    // end - the vertex to find
    // returns shortest path
    template <typename T>
        structs::List<T> djkstra(const structs::Graph<T>& graph, T start, T end);
}

#include "algorithms/graph.cpp"

#endif /* end of include guard: _ALGO_GRAPH_H_ */
