#include "Chunk.h"
#include <glm/ext/matrix_transform.hpp>
#include "Constants.h"

std::vector<Engine::GFX::VertexAttribute> ChunkVertexAttribs = {
	{3, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), 0},
	{3, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(3 * sizeof(int))},
	{2, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(6 * sizeof(int))},
	{1, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(8 * sizeof(int))},
	{1, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(9 * sizeof(int))}
};


void Game::GFX::ChunkMesh::addFaceToMesh(int x, int y, int z, int face, int blockId)
{

#define face(offset) ((6 * face) + offset)

	std::vector<Game::GFX::ChunkVertex> vertsV = {
		{ x + vCoords[face(0)][0], y + vCoords[face(0)][1], z + vCoords[face(0)][2], x, y, z, vCoords[face(0)][3], vCoords[face(0)][4], blockId, face},
		{ x + vCoords[face(1)][0], y + vCoords[face(1)][1], z + vCoords[face(1)][2], x, y, z, vCoords[face(1)][3], vCoords[face(1)][4], blockId, face},
		{ x + vCoords[face(2)][0], y + vCoords[face(2)][1], z + vCoords[face(2)][2], x, y, z, vCoords[face(2)][3], vCoords[face(2)][4], blockId, face},
		{ x + vCoords[face(3)][0], y + vCoords[face(3)][1], z + vCoords[face(3)][2], x, y, z, vCoords[face(3)][3], vCoords[face(3)][4], blockId, face},
		{ x + vCoords[face(4)][0], y + vCoords[face(4)][1], z + vCoords[face(4)][2], x, y, z, vCoords[face(4)][3], vCoords[face(4)][4], blockId, face},
		{ x + vCoords[face(5)][0], y + vCoords[face(5)][1], z + vCoords[face(5)][2], x, y, z, vCoords[face(5)][3], vCoords[face(5)][4], blockId, face},
	};
	this->vertices.insert(vertices.end(), vertsV.begin(), vertsV.end());
}

void Game::GFX::ChunkMesh::generateMesh()
{
	Game::GFX::ChunkVertex* verts = this->vertices.data();
	mesh.addBufferData(verts, this->vertices.size() * sizeof(Game::GFX::ChunkVertex), GL_DYNAMIC_DRAW);
}

Game::GFX::ChunkMesh::ChunkMesh(Engine::GFX::TextureArray& texture, Engine::GFX::VBO* vbo) :
	mesh(Engine::GFX::Shader("chunk.v.glsl", "chunk.f.glsl"),
		ChunkVertexAttribs),
	texture(texture)
{
	mesh.shader.use();
	mesh.shader.setInt("textureAtlas", 0);
}

void Game::GFX::ChunkMesh::draw(const glm::mat4& view, const glm::mat4& projection, const ChunkPos& chunkPos)
{
	if (redraw)
	{
		redraw = false;
		mesh.addBufferData(this->vertices.data(), sizeof(this->vertices.data()), GL_DYNAMIC_DRAW);
	}
	texture.use(GL_TEXTURE0);
	mesh.shader.use();
	mesh.shader.setMat4("projection", projection);
	mesh.shader.setMat4("view", view);
	mesh.shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(chunkPos.x * C_sizeX * 1.0f, chunkPos.y * C_sizeY * 1.0f, chunkPos.z * C_sizeZ * 1.0f)));
	mesh.shader.setFloat("opacity", 1.0f);
	mesh.shader.setVec3("hint", glm::vec3(1.0f, 1.0f, 1.0f));
	mesh.draw(this->vertices.size());
	/*
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		mesh.shader.setMat4("model", model);

		glCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}*/
}

Game::GFX::ChunkMesh::~ChunkMesh()
{
	delete(&vertices);
}