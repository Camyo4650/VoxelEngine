#include "Chunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::vector<Engine::GFX::VertexAttribute> ChunkVertexAttribs = {
	{3, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), 0},
	{2, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(3 * sizeof(int))},
	{1, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(5 * sizeof(int))},
	{1, GL_INT, GL_FALSE, sizeof(Game::GFX::ChunkVertex), (void*)(6 * sizeof(int))}
};

Game::Chunk::Chunk(Engine::GFX::Texture &texture, ChunkPos chunkPos, Engine::GFX::VBO * vbo) :
	mesh(Engine::GFX::Shader("chunk.v.glsl", "chunk.f.glsl"),
		ChunkVertexAttribs, vbo),
	chunkPos(chunkPos),
	texture(texture)
{
	mesh.shader.use();
	mesh.shader.setInt("textureAtlas", 0);
	//mesh.addBufferData(vertices.data(), vertices.size() * sizeof(float), GL_DYNAMIC_DRAW);
	for (uint16_t z = 0; z < C_sizeZ; z++)
	{
		for (uint16_t y = 0; y < C_sizeY; y++)
		{
			for (uint16_t x = 0; x < C_sizeX; x++)
			{
				if (z < 2)
					blocks[x][y][z] = 1;
				else
					blocks[x][y][z] = 0;
			}
		}
	}
}

bool Game::Chunk::isEmpty(int x, int y, int z)
{
	if (this == NULL) 
		return true;
	if (x < 0)  
		return Cx->isEmpty(C_sizeX - 1, y, z);
	else if (y < 0)  
		return Cy->isEmpty(x, C_sizeY - 1, z);
	else if (z < 0)  
		return Cz->isEmpty(x, y, C_sizeZ - 1);
	else if (x >= C_sizeX) 
		return Cx1->isEmpty(0, y, z);
	else if (y >= C_sizeY) 
		return Cy1->isEmpty(x, 0, z);
	else if (z >= C_sizeZ) 
		return Cz1->isEmpty(x, y, 0);
	else return blocks[x][y][z] == 0;
}

//  7  6  5  4  3  2  1  0
//  _  _ -x +x -y +y -z +z
uint8_t Game::Chunk::getExposedFaces(int x, int y, int z)
{
	uint8_t faces = 0;

	faces = isEmpty(x - 1, y, z) << 5 
		| isEmpty(x + 1, y, z) << 4
		| isEmpty(x, y - 1, z) << 3
		| isEmpty(x, y + 1, z) << 2
		| isEmpty(x, y, z - 1) << 1
		| isEmpty(x, y, z + 1);

	return faces;
}

void Game::Chunk::generate()
{// nothing complex.. for now ;)
	
	for (int z = 0; z < C_sizeZ; z++)
	{
		for (int y = 0; y < C_sizeY; y++)
		{
			for (int x = 0; x < C_sizeX; x++)
			{
				uint8_t exposed = getExposedFaces(x, y, z);
				if (blocks[x][y][z] != 0 && exposed != 0)
				{
					int id = blocks[x][y][z];
					if (exposed & 1 << 5) {
						// -x exposed
						std::vector<Game::GFX::ChunkVertex> verts = { 
							{ x + 0, y + 1, z + 1,  1, 1, id, 0 },
							{ x + 0, y + 1, z + 0,  1, 0, id, 0 },
							{ x + 0, y + 0, z + 0,  0, 0, id, 0 },
							{ x + 0, y + 0, z + 0,  0, 0, id, 0 },
							{ x + 0, y + 0, z + 1,  0, 1, id, 0 },
							{ x + 0, y + 1, z + 1,  1, 1, id, 0 },
						};
						this->vertices.insert(vertices.end(), verts.begin(), verts.end());
					}
					if (exposed & 1 << 4) {
						// +x exposed
						std::vector<Game::GFX::ChunkVertex> verts = {
							{ x + 1, y + 0, z + 0,  1, 0, id, 1 },
							{ x + 1, y + 1, z + 0,  0, 0, id, 1 },
							{ x + 1, y + 1, z + 1,  0, 1, id, 1 },
							{ x + 1, y + 1, z + 1,  0, 1, id, 1 },
							{ x + 1, y + 0, z + 1,  1, 1, id, 1 },
							{ x + 1, y + 0, z + 0,  1, 0, id, 1 },
						};
						this->vertices.insert(vertices.end(), verts.begin(), verts.end());
					}
					if (exposed & 1 << 3) {
						// -y exposed
						std::vector<Game::GFX::ChunkVertex> verts = {
							{ x + 0, y + 0, z + 0,  0, 0, id, 2 },
							{ x + 1, y + 0, z + 0,  1, 0, id, 2 },
							{ x + 1, y + 0, z + 1,  1, 1, id, 2 },
							{ x + 1, y + 0, z + 1,  1, 1, id, 2 },
							{ x + 0, y + 0, z + 1,  0, 1, id, 2 },
							{ x + 0, y + 0, z + 0,  0, 0, id, 2 },
						};
						this->vertices.insert(vertices.end(), verts.begin(), verts.end());
					}
					if (exposed & 1 << 2) {
						// +y exposed
						std::vector<Game::GFX::ChunkVertex> verts = {
							{ x + 1, y + 1, z + 1,  0, 1, id, 3 },
							{ x + 1, y + 1, z + 0,  0, 0, id, 3 },
							{ x + 0, y + 1, z + 0,  1, 0, id, 3 },
							{ x + 0, y + 1, z + 0,  1, 0, id, 3 },
							{ x + 0, y + 1, z + 1,  1, 1, id, 3 },
							{ x + 1, y + 1, z + 1,  0, 1, id, 3 },
						};
						this->vertices.insert(vertices.end(), verts.begin(), verts.end());
					}
					if (exposed & 1 << 1) {
						// -z exposed
						std::vector<Game::GFX::ChunkVertex> verts = {
							{ x + 1, y + 1, z + 0,  0, 1, id, 4 },
							{ x + 1, y + 0, z + 0,  0, 0, id, 4 },
							{ x + 0, y + 0, z + 0,  1, 0, id, 4 },
							{ x + 0, y + 0, z + 0,  1, 0, id, 4 },
							{ x + 0, y + 1, z + 0,  1, 1, id, 4 },
							{ x + 1, y + 1, z + 0,  0, 1, id, 4 },
						};
						this->vertices.insert(vertices.end(), verts.begin(), verts.end());
					}
					if (exposed & 1) {
						// +z exposed
						std::vector<Game::GFX::ChunkVertex> verts = {
							{ x + 0, y + 0, z + 1,  0, 0, id, 5 },
							{ x + 1, y + 0, z + 1,  1, 0, id, 5 },
							{ x + 1, y + 1, z + 1,  1, 1, id, 5 },
							{ x + 1, y + 1, z + 1,  1, 1, id, 5 },
							{ x + 0, y + 1, z + 1,  0, 1, id, 5 },
							{ x + 0, y + 0, z + 1,  0, 0, id, 5 },
						};
						this->vertices.insert(vertices.end(), verts.begin(), verts.end());
					}
				}
			}
		}
	}
	Game::GFX::ChunkVertex *verts = this->vertices.data();
	mesh.addBufferData(verts, this->vertices.size() * sizeof(Game::GFX::ChunkVertex), GL_DYNAMIC_DRAW);
	
}

void Game::Chunk::modify(uint16_t blockId, int x, int y, int z)
{
	modified = true;
	redraw = true;
	blocks[x][y][z] = blockId;
}

void Game::Chunk::draw(const glm::mat4& view, const glm::mat4& projection)
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
	mesh.shader.setMat4("model", glm::translate(glm::mat4(1.0f), glm::vec3(chunkPos.X * C_sizeX * 1.0f, chunkPos.Y * C_sizeY * 1.0f, chunkPos.Z * C_sizeZ * 1.0f)));
	mesh.shader.setFloat("opacity", 1.0f);
	mesh.shader.setVec3("hint", glm::vec3(1.0f,1.0f,1.0f));
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
	}
	*/
}

Game::Chunk::~Chunk()
{

}
