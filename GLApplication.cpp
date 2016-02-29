#include "GLApplication.h"
#include <stdio.h>
#include <assert.h>

/* TODO:
 * handle errors rather than assert
 * present/poll can be on separate threads
 * expose window parameters through constructor
 */

#define FRAME_COUNTER_PERIOD 1.0

// static initialization/termination of GLFW
class GLFW {
public:
	static void error(int id, const char* msg) { fprintf(stderr, "GLFW Error %d: %s\n", id, msg); }
	GLFW() { glfwSetErrorCallback(error);  assert(glfwInit()); printf("Initialized GLFW\n"); }
	~GLFW() { glfwTerminate(); printf("Terminated GLFW\n"); }
} instance;

GLApplication::GLApplication(const char* title, int width, int height, bool vsync) {
	// set window parameters
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	assert(window);

	// initialize context
	glfwMakeContextCurrent(window);
	glfwSwapInterval(vsync ? 1 : 0);

	// load OpenGL functions
	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
	glViewport(0, 0, width, height);

}

GLApplication::~GLApplication() {
	glfwDestroyWindow(window);
}

void GLApplication::run() {
	// initialize frame counter
	frames = 0;
	time = 0.0;
	curr = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		
		prev = curr;
		curr = glfwGetTime();
		const double dt = curr - prev;
		time += dt;
		frames++;

		const double period = FRAME_COUNTER_PERIOD;
		if (time >= period) {
			const double fps = frames / period;
			printf("Average Frame Time: %.4fms (%.2ffps)\n", 1000.0 / fps, fps);
			time -= period;
			frames = 0;
		}

		update(dt);
		render();
		
		glfwSwapBuffers(window);
	}
}

void GLApplication::quit() {
	glfwSetWindowShouldClose(window, GL_TRUE);
}

int GLApplication::getWidth() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return width;
}

int GLApplication::getHeight() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	return height;
}