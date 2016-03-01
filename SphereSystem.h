#pragma once

#include "GLUtils.h"
#include <vector>

class SphereSystem
{
public:
	SphereSystem();
	~SphereSystem();

	void startup(int width, int height, int num);
	void shutdown();

	void render();

	struct Sphere {
		float x, y, z;
		float radius;
	};

	Sphere& operator[](int i) { return spheres[i]; }

private:
	enum Consts {
		TEXTURES_PER_FBO = 4,		// number of textures per FBO
		NUM_FBOS = 2,
		NUM_TEXTURES = TEXTURES_PER_FBO * TEXTURES_PER_FBO
	};

	GLuint shader;
	std::vector<Sphere> spheres;
	GLuint textures[NUM_TEXTURES];
	GLuint fbos[NUM_FBOS];
	GLuint depth;
	int target;
};

