#include "common/shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <string>

glm::mat4 transform(float x, float y, float z)
{
	return {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {x, y, z, 1}};
}

// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f, // point 1
    1.0f,  -1.0f, 0.0f, // point 2
    0.0f,  1.0f,  0.0f, // point 3
};

float width = 1280 / 1.3;
float height = 720 / 1.3;

int main(int, char **)
{
	glewExperimental = true;
	// init glfw
	if (!glfwInit())
	{
		std::cerr << "Failed to init GLFW\n";
		return -1;
	}

	// make a window
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window =
	    glfwCreateWindow(width, height, "My Window", nullptr, nullptr);
	if (window == NULL)
	{
		std::cerr << "Failed to create a window\n";
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init glew
	glewExperimental = true;
	GLenum glewInitState = glewInit();
	if (glewInitState != GLEW_OK)
	{
		std::cerr << "Canno't init GLEW, Reason:\n"
		          << glewGetErrorString(glewInitState) << "\n";
		return -1;
	}

	// make gl context
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init Vertex Array Object
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// INITIALIZE TRIANGLE
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
	             g_vertex_buffer_data, GL_STATIC_DRAW);

	// Matrix for projection

	auto pid = LoadShaders("../shaders/SimpleVertexShader.vert",
	                       "../shaders/SimpleFragmentShader.frag");

	auto xPos = 0.1f;

	double previousTime = glfwGetTime();
	int frameCount = 0;

	// main loop
	do
	{
		double currentTime = glfwGetTime();
		frameCount++;
		if (currentTime - previousTime >= 1.0)
		{
			auto title = "My Window (" + std::to_string(frameCount) + " FPS)";
			glfwSetWindowTitle(window, title.c_str());

			frameCount = 0;
			previousTime = currentTime;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(pid);

		// draw a triangle
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		// change projection
		auto Projection =
		    glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

		auto View = glm::lookAt(glm::vec3(4, 3, 4), glm::vec3(0, 0, 0),
		                        glm::vec3(0, 1, 0));

		auto Model = glm::mat4(1);

		glm::mat4 MVP =
		    Projection * View * glm::rotate(xPos += 0.05, glm::vec3(0, 1, 0));

		GLuint MatrixID = glGetUniformLocation(pid, "MVP");

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwWindowShouldClose(window) == 0 &&
	         glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

	// exit window
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
