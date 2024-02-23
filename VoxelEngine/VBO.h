#pragma once

#include "GLHelper.h"
#include <glad/glad.h>

namespace Engine {
namespace GFX {
	class VBO
	{
		unsigned int vboID;
	public:
		VBO() {
			glCall(glGenBuffers(1, &(this->vboID)));
		}
		void bind(GLenum target)
		{
			glCall(glBindBuffer(target, this->vboID));
		}
		~VBO() {

		}
	};
}
}