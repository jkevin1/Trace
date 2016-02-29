#include "GLUtils.h"
#include <stdio.h>

#ifdef _WIN32
#pragma warning( disable: 4996 )
#endif

GLuint loadShader(const char* filename, GLenum type) {
	// open the file
	FILE* file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Failed to open file: %s\n", filename);
		return 0;
	}

	// find size of the file
	fseek(file, 0, SEEK_END);
	GLsizei size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// load file into memory
	GLchar* src = new GLchar[size];
	fread(src, 1, size, file);
	fclose(file);

	// create and compile the shader
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, (const GLchar**)&src, &size);
	glCompileShader(id);
	delete[] src;

	// check compilation status
	GLint compiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
		char* error = new char[size];
		glGetShaderInfoLog(id, size, NULL, error);
		fprintf(stderr, "Error compiling %s:\n%s", filename, error);
		delete[] error;
		glDeleteShader(id);
		return 0;
	}
	
	printf("Loaded shader %s with handle %u\n", filename, id);
	return id;
}

GLuint linkProgram(std::initializer_list<GLuint> shaders) {
	// create and link shader program
	GLuint id = glCreateProgram();
	for (GLuint shader : shaders)
		glAttachShader(id, shader);
	glLinkProgram(id);

	// check compilation status
	GLint linked; GLsizei length;
	glGetProgramiv(id, GL_LINK_STATUS, &linked);
	if (!linked) {
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
		char* error = new char[length];
		glGetProgramInfoLog(id, length, NULL, error);
		fprintf(stderr, "Error linking program:\n%s", error);
		delete[] error;
		glDeleteShader(id);
		return 0;
	}

	printf("Created shader program %u\n", id);
	return id;
}

GLuint createProgram(const char* vert, const char* frag) {
	GLuint vs = loadShader(vert, GL_VERTEX_SHADER);
	GLuint fs = loadShader(frag, GL_FRAGMENT_SHADER);
	GLuint id = linkProgram({ vs, fs });
	glDeleteShader(vs);
	glDeleteShader(fs);
	return id;
}