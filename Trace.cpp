#include "GLApplication.h"
#include "SphereSystem.h"
#include "GLUtils.h"
#include <stdlib.h>
#include <stdio.h>

// some RAII things to be cleaner:
struct VAO {	// only one is needed, bind immediately
	VAO() { glGenVertexArrays(1, &id); glBindVertexArray(id); }
	~VAO() { glDeleteVertexArrays(1, &id); }
	GLuint id;
};

class Tracer : public GLApplication {
public:
	Tracer(int width, int height) : GLApplication("Tracer", width, height, false) {
		spheres.startup();
		spheres.addSphere(0.0f, 0.0f, 0.0f, 0.5f);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		printf("Initialized\n");
	}

	virtual ~Tracer() override {

		printf("Terminated\n");
	}

	virtual void update(double dt) override {
//		printf("update(%lf)\n", dt);
	}

	virtual void render() override {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		spheres.render();
	}

private:
	VAO vao;
	SphereSystem spheres;
};

int main(int argc, char* argv[]) {
	int width = 0, height = 0;
	
	if (argc == 3) {
		width = strtol(argv[1], nullptr, 10);
		height = strtol(argv[2], nullptr, 10);
	}

	if (width == 0 || height == 0) {
		width = 800;
		height = 600;
	}

	Tracer app(width, height);
	app.run();
}