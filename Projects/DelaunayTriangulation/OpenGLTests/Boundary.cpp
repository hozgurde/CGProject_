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
	//std::cout << "Points size in boundary " << points->GetPointsSize() << std::endl;
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

int Boundary::ClosestPointTo(int p)
{
	float distance = FLT_MAX;
	int curPoint = 0;
	float* pointP = points->GetPoint(p);
	float x = CheckMaxYDisk(boundarySize - 2, boundarySize - 1, pointP[1]);
	for (int i = 0; i < boundarySize - 1; i++) {
		float temp = CheckMaxYDisk(i, i + 1, pointP[1]);
		if ((x < pointP[0] && temp > pointP[0])) {
			float* tempPoint = points->GetPoint(boundary[i]);
			float curDistance = sqrt((tempPoint[0] - pointP[0]) * (tempPoint[0] - pointP[0]) + (tempPoint[1] - pointP[1]) * (tempPoint[1] - pointP[1]));
			if (curDistance < distance) {
				distance = curDistance;
				curPoint = i;
			}
		}
		x = temp;
	}
	if (distance == FLT_MAX) {
		if (x > pointP[0]) {
			curPoint = 0;
		}
		else {
			curPoint = boundarySize - 1;
		}
	}

	return curPoint;

}



void Boundary::InsertNewOnBoundary(int p, int q)
{

	boundarySize = boundarySize + 2;
	for (int i = boundarySize - 1; i > q + 1; i--) {
		boundary[i] = boundary[i - 2];
	}
	boundary[q + 1] = p;
}

void Boundary::UpdateOnBoundary(int q)
{
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
	std::cout << "-----------Boundary-------------" << std::endl;
	for (int i = 0; i < boundarySize; i++) {
		std::cout << boundary[i] << std::endl;
	}
	std::cout << "--------------------------------" << std::endl;
}

float Boundary::CheckMaxYDisk(int p1,int p2, float yp)
{
	float* point1 = points->GetPoint(boundary[p1]);
	float* point2 = points->GetPoint(boundary[p2]);
	float b1 = -2 * point1[0];
	float c1 = point1[0] * point1[0] + point1[1] * point1[1] - yp * yp;
	float d1 = 2 * yp - 2 * point1[1];
	float b2 = (2 * point2[0] - 2 * point1[0]) / (2 * point1[1] - 2 * point2[1]);
	float c2 = (point1[0] * point1[0] - point2[0] * point2[0] + point1[1] * point1[1] - point2[1] * point2[1]) / (2 * point1[1] - 2 * point2[1]);
	float b = b1 + b2 * d1;
	float c = c1 + c2 * d1;
	float res1 = (-b + sqrt(b * b - 4 * c)) / 2;
	float res2 = (-b - sqrt(b * b - 4 * c)) / 2;

	//Check Left Turn
	float D = point1[0] * point2[1] + point2[0] * yp + res1 * point1[1] - point1[1] * point2[0] - point2[1] * res1 - yp * point1[0];

	if (D > 0) {
		return res1;
	}
	else {
		return res2;
	}
}
