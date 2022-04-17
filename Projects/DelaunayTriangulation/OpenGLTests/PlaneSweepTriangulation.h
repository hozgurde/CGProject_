#pragma once
#include "Points.h"

class PlaneSweepTriangulation
{
public:
	PlaneSweepTriangulation(Points points);
	void AdvanceTriangulation();
	void CompleteTriangulation();
	void Render();
private:
	Points points;
};