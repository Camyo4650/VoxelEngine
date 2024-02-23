#pragma once

#include "GLHelper.h"
#include <glad/glad.h>

namespace Engine {
namespace GFX {
	class VAO
	{
		unsigned int vaoID;
	public:
		VAO() {
			glCall(glGenVertexArrays(1, &vaoID));
		}
		void bind()
		{
			glCall(glBindVertexArray(vaoID));
		}
	};
}
}