#pragma once
#include "Points.h"
#include "Graph.h"

class Boundary {
public:
	Boundary();
	~Boundary();
	Boundary(Points* points, Graph* graph);
	int Before(int q);
	int After(int q);
	int GetCorrespondingPoint(int q);
	void ClosestPointTo(int p, int& q1, int& q2);
	void InsertNewOnBoundary(int p, int q);
	void UpdateOnBoundary(int q);
	void Initialize(int q);
	Points* GetPoints() { return points; };
	void PrintBoundary();
	int* GetBoundary() { return boundary; };
	int GetBoundarySize() { return boundarySize; };
private:
	int* boundary;
	int boundarySize;
	Points* points;
	Graph* graph;
};