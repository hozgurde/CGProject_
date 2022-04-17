#define _USE_MATH_DEFINES

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>

#include <cmath>

#include <chrono>
#include <time.h>

#include <GL/glew.h>
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


int main() {

	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	


	CreateShader();
	glEnable(GL_PROGRAM_POINT_SIZE);

	GLuint uniformProjection = 0, uniformModel = 0;

	//glm::mat4 projection = 
	glm::mat4 projection = glm::perspective(90.0f, (GLfloat)mainWindow.GetBufferWidth() / (GLfloat)mainWindow.GetBufferHeight(), 0.1f, 1000.0f);

	Points points(50);

	//Create ImGUI components
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.GetGLFWwindow(), true);
	ImGui_ImplOpenGL3_Init("#version 150");
	ImGui::StyleColorsDark();

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

		
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.5f));
		//model = glm::rotate(model, rot, glm::vec3(1.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
		
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