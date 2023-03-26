#ifndef __SHADERS_CPP__
#define __SHADERS_CPP__
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <vector>

GLuint LoadShaders(const char *vertex_file_path,
                   const char *fragment_file_path);

#endif