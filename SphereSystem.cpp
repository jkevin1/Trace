#include "SphereSystem.h"
#include <stdio.h>

SphereSystem::SphereSystem() {
	shader = 0;
}

SphereSystem::~SphereSystem() {
	shutdown();
}

void SphereSystem::startup(int width, int height, int num) {
	shader = createProgram("sphere.vert", "sphere.frag");
	glEnable(GL_MULTISAMPLE);
	// create sphere data
	spheres.resize(num);


	GLint samples;
	glGetIntegerv(GL_MAX_SAMPLES, &samples);
	samples = 1;
	printf("Initializing MSAA render targets with %d samples\n", samples);

	const GLuint target = GL_TEXTURE_2D_MULTISAMPLE;
	const GLenum format[TEXTURES_PER_FBO] = {
		GL_RGBA32F, GL_RGB32F, GL_RGB32F, GL_RGB32F	// TODO tune these
	};

	// create render textures
	glGenTextures(NUM_TEXTURES, textures);
	for (int i = 0; i < NUM_TEXTURES; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(target, textures[i]);
		const int index = i % TEXTURES_PER_FBO;
		glTexStorage2DMultisample(target, samples, format[index], width, height, false);
	}

	glGenRenderbuffers(1, &depth);
	glBindRenderbuffer(GL_RENDERBUFFER, depth);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

	// create first framebuffer with depth attachment
	glGenFramebuffers(NUM_FBOS, fbos);
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[0]);
	for (int i = 0; i < TEXTURES_PER_FBO; i++) {
		const GLenum index = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, index, target, textures[i], 0);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Failed to create primary framebuffer (%u)\n", status);
	switch (status) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		fprintf(stderr, "Primary framebuffer invalid attachment\n");
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		fprintf(stderr, "Primary framebuffer draw buffer error\n");
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		fprintf(stderr, "Primary framebuffer layer targets error\n");
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		fprintf(stderr, "Primary framebuffer multisample error\n");
	}

	// create second framebuffer, no depth attachment
	glGenFramebuffers(NUM_FBOS, fbos); 
	glBindFramebuffer(GL_FRAMEBUFFER, fbos[1]);
	for (int i = 0; i < TEXTURES_PER_FBO; i++) {
		const GLenum index = GL_COLOR_ATTACHMENT0 + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, index, target, textures[TEXTURES_PER_FBO + i], 0);
	}
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		fprintf(stderr, "Failed to create secondary framebuffer (%u)\n", status);
	switch (status) {
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		fprintf(stderr, "Primary framebuffer invalid attachment\n");
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		fprintf(stderr, "Primary framebuffer draw buffer error\n");
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		fprintf(stderr, "Primary framebuffer layer targets error\n");
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		fprintf(stderr, "Primary framebuffer multisample error\n");
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GLenum error = glGetError();
	if (error) fprintf(stderr, "Error occurred: %d\n", error);
}

void SphereSystem::shutdown() {

	glDeleteFramebuffers(NUM_FBOS, fbos);
	for (int i = 0; i < NUM_FBOS; i++) fbos[i] = 0;

	glDeleteRenderbuffers(1, &depth);
	depth = 0;

	glDeleteTextures(NUM_TEXTURES, textures);
	for (int i = 0; i < NUM_TEXTURES; i++) textures[i] = 0;

	glDeleteProgram(shader);
	shader = 0;

	// free sphere memory
	spheres.swap(std::vector<Sphere>());
}

void SphereSystem::render() {
	glUseProgram(shader);
	// TODO update dirty SSBO data
//	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, spheres.size());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glUseProgram(0);
}