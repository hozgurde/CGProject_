#include "Graph.h"

Graph::Graph()
{
	Graph(2);
}

Graph::Graph(int noOfPoints)
{
	maxNoOfEdges = 3 * noOfPoints - 6;
	noOfEdges = 0;
	edges = new int[2 * noOfEdges];
}

Graph::~Graph()
{
	delete[] edges;
}

void Graph::Add_Edge( int p,  int q)
{
	edges[2 * noOfEdges] = p;
	edges[2 * noOfEdges + 1] = q;
	noOfEdges++;
}
