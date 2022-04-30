#pragma once

#include <GL\glew.h>

class Points
{
public:
	Points();
	Points(int noOfPoints);
	~Points();

	void CreateRandomPoints(int noOfPoints);
	void RenderPoints();
	void ClearPoints();
	GLfloat* GetPoints();
	int GetPointsSize() { return pointsSize; };

	

private:
	GLuint VAO, VBO;
	GLsizei pointsSize;
	GLfloat* points;
};

