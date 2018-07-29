#pragma once
#include "Graph.h"
// These functions have been coppied directly from my 
// Data Structures textbook after being studied thoroughly

// Data Structures & Other Objects Using C++ 4th Edition
// By Michael Main & Walter Savitch
// Pages 762-763

template <class Process, class Item, class SizeType>
void rec_dfs(Process f, Graph<Item>& g, SizeType v, bool marked[]);
// Precondition: g is a labeled graph that is being traversed by a depth-first search. For each
// vertex x, marked[x] is true if x has already been visited by this search, otherwise marked[x]
// is false. The vertex v is an unmarked vertex that the search has just arrived at.
// Postcondition: The depth-first search of g has been continued through vertex v and beyond
// to all the vertices that can be reached from v via a path of unmarked vertices. The function
// f has been applied to the label of each vertex visited by the search, and each such vertex x
// has also been marked by setting marked[x] to true.
// Library facilities used: cstdlib, graph.h, set

template <class Process, class Item, class SizeType>
void depth_first(Process f, Graph<Item>& g, SizeType start);
// Precondition: start is a vertex number of the labeled graph g.
// Postcondition: A depth-first search of g has been executed, starting at the start vertex.
// The function f has been applied to the label of each vertex visited by the search.
// Library facilities used: algorithm, cassert, graph.h

template <class Process, class Item, class SizeType>
void breadth_first(Process f, Graph<Item>& g, SizeType start);
// Same as the depth_first function, but using a breadth-first search instead
// Library facilities used: algorithm, cassert, cstdlib, graph.h, queue
