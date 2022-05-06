#include "PlaneSweepTriangulation.h"
#include <cmath>
#include <iostream>

PlaneSweepTriangulation::PlaneSweepTriangulation(Points* points)
{
	IBO = 0;
	VBO = 0;
	VAO = 0;
	circlePoints = NULL;
	curPoint = 0;
	initialized = false;
	this->points = points;
	//std::cout << "point size in triangulation: " << points->GetPointsSize() << std::endl;
	graph = new Graph(points->GetPointsSize());
	boundary = new Boundary(points, graph);
	triples = new Triples(boundary);
}

PlaneSweepTriangulation::~PlaneSweepTriangulation()
{
	delete graph;
	delete boundary;
	delete triples;
	if (circlePoints) {
		delete[] circlePoints;
	}
	
}

void PlaneSweepTriangulation::AdvanceTriangulation()
{
	if (!initialized) {
		Initialize(0);
		initialized = true;
		AddTo(1, 0);
		curPoint = 2;
	}
	else {
		int q;
		if (triples->MinTop() <= points->GetPoint(curPoint)[1]) {
			q = triples->GetPointCorrToMinTop();
			Update(q);
		}
		else if(curPoint != points->GetPointsSize()) {
			q = boundary->ClosestPointTo(curPoint);
			AddTo(curPoint, q);
			curPoint++;
		}
		else if (triples->MinTop() != FLT_MAX) {
			q = triples->GetPointCorrToMinTop();
			Update(q);
		}
	}
}

void PlaneSweepTriangulation::CompleteTriangulation()
{
	while (triples->MinTop() != FLT_MAX || curPoint != points->GetPointsSize()) {
		AdvanceTriangulation();
	}
}

void PlaneSweepTriangulation::Render(GLuint uniformMyColor)
{
	glUniform1i(uniformMyColor, 0);
	RenderPoints();
	RenderGraph();
	glUniform1i(uniformMyColor, 1);
	RenderBoundary();
	glUniform1i(uniformMyColor, 2);
	RenderTriples();
}

void PlaneSweepTriangulation::UpdateBuffers()
{
	if (circlePoints) {
		delete[] circlePoints;
		circlePoints = NULL;
	}
	
	if (triples->GetSizeOfTriples() > 0) {
		circlePoints = new float[triples->GetSizeOfTriples() * 90 * 3];
		float* circles = triples->GetCircles();
		for (int i = 0; i < triples->GetSizeOfTriples(); i++) {
			drawCircle(circles[3 * i], circles[3 * i + 1], circles[3 * i + 2], 90, i);
		}
	}

	//Add Circle Points
	

	//Generate and bind Vertex Array
	glBindVertexArray(VAO);

	//Generate Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(graph->GetEdges()[0]) * graph->GetNoOfEdges() * 2, graph->GetEdges());
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graph->GetEdges()[0]) * 2 * graph->GetMaxNoOfEdges(), sizeof(graph->GetEdges()[0]) * boundary->GetBoundarySize(), boundary->GetBoundary());

	//Generate and bind Vetex Buffer
	

	//Put vertices into buffer
	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3, points->GetPoints());
	if (circlePoints) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3, sizeof(points->GetPoints()[0]) * 90 * triples->GetSizeOfTriples() * 3, circlePoints);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PlaneSweepTriangulation::InitializeBuffers()
{
	//Generate and bind Vertex Array
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate Index Buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graph->GetEdges()[0]) * 3 * graph->GetMaxNoOfEdges(), NULL, GL_STATIC_DRAW);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graph->GetEdges()[0]) * 2 * graph->GetMaxNoOfEdges(), sizeof(graph->GetEdges()[0]) * boundary->GetBoundarySize(), boundary->GetBoundary());

	//Generate and bind Vetex Buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Put vertices into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3 + sizeof(points->GetPoints()[0]) * 90 * graph->GetMaxNoOfEdges() * 3, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3, points->GetPoints());
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3, sizeof(points->GetPoints()[0]) * 90 * triples->GetSizeOfTriples() * 3, circlePoints);
	//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(graph->GetEdges()[0]) * 2 * graph->GetMaxNoOfEdges()));
	glEnableVertexAttribArray(0);

	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PlaneSweepTriangulation::PrintTriangulation()
{
	graph->PrintEdges();
	boundary->PrintBoundary();
	triples->PrintTriples();
}

void PlaneSweepTriangulation::RenderPoints()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_POINTS, 0, points->GetPointsSize());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PlaneSweepTriangulation::RenderGraph()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_LINES, graph->GetNoOfEdges() * 2, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PlaneSweepTriangulation::RenderBoundary()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_POINTS, boundary->GetBoundarySize(), GL_UNSIGNED_INT, (void*)(sizeof(graph->GetEdges()[0]) * 2 * graph->GetMaxNoOfEdges()));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PlaneSweepTriangulation::RenderTriples()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	for (int i = 0; i < triples->GetSizeOfTriples(); i++) {
		glDrawArrays(GL_LINE_LOOP, points->GetPointsSize() + i * 90, 90);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void PlaneSweepTriangulation::Initialize(int p)
{
	boundary->Initialize(p);
}

void PlaneSweepTriangulation::AddTo(int p, int q)
{
	graph->Add_Edge(p, boundary->GetCorrespondingPoint(q));
	triples->DeleteFromTriples(q);
	boundary->InsertNewOnBoundary(p, q);
	triples->InsertNewOnBoundary(q);
	int pIndex = q + 1;
	// Continue from here
	if (CheckLeftTurn(pIndex - 2, pIndex - 1, pIndex)) {
		triples->InsertInTriples(pIndex - 1);
	}
	if (CheckLeftTurn(pIndex, pIndex + 1, pIndex + 2)) {
		triples->InsertInTriples(pIndex + 1);
	}
}

void PlaneSweepTriangulation::Update(int q)
{
	int bq = q - 1;
	int aq = q + 1;
	graph->Add_Edge(boundary->GetCorrespondingPoint(bq), boundary->GetCorrespondingPoint(aq));
	triples->DeleteFromTriples(bq);
	triples->DeleteFromTriples(q);
	triples->DeleteFromTriples(aq);
	boundary->UpdateOnBoundary(q);
	triples->UpdateOnBoundary(q);
	if (CheckLeftTurn(q - 2, q - 1, q)) {
		triples->InsertInTriples(q - 1);
	}
	if (CheckLeftTurn(q - 1, q, q + 1)) {
		triples->InsertInTriples(q);
	}
	

}

bool PlaneSweepTriangulation::CheckLeftTurn(int i, int j, int k)
{
	if (i < 0) {
		return false;
	}
	if (k >= boundary->GetBoundarySize()) {
		return false;
	}
	float* ip = points->GetPoint(boundary->GetCorrespondingPoint(i));
	float* jp = points->GetPoint(boundary->GetCorrespondingPoint(j));
	float* kp = points->GetPoint(boundary->GetCorrespondingPoint(k));
	// Check Left Turn
	float D = ip[0] * jp[1] + jp[0] * kp[1] + kp[0] * ip[1] - ip[1] * jp[0] - jp[1] * kp[0] - kp[1] * ip[0];
	return (D > 0);
}

void PlaneSweepTriangulation::drawCircle(float cx, float cy, float r, int num_segments, int index)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 

	float x = r;//we start at angle = 0 

	float y = 0;
	for (int ii = 0; ii < num_segments; ii++)
	{
		circlePoints[index * num_segments * 3 + 3 * ii] = x + cx;
		circlePoints[index * num_segments * 3 + 3 * ii + 1] = y + cy;
		circlePoints[index * num_segments * 3 + 3 * ii + 2] = 0;

		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 

		float tx = -y;
		float ty = x;

		//add the tangential vector 

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor 

		x *= radial_factor;
		y *= radial_factor;
	}
}

