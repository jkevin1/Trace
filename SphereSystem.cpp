#include "SphereSystem.h"

SphereSystem::SphereSystem() {
	shader = 0;
}

SphereSystem::~SphereSystem() {
	shutdown();
}

void SphereSystem::startup() {
	shader = createProgram("sphere.vert", "sphere.frag");
}

void SphereSystem::shutdown() {
	glDeleteProgram(shader);
	shader = 0;
}

void SphereSystem::render() {
	glUseProgram(shader);
	// TODO update dirty SSBO data
//	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, spheres.size());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glUseProgram(0);
}

void SphereSystem::addSphere(float x, float y, float z, float r) {
	Sphere sphere{ x, y, z, r };
	spheres.push_back(sphere);
}