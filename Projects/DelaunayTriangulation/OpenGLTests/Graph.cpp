#include "Graph.h"
#include <iostream>

Graph::Graph()
{
	Graph(2);
}

Graph::Graph(int noOfPoints)
{
	maxNoOfEdges = 3 * noOfPoints - 6;
	noOfEdges = 0;
	edges = new int[2 * maxNoOfEdges];
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

void Graph::PrintEdges()
{
	std::cout << "----------Edges-----------" << std::endl;
	for (int i = 0; i < noOfEdges; i++) {
		std::cout << edges[2 * i] << "-" << edges[2 * i + 1] << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
}
