
#ifndef GLHELPER_H
#define GLHELPER_H

#include <iostream>
#include <glad/glad.h>

#define glCall(x) GL_clearError(); \
	x; \
	GL_logCall(#x, __FILE__, __LINE__);

static void GL_clearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static void GL_logCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::cout << "[OpenGL error] " << error << "\n" << file << "\n" << function << "\n" << line << "\n";
		exit(-1);
	}
}

#endif