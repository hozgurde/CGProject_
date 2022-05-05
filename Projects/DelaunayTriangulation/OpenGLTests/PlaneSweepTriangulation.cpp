#include "PlaneSweepTriangulation.h"
#include <cmath>
#include <iostream>

PlaneSweepTriangulation::PlaneSweepTriangulation(Points* points)
{
	this->points = points;
	std::cout << "point size in triangulation: " << points->GetPointsSize() << std::endl;
	graph = new Graph(points->GetPointsSize());
	boundary = new Boundary(points, graph);
	triples = new Triples(boundary);

	//Example Structure
	graph->Add_Edge(0, 1);
	graph->Add_Edge(0, 2);
	graph->Add_Edge(1, 2);
	graph->Add_Edge(1, 4);
	graph->Add_Edge(2, 4);
	graph->Add_Edge(0, 3);
	graph->Add_Edge(2, 3);
	
	boundary->Initialize(3);
	boundary->InsertNewOnBoundary(2, 0);
	boundary->InsertNewOnBoundary(4, 2);
	boundary->PrintBoundary();
	std::cout << "size: " << boundary->GetBoundarySize() << std::endl;

	triples->InsertInTriples(2);

	for (int i = 0; i < graph->GetNoOfEdges(); i++) {
		std::cout << graph->GetEdges()[2 * i] << " " << graph->GetEdges()[2 * i + 1] << std::endl;
	}

	circlePoints = new float[triples->GetSizeOfTriples() * 90 * 3];

	//Add Circle Points
	float* circles = triples->GetCircles();
	for (int i = 0; i < triples->GetSizeOfTriples(); i++) {
		drawCircle(circles[3 * i], circles[3 * i + 1], circles[3 * i + 2], 90);
	}

	//Generate and bind Vertex Array
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Generate Index Buffer
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graph->GetEdges()[0]) * 3 * graph->GetMaxNoOfEdges(), graph->GetEdges(), GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(graph->GetEdges()[0]) * 2 * graph->GetMaxNoOfEdges(), sizeof(graph->GetEdges()[0]) * boundary->GetBoundarySize(), boundary->GetBoundary());

	//Generate and bind Vetex Buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Put vertices into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3 + sizeof(points->GetPoints()[0]) * 90 * triples->GetSizeOfTriples() * 3, points->GetPoints(), GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points->GetPoints()[0]) * points->GetPointsSize() * 3, sizeof(points->GetPoints()[0]) * 90 * triples->GetSizeOfTriples() * 3, circlePoints);
	//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(graph->GetEdges()[0]) * 2 * graph->GetMaxNoOfEdges()));
	glEnableVertexAttribArray(0);

	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

PlaneSweepTriangulation::~PlaneSweepTriangulation()
{
	delete graph;
	delete boundary;
	delete triples;
	delete[] circlePoints;
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







void PlaneSweepTriangulation::drawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 3.1415926 * 2 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor 

	float radial_factor = cosf(theta);//calculate the radial factor 

	float x = r;//we start at angle = 0 

	float y = 0;
	for (int ii = 0; ii < num_segments; ii++)
	{
		circlePoints[3 * ii] = x + cx;
		circlePoints[3 * ii + 1] = y + cy;
		circlePoints[3 * ii + 2] = 0;

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

