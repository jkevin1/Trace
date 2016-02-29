#pragma once

#include "GLUtils.h"
#include <vector>

class SphereSystem
{
public:
	SphereSystem();
	~SphereSystem();

	void startup();
	void shutdown();

	void render();

	struct Sphere {
		float x, y, z;
		float radius;
	};

	Sphere& operator[](int i) { return spheres[i]; }
	void addSphere(float x, float y, float z, float r);

private:
	GLuint shader;
	std::vector<Sphere> spheres;
};

