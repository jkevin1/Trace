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
	// TODO update dirty
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, spheres.size());
}

void SphereSystem::addSphere(float x, float y, float z, float r) {
	Sphere sphere{ x, y, z, r };
	spheres.push_back(sphere);
}