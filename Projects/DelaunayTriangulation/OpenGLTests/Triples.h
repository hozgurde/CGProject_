#pragma once
#include "Boundary.h"

class Triples {
public:
	Triples();
	~Triples();
	Triples(Boundary* boundary);
	float MinTop();
	int GetPointCorrToMinTop();
	void DeleteFromTriples(int q);
	void InsertInTriples(int q);
	int GetSizeOfTriples() { return sizeOfTriples; };
	float* GetCircles() { return circles; };
private:
	Boundary* boundary;
	int* triples;
	float* tops;
	float* circles;
	int sizeOfTriples;
};