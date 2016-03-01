#include "SphereSystem.h"
#include <stdio.h>

SphereSystem::SphereSystem() {
	shader = 0;
}

SphereSystem::~SphereSystem() {
	shutdown();
}

void SphereSystem::startup(int width, int height) {
	shader = createProgram("sphere.vert", "sphere.frag");

	GLint samples;
	glGetIntegerv(GL_MAX_SAMPLES, &samples);
	printf("Initializing MSAA render targets with %d samples\n", samples);

	const GLuint target = GL_TEXTURE_2D_MULTISAMPLE;
	const GLenum format[TEXTURES_PER_FBO] = {
		GL_RGBA32F, GL_RGB32F, GL_RGB32F, GL_R8UI	// TODO tune these
	};

	// create render targets
	glGenTextures(NUM_TEXTURES, textures);
	for (int i = 0; i < NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(target, textures[i]);
		const int index = i % TEXTURES_PER_FBO;
		glTexStorage2DMultisample(target, samples, format[index], width, height, false);
	}

	// add render targets to 
	glGenFramebuffers(NUM_FBOS, fbos);
	for (int i = 0; i < NUM_FBOS; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
		for (int j = 0; j < TEXTURES_PER_FBO; j++) {
			const GLenum index = GL_COLOR_ATTACHMENT0 + j;
			const GLuint texture = textures[(i * TEXTURES_PER_FBO) + j];
			glFramebufferTexture2D(GL_FRAMEBUFFER, index, target, texture, 0);
		}
	}

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
}

void SphereSystem::shutdown() {
	glDeleteProgram(shader);
	shader = 0;
	spheres.clear();
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