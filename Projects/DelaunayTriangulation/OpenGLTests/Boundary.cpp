#include "Boundary.h"
#include <iostream>

Boundary::Boundary()
{

}

Boundary::~Boundary()
{
	delete[] boundary;
}

Boundary::Boundary(Points* points, Graph* graph)
{
	std::cout << "Points size in boundary " << points->GetPointsSize() << std::endl;
	boundary = new int[3 * points->GetPointsSize() - 6];
	boundarySize = 0;
	this->points = points;
	this->graph = graph;
}

int Boundary::Before(int q)
{
	int start = 0;
	int end = boundarySize - 1;
	while (start != end) {
		int mid = (start + end) / 2;
		if (boundary[mid] == q) {
			if (mid != 0) {
				return boundary[mid - 1];
			}
			else {
				return -1;
			}
		}
		else if (points->GetPoint(boundary[mid])[0] < points->GetPoint(q)[0]) {
			start = mid;
		}
		else {
			end = mid;
		}
	}
	return boundary[((start + end) / 2) - 1];
}

int Boundary::After(int q)
{
	int start = 0;
	int end = boundarySize - 1;
	while (start != end) {
		int mid = (start + end) / 2;
		if (boundary[mid] == q) {
			if (mid != boundarySize - 1) {
				return boundary[mid + 1];
			}
			else {
				return -1;
			}
		}
		else if (points->GetPoint(boundary[mid])[0] < points->GetPoint(q)[0]) {
			start = mid;
		}
		else {
			end = mid;
		}
	}
	return boundary[((start + end) / 2) + 1];
}

int Boundary::GetCorrespondingPoint(int q)
{
	return boundary[q];
}

void Boundary::ClosestPointTo(int p, int& q1, int& q2)
{
	q1 = 0;
	q2 = boundarySize - 1;
	float x = points->GetPoint(p)[0];

	while (q1 + 1 != q2) {
		int mid = (q1 + q2) / 2;
		if (points->GetPoint(boundary[mid])[0] < x) {
			q1 = mid;
		}
		else {
			q2 = mid;
		}
	}
	return;
}

void Boundary::InsertNewOnBoundary(int p, int q)
{

	boundarySize = boundarySize + 2;
	for (int i = boundarySize - 1; i > q + 1; i--) {
		boundary[i] = boundary[i - 2];
	}
	boundary[q + 1] = p;
	graph->Add_Edge(p, q);
}

void Boundary::UpdateOnBoundary(int q)
{
	graph->Add_Edge(boundary[q - 1], boundary[q + 1]);
	boundarySize = boundarySize - 1;
	for (int i = q; i < boundarySize; i++) {
		boundary[i] = boundary[i + 1];
	}
}

void Boundary::Initialize(int q)
{
	boundarySize++;
	boundary[0] = q;
}

void Boundary::PrintBoundary()
{
	for (int i = 0; i < boundarySize; i++) {
		std::cout << boundary[i] << std::endl;
	}
}