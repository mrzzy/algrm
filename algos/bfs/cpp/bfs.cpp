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

    
    // perform breath first search
    template <typename T, typename F> 
        bool bfsSearch(
                const Graph<T>& graph, T current, HashSet<T> traversedSet,
                int32_t maxDepth, List<T> path, F && lambda)

        {
            // stop exploring if exceeded max depth
            if(maxDepth < 0) return true;

            // breath first search explore state
            struct ExploreState 
            {
                T vertex;
                HashSet<T> traversedSet;
                List<T> path;
                uint32_t depth;
            };

            // queue of vertices to explore
            List<ExploreState> exploreQueue;
            exploreQueue.append(ExploreState{current, HashSet<T>(), List<T>(), 0});
            
            bool shouldContinue;
            while(exploreQueue.length() > 0)
            {
                // explore first neighbour if not already explored
                ExploreState state = exploreQueue.pop();
                
                // don't explore neighbour if already explred
                if(state.traversedSet.has(state.vertex)) continue;
                // don't explore if already max depth exceeded
                if(state.depth > maxDepth) continue;
            
                shouldContinue = explore(state.vertex, state.traversedSet, state.path, lambda);
                if(!shouldContinue) return shouldContinue;

                // add neighbours to explore queue
                List<T> neighbours = graph.neighbours(state.vertex);
                for(auto neighIter = neighbours.begin(); !neighIter.empty(); neighIter.next())
                {
                    T neighbour = *neighIter;
                    exploreQueue.append(ExploreState{ neighbour, state.traversedSet, 
                            state.path, state.depth + 1  });
                }
            }
            
            return true;
        }
}


#endif /* end of include guard: _ALGO_GRAPH_CPP_ */
