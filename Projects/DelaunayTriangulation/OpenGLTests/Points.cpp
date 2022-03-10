#include "Points.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Points::Points()
{
	pointsSize = 0;
	VAO = 0;
	VBO = 0;
	points = NULL;
	CreateRandomPoints(0);
}

Points::Points(int noOfPoints)
{
	pointsSize = 0;
	VAO = 0;
	VBO = 0;
	CreateRandomPoints(noOfPoints);
}

Points::~Points()
{
	ClearPoints();
}

void Points::CreateRandomPoints(int noOfPoints)
{
	if (pointsSize != 0) {
		ClearPoints();
	}
	srand(time(NULL));
	pointsSize = noOfPoints;
	if (pointsSize > 0) {
		points = new GLfloat[pointsSize * 3];
		GLfloat p;
		for (int i = 0; i < pointsSize * 3; i++) {
			if (i % 3 == 2) {
				points[i] = 0.0;
			}
			else {
				p =  (rand()) / ((float)(RAND_MAX / 2)) - 1.0;
				points[i] = p;
			}
			printf("%f\n", points[i]);
		}


		//Generate and bind Vertex Array
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);


		//Generate and bind Vetex Buffer
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		//Put vertices into buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(points[0]) * pointsSize * 3, points, GL_STATIC_DRAW);

		//
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		//
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
	}
}

void Points::RenderPoints()
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glDrawArrays(GL_POINTS, 0, pointsSize);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Points::ClearPoints()
{
}

GLfloat* Points::GetPoints()
{
	return points;
}
