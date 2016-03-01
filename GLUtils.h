#pragma once

#include "glad/glad.h"
#include <initializer_list>

// load a shader from file
GLuint loadShader(const char* filename, GLenum type);

// create a shader program from a list of shaders
GLuint linkProgram(std::initializer_list<GLuint> shaders);

// helper function that does both stages in one call
GLuint createProgram(const char* vert, const char* frag);

GLuint createTexture(int width, int height, int samples, GLenum format);