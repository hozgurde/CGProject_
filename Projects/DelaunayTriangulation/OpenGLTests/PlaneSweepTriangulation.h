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
	void UpdateBuffers();
	void InitializeBuffers();
	void PrintTriangulation();

private:
	void drawCircle(float cx, float cy, float r, int num_segments, int index);
	void RenderPoints();
	void RenderGraph();
	void RenderBoundary();
	void RenderTriples();
	void RenderSweepLine();
	void Initialize(int p);
	void AddTo(int p, int q);
	void Update(int q);
	bool CheckLeftTurn(int i, int j, int k);
	GLuint VAO, VBO, IBO;
	Points* points;
	Graph* graph;
	Boundary* boundary;
	Triples* triples;
	float* circlePoints;
	bool initialized;
	int curPoint;
	float lastY;
	GLfloat* sweepLine;
};