#pragma once

#include <vector>

#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "VertexAttribute.h"

namespace Engine {
namespace GFX {
	class Mesh
	{
	private:
		VBO* vbo;
		VAO vao;
		std::vector<VertexAttribute> vertexAttribs;
		int dataSize = 0;
		void init();
	public:
		Shader shader;
		Mesh(Shader shader, std::vector<VertexAttribute> vertexAttribs);
		Mesh(Shader shader, std::vector<VertexAttribute> vertexAttribs, Engine::GFX::VBO *vbo);
		void addBufferData(const void* data, int dataSize, GLenum usage);
		void draw(int indices);
		~Mesh();
	};
}
}