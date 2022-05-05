#pragma once
#include "Points.h"
#include "float.h"
#include "Graph.h"
#include "Boundary.h"
#include "Triples.h"

class PlaneSweepTriangulation
{
public:
	PlaneSweepTriangulation(Points* points);
	~PlaneSweepTriangulation();
	void AdvanceTriangulation();
	void CompleteTriangulation();
	void Render(GLuint uniformMyColor);
private:
	void drawCircle(float cx, float cy, float r, int num_segments);
	void RenderPoints();
	void RenderGraph();
	void RenderBoundary();
	void RenderTriples();
	GLuint VAO, VBO, IBO;
	Points* points;
	Graph* graph;
	Boundary* boundary;
	Triples* triples;
	float* circlePoints;
};