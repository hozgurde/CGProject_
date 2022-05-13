#include "Points.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <random>

Points::Points()
{
	pointsSize = 0;
	points = NULL;
	CreateRandomPoints(0);
}

Points::Points(int noOfPoints)
{
	pointsSize = 0;
	CreateRandomPoints(noOfPoints);
}

Points::~Points()
{
	ClearPoints();
}

void Points::ChangePoint(int index, float x, float y)
{
	points[3 * index] = x;
	points[3 * index + 1] = y;

}

void Points::generateRandomPointsForRandomized(int noOfPoints) {

	if (pointsSize != 0) {
		ClearPoints();
	}

	pointsSize = noOfPoints;

	if (pointsSize <= 0) {
		return;
	}

	double upper_bound = 10 * noOfPoints;
	double lower_bound = -upper_bound;

	std::mt19937 rng;
	std::uniform_real_distribution<double> dist(lower_bound, upper_bound);
	points = new GLfloat[pointsSize * 3];

	for (int i = 0; i < noOfPoints; i++) {
		if (i % 3 == 2) {
			points[i] = 0.0;
		}
		else {
			double p = dist(rng);
			points[i] = p;
		}

	}
}

void Points::CreateRandomPoints(int noOfPoints)
{
	if (pointsSize != 0) {
		ClearPoints();
	}
	srand(time(NULL));
	std::cout << time(NULL) << std::endl;
	pointsSize = noOfPoints;
	if (pointsSize > 0) {
		points = new GLfloat[pointsSize * 3];
		GLfloat p;
		for (int i = 0; i < pointsSize * 3; i++) {
			if (i % 3 == 2) {
				points[i] = 0.0;
			}
			else {
				p = (rand()) / ((float)(RAND_MAX / 2)) - 1.0;
				points[i] = p;
			}
		}
		
	}
}

void Points::RenderPoints()
{

}

void Points::ClearPoints()
{
	delete[] points;
}

GLfloat* Points::GetPoint(int index)
{
	return &points[3 * index];
}

GLfloat* Points::GetPoints()
{
	return points;
}

void Points::Sort()
{
	/*std::cout << "--------------------" << std::endl;
	for (int i = 0; i < pointsSize; i++) {
		std::cout << points[3 * i] << " " << points[3 * i + 1] << std::endl;
	}
	std::cout << "--------------------" << std::endl;*/
	QuickSort(0, pointsSize - 1);
	/*for (int i = 0; i < pointsSize; i++) {
		std::cout << points[3 * i] << " " << points[3 * i + 1] << std::endl;
	}
	std::cout << "--------------------" << std::endl;*/
}

void Points::QuickSort(int start, int end)
{
	if (start < end)
	{
		/* pi is partitioning index, arr[pi] is now
		   at right place */
		int pi = Partition(start, end);

		QuickSort(start, pi - 1);  // Before pi
		QuickSort(pi + 1, end); // After pi
	}
}

int Points::Partition(int start, int end)
{

	// pivot (Element to be placed at right position)
	GLfloat pivot = points[end * 3 + 1];
	GLfloat pivotx = points[end * 3];

	int i = start; // Index of smaller element and indicates the 
					// right position of pivot found so far

	for (int j = start; j < end; j++)
	{
		// If current element is smaller than the pivot
		if (points[j * 3 + 1] < pivot)
		{
			i++;    // increment index of smaller element
			SwapPoints(i - 1, j);
		}
		else if (points[j * 3 + 1] == pivot && points[j * 3] < pivotx) {
			i++;    // increment index of smaller element
			SwapPoints(i - 1, j);
		}
	}

	SwapPoints(i, end);
	return i;

}

void Points::SwapPoints(int i, int j)
{
	GLfloat x = points[3 * i];
	GLfloat y = points[3 * i + 1];
	points[3 * i] = points[3 * j];
	points[3 * i + 1] = points[3 * j + 1];
	points[3 * j] = x;
	points[3 * j + 1] = y;
}
