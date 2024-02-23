#pragma once

#include <glad/glad.h>

namespace Engine {
namespace GFX {
	class Texture
	{
		unsigned int id;
		int width, height, nrChannels;
	public:
		Texture(const char* path);
		void use(GLenum texture);
		~Texture();
	};
}
}