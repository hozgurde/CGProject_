#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <cmath>

#include <chrono>
#include <time.h>

#include <GL/glew.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Points.h"
#include "Shader.h"
#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


using namespace std;

//window dimensions
const GLint WIDTH = 800, HEIGHT = 800;

Window mainWindow;
vector<Shader*> shaderList;




// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

//Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateShader() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

double xToLocalPosition(double x) {
	return (x / WIDTH * 2 - 1);
}

double yToLocalPosition(double y) {
	return -(y / HEIGHT * 2 - 1);
}

double scroll = 0;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	scroll += yoffset;
}

int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	

	CreateShader();
	glEnable(GL_PROGRAM_POINT_SIZE);
	glfwSetInputMode(mainWindow.GetGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetScrollCallback(mainWindow.GetGLFWwindow(), scroll_callback);

	GLuint uniformProjection = 0, uniformModel = 0;

	//glm::mat4 projection = 
	glm::mat4 projection = glm::perspective(90.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 1000.0f);

	Points points(50);

	//Create ImGUI components
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.GetGLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();

	glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f);
	float rot = 0.0f;
	float scale = 1.0f;

	//cursor positions
	double xpos = 0;
	double ypos = 0;
	double prevscroll = scroll;

	// Loop until window closed
	while (!mainWindow.GetShouldClose()) {
		//Get and handle user input events
		glfwPollEvents();

		//Clear window
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Render Imgui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model(1.0f);



		//Handle Mouse Inputs
		int leftstate = glfwGetMouseButton(mainWindow.GetGLFWwindow(), GLFW_MOUSE_BUTTON_LEFT);
		int rightstate = glfwGetMouseButton(mainWindow.GetGLFWwindow(), GLFW_MOUSE_BUTTON_RIGHT);

		if (leftstate == GLFW_PRESS) {
			double curxpos;
			double curypos;
			glfwGetCursorPos(mainWindow.GetGLFWwindow(), &curxpos, &curypos);
			translate = glm::vec3(translate.x + (curxpos - xpos) / (WIDTH / 2), translate.y - (curypos - ypos) / (HEIGHT / 2), translate.x);
		}
		if (rightstate == GLFW_PRESS) {
			double curxpos;
			double curypos;
			glfwGetCursorPos(mainWindow.GetGLFWwindow(), &curxpos, &curypos);
			double prevangle = atan2(yToLocalPosition(ypos), xToLocalPosition(xpos));
			double curangle = atan2(yToLocalPosition(curypos), xToLocalPosition(curxpos));
			cout << "prevangle: " << curxpos << " curangle: " << curypos << endl;
			rot += (curangle - prevangle);
		}
		if ((scroll - prevscroll) > 0) {
			scale *= 11.0f / 10.0f;
		}
		else if ((scroll - prevscroll) < 0) {
			scale *= 9.0f / 10.0f;
		}
		prevscroll = scroll;

		glfwGetCursorPos(mainWindow.GetGLFWwindow(), &xpos, &ypos);
		
		model = glm::translate(model, translate);
		model = glm::rotate(model, rot, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		// Rendering
		points.RenderPoints();

		glUseProgram(0);

		{//Temp code

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.SwapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	return 0;
}