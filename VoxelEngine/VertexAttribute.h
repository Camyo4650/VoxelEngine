#pragma once

#ifndef VERTEX_ATTRIBUTE_H
#define VERTEX_ATTRIBUTE_H
#include <glad/glad.h>
#include "GLHelper.h"

namespace Engine {
namespace GFX {
	struct VertexAttribute {
		int size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		const void* pointer;

		void enable(int index)
		{
			if (type == GL_BYTE || type == GL_UNSIGNED_BYTE || type == GL_SHORT || type == GL_UNSIGNED_SHORT || type == GL_INT || type == GL_UNSIGNED_INT)
			{
				glCall(glVertexAttribIPointer(index, size, type, stride, pointer));
			}
			else {
				glCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
			}
			glCall(glEnableVertexAttribArray(index));
		}
	};
}
}
#endif