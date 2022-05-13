#pragma once
#include "Points.h"
#include "Graph.h"

class Boundary {
public:
	Boundary();
	~Boundary();
	Boundary(Points* points, Graph* graph);
	int GetCorrespondingPoint(int q);
	int ClosestPointTo(int p);
	void InsertNewOnBoundary(int p, int q);
	void UpdateOnBoundary(int q);
	void Initialize(int q);
	Points* GetPoints() { return points; };
	void PrintBoundary();
	int* GetBoundary() { return boundary; };
	int GetBoundarySize() { return boundarySize; };
	float CheckMaxYDisk(int p1, int p2, float yp);
private:
	int* boundary;
	int boundarySize;
	Points* points;
	Graph* graph;
};