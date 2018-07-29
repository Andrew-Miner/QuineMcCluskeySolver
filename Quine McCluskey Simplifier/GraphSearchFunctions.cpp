#include "GraphSearchFunctions.h"

template <class Process, class Item, class SizeType>
void rec_dfs(Process f, Graph<Item>& g, SizeType v, bool marked[])
{
	std::set<std::size_t> connections = g.neighbors(v);
	std::set<std::size_t>::iterator it;

	marked[v] = true;	// Mark vertex v.
	f(g[v]);			// Process the label of vertex v with the function f.
	
	// Traverse all the neighbors, looking for unmarked vertices:
	for (it = connections.begin(); it != connections.end(); ++it)
	{
		if (!marked[*it])
			rec_dfs(f, g, *it, marked);
	}
}

template <class Process, class Item, class SizeType>
void depth_first(Process f, Graph<Item>& g, SizeType start)
{
	assert(start < g.size());

	bool marked[g.MAXIMUM];
	std::fill_n(marked, g.size(), false);
	rec_dfs(f, g, start, marked);
}

template <class Process, class Item, class SizeType>
void breadth_first(Process f, Graph<Item>& g, SizeType start)
{
	assert(start < g.size());

	bool marked[g.MAXIMUM];
	std::set<std::size_t> connections;
	std::set<std::size_t>::iterator it;
	std::queue<std::size_t> vertex_queue;

	std::fill_n(marked, g.size(), false);
	marked[start] = true;
	f(g[start]);
	vertex_queue.push(start);

	do
	{
		connections = g.neighbors(vertex_queue.front());
		vertex_queue.pop();

		// Mark and process the unmarked neighbors, and place them in the queue.
		for (it = connections.begin(); it != connections.end(); ++it)
		{
			if (!marked[*it])
			{
				marked[*it] = true;
				f(g[*it]);
				vertex_queue.push(*it);
			}
		}
	} while (!vertex_queue.empty());
}