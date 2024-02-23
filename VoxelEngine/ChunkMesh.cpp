#include "ChunkMesh.h"

Game::GFX::ChunkMesh::ChunkMesh() :
	Mesh(Shader("resources/shaders/chunk.v.glsl", "resources/shaders/chunk.f.glsl"), 
		{
			{3, GL_INT, GL_FALSE, 5 * sizeof(int), 0}
		})
{

}

void Game::GFX::ChunkMesh::addBufferData(ChunkVertex* data, size_t dataSize, GLenum usage)
{

}

void Game::GFX::ChunkMesh::draw()
{
}

Game::GFX::ChunkMesh::~ChunkMesh()
{
}
