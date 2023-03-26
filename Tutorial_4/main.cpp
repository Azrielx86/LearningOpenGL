#include "common/shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <string>

float width = 1280 / 1.3;
float height = 720 / 1.3;

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, -1.0f, // Cube starts here, triangle 1
    -1.0f, -1.0f, 1.0f,  //
    -1.0f, 1.0f,  1.0f,  // triangle 1
    1.0f,  1.0f,  -1.0f, // triangle 2
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  -1.0f, // triangle 2
    1.0f,  -1.0f, 1.0f,  // triangle n
    -1.0f, -1.0f, -1.0f, //
    1.0f,  -1.0f, -1.0f, // triangle n
    1.0f,  1.0f,  -1.0f, //
    1.0f,  -1.0f, -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    -1.0f, 1.0f,  -1.0f, //
    1.0f,  -1.0f, 1.0f,  //
    -1.0f, -1.0f, 1.0f,  //
    -1.0f, -1.0f, -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    -1.0f, -1.0f, 1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  -1.0f, -1.0f, //
    1.0f,  1.0f,  -1.0f, //
    1.0f,  -1.0f, -1.0f, //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  -1.0f, 1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    1.0f,  1.0f,  -1.0f, //
    -1.0f, 1.0f,  -1.0f, //
    1.0f,  1.0f,  1.0f,  //
    -1.0f, 1.0f,  -1.0f, //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  1.0f,  1.0f,  //
    -1.0f, 1.0f,  1.0f,  //
    1.0f,  -1.0f, 1.0f,  // Cube ends here
    -3.0f, -1.0f, -1.0f, // triangle starts here
    -3.0f, -1.0f, 1.0f,  //
    -3.0f, 1.0f,  1.0f,  //
};

int main(int, char **)
{
	srand(time(0));
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

	// Init cube
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
	             g_vertex_buffer_data, GL_STATIC_DRAW);

	static GLfloat g_color_buffer_data[13 * 3 * 3];
	for (int v = 0; v < 13 * 3; v++)
	{
		// 46, 204, 113
		g_color_buffer_data[3 * v + 0] = (rand() % 255) / 255.0f;
		g_color_buffer_data[3 * v + 1] = (rand() % 255) / 255.0f;
		g_color_buffer_data[3 * v + 2] = (rand() % 255) / 255.0f;
	}

	// Init color buffer
	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
	             g_color_buffer_data, GL_STATIC_DRAW);

	// matrix for projections
	auto pid = LoadShaders("../shaders/SimpleVertexShader.vert",
	                       "../shaders/SimpleFragmentShader.frag");

	float xPos = 0.1f;
	double previousTime = glfwGetTime();
	int frameCount = 0;
	float offset = 0.0f;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

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

		// vertex buffer
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		glDrawArrays(GL_TRIANGLES, 0, 3 + (3 * 12));
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
		             g_color_buffer_data, GL_STATIC_DRAW);
		for (int v = 0; v < 13 * 3; v++)
		{
			g_color_buffer_data[3 * v + 0] =
			    sin(0.003f * (offset) + 0.0f) * 0.5f + 0.5f; // red
			g_color_buffer_data[3 * v + 1] =
			    sin(0.003f * (offset) + 2.0f) * 0.5f + 0.5f; // green
			g_color_buffer_data[3 * v + 2] =
			    sin(0.003f * (offset) + 4.0f) * 0.5f + 0.5f; // blue
			offset += 0.1f;
		}

		// color buffer
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
		// glDisableVertexAttribArray(1);

		// change projection here!
		auto projection =
		    glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

		auto view = glm::lookAt(glm::vec3(4, 3, 4), glm::vec3(0, 0, 0),
		                        glm::vec3(0, 1, 0));

		auto model = glm::mat4(1);

		glm::mat4 MVP =
		    projection * view * glm::rotate(xPos += 0.05, glm::vec3(0, 1, 0));
		// glm::rotate(xPos += 0.05, glm::vec3(0, 1, 0));

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
