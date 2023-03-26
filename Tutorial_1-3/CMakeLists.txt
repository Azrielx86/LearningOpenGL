cmake_minimum_required(VERSION 3.0.0)
project(LearnOpenGL VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
add_library(shader ./common/shader.cpp)

include(CTest)
enable_testing()

add_executable(LearnOpenGL main.cpp)
target_link_libraries(LearnOpenGL glfw OpenGL::GL GLEW shader)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
