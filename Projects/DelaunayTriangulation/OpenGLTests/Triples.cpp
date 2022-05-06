#include "Triples.h"
#include <cfloat>
#include <iostream>


Triples::Triples()
{
}

Triples::~Triples()
{
	delete[] triples;
	delete[] tops;
	delete[] circles;
}

Triples::Triples(Boundary* boundary)
{
	this->boundary = boundary;
	triples = new int[3 * boundary->GetPoints()->GetPointsSize() - 6];
	tops = new float[3 * boundary->GetPoints()->GetPointsSize() - 6];
	circles = new float[3 * (3 * boundary->GetPoints()->GetPointsSize() - 6)];
	sizeOfTriples = 0;
}

float Triples::MinTop()
{
	float min = FLT_MAX;
	for (int i = 0; i < sizeOfTriples; i++) {
		if (tops[i] < min) {
			min = tops[i];
		}
	}

	return min;
}

int Triples::GetPointCorrToMinTop()
{
	float min = FLT_MAX;
	int index = -1;
	for (int i = 0; i < sizeOfTriples; i++) {
		if (tops[i] < min) {
			index = triples[i];
			min = tops[i];
		}
	}

	return index;
}

void Triples::DeleteFromTriples(int q)
{
	for (int i = 0; i < sizeOfTriples; i++) {
		if (triples[i] == q) {
			for (int j = i; j < sizeOfTriples - 1; j++) {
				triples[j] = triples[j + 1];
				tops[j] = tops[j + 1];
				circles[3 * j] = circles[3 * (j+ 1)];
				circles[3 * j + 1] = circles[3 * (j+1) + 1];
				circles[3 * j + 2] = circles[3 * (j+1) + 2];
			}
			sizeOfTriples--;
		}
	}
}

void Triples::InsertInTriples(int q)
{
	triples[sizeOfTriples] = q;

	int p1 = boundary->GetCorrespondingPoint(q - 1);
	int p2 = boundary->GetCorrespondingPoint(q);
	int p3 = boundary->GetCorrespondingPoint(q + 1);
	float* point1 = boundary->GetPoints()->GetPoint(p1);
	float* point2 = boundary->GetPoints()->GetPoint(p2);
	float* point3 = boundary->GetPoints()->GetPoint(p3);
	/*std::cout << point1[0] << " " << point1[1] << " " << point1[2] << std::endl;
	std::cout << point2[0] << " " << point2[1] << " " << point2[2] << std::endl;
	std::cout << point3[0] << " " << point3[1] << " " << point3[2] << std::endl;*/

	float A12x = point1[0] - point2[0];
	float A12y = point1[1] - point2[1];
	float A13x = point1[0] - point3[0];
	float A13y = point1[1] - point3[1];
	float B12x = point1[0] + point2[0];
	float B12y = point1[1] + point2[1];
	float B13x = point1[0] + point3[0];
	float B13y = point1[1] + point3[1];

	float C12 = A12x * B12x + A12y * B12y;
	float C13 = A13x * B13x + A13y * B13y;

	float yc = (C13 - A13x * C12 / A12x) / (2 * A13y - 2 * A13x * A12y / A12x);
	float xc = (C12 / (2 * A12x)) - (A12y * yc / A12x);

	float r = sqrt((point1[0] - xc) * (point1[0] - xc) + (point1[1] - yc) * (point1[1] - yc));

	float top = yc + r;

	tops[sizeOfTriples] = top;
	circles[3 * sizeOfTriples] = xc;
	circles[3 * sizeOfTriples + 1] = yc;
	circles[3 * sizeOfTriples + 2] = r;
	//std::cout << top << " " << xc << " " << yc << " " << r << std::endl;

	sizeOfTriples++;



}

void Triples::InsertNewOnBoundary(int q)
{
	for (int i = 0; i < sizeOfTriples; i++) {
		if (triples[i] > q) {
			triples[i] += 2;
		}
	}
}

void Triples::UpdateOnBoundary(int q)
{
	for (int i = 0; i < sizeOfTriples; i++) {
		if (triples[i] > q) {
			triples[i]--;
		}
	}
}

void Triples::PrintTriples()
{
	std::cout << "-----------Triples-------------" << std::endl;
	for (int i = 0; i < sizeOfTriples; i++) {
		std::cout << triples[i] << std::endl;
	}
	std::cout << "-------------------------------" << std::endl;
}
