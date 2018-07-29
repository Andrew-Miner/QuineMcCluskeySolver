// FILE: Graph.h (implementation of Walter Savitch's Graph class)
// TEMPLATE CLASS PROVIDED: Graph<T> (a class for labeled graphs)
//   The vertices of an n-vertex graph are numbered from 0 to n-1. Each vertex has a
//   label of type T. It may be any of the C++ built-in types (int, char, etc.), or any
//   class with a default constructor and an assignment operator. The graph may not have
//   multiple edges.

// Note: This class is built upon the Graph class defined in
// the textbook: 
//
//		Data Structures & Other Objects Using C++ 4th Edition
//				By Michael Main & Walter Savitch
//						Pages 750-752

#pragma once
#include <cstdlib> // std::size_t
#include <set>

template <class T>
class Graph
{
public:
	// MEMBER CONSTANTS
	static const std::size_t MAXIMUM = 20;

	// CONSTRUCTOR
	Graph() : vertexCount(0) {}

	// MODIFICATION MEMBER FUNCTIONS
	void addVertex(const T& label);
	void addEdge(std::size_t source, std::size_t target);
	void removeEdge(std::size_t source, std::size_t target);
	T& operator[] (std::size_t vertex);

	// CONSTANT MEMBER FUNCTIONS
	std::size_t size() const { return vertexCount; }
	bool isEdge(std::size_t source, std::size_t target) const;
	std::set<std::size_t> neighbors(std::size_t vertex) const;
	const T& operator[] (std::size_t vertex) const;

private:
	bool edges[MAXIMUM][MAXIMUM];
	T labels[MAXIMUM];
	std::size_t vertexCount;
};

#include "Graph.template" // include implementation