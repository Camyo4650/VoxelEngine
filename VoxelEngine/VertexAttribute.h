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
			glCall(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
			glCall(glEnableVertexAttribArray(index));
		}
	};
}
}
#endif