#include "Mesh.h"


Engine::GFX::Mesh::Mesh(Shader shader, std::vector<VertexAttribute> vertexAttribs) :
	vao(),
	vbo(),
	vertexAttribs(vertexAttribs),
	shader(shader)
{
	init();
}

void Engine::GFX::Mesh::init()
{
	shader.use();
	vbo.bind(GL_ARRAY_BUFFER);
	vao.bind();
	int num = 0;
	for (auto& vAttrib : vertexAttribs)
	{
		vAttrib.enable(num++);
	}
}

void Engine::GFX::Mesh::addBufferData(const void* data, int dataSize, GLenum usage)
{
	shader.use();
	vbo.bind(GL_ARRAY_BUFFER);
	vao.bind();
	glCall(glBufferData(GL_ARRAY_BUFFER, dataSize, data, usage));
}

void Engine::GFX::Mesh::draw(int indices)
{
	if (indices == 0) return;
	shader.use();
	vbo.bind(GL_ARRAY_BUFFER);
	vao.bind();
	glCall(glDrawArrays(GL_TRIANGLES, 0, indices));
	glCall(glBindVertexArray(0));
}

Engine::GFX::Mesh::~Mesh()
{

}
