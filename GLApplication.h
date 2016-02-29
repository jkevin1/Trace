#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLApplication {
public:
	GLApplication(const char* title, int width, int height, bool vsync = true);

	virtual ~GLApplication();

	virtual void update(double dt) = 0;
	virtual void render() = 0;

	void run();
	void quit();

protected:
	int getWidth();
	int getHeight();

	/* TODO event callback(s) */

private:
	GLFWwindow* window = nullptr;
	int frames;
	double time;
	double prev, curr;
};

