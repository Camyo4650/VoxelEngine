#pragma once

#include <glad/glad.h>

namespace Engine {
namespace GFX {
	class TextureArray
	{
		unsigned int id;
		int width, height, nrChannels;
	public:
		TextureArray(const char* path);
		void use(GLenum texture);
		~TextureArray();
	};
}
}