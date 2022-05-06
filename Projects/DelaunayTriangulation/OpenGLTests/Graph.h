#pragma once
class Graph {
public:
	Graph();
	Graph(int noOfPoints);
	~Graph();
	void Add_Edge( int p,  int q);
	int* GetEdges() { return edges; };
	int GetMaxNoOfEdges() { return maxNoOfEdges; };
	int GetNoOfEdges() { return noOfEdges; };
	void PrintEdges();
private:
	int* edges;
	int noOfEdges;
	int maxNoOfEdges;
};