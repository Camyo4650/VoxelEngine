#include "Chunk.h"
#include "World.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Constants.h"
#include <omp.h>

Game::Chunk::Chunk(World* world, ChunkPos chunkPos, Engine::GFX::TextureArray &texture, Engine::GFX::VBO * vbo) :
	mesh(texture, vbo),
	chunkPos(chunkPos),
	world(world)
{
	relX = chunkPos.x;
	relY = chunkPos.y;
	relZ = chunkPos.z;
}

bool Game::Chunk::isEmpty(int x, int y, int z)
{
	if (this == NULL) 
		return false;
	if (x < 0)  
		return this->Cx->isEmpty(C_sizeX - 1, y, z);
	else if (y < 0)  
		return this->Cy->isEmpty(x, C_sizeY - 1, z);
	else if (z < 0)  
		return this->Cz->isEmpty(x, y, C_sizeZ - 1);
	else if (x >= C_sizeX) 
		return this->Cx1->isEmpty(0, y, z);
	else if (y >= C_sizeY) 
		return this->Cy1->isEmpty(x, 0, z);
	else if (z >= C_sizeZ) 
		return this->Cz1->isEmpty(x, y, 0);
	else return this->blocks[x][y][z] == 0;
}

uint16_t Game::Chunk::getBlockId(uint8_t x, uint8_t y, uint8_t z) const
{
	return blocks[x][y][z];
}

void Game::Chunk::generateTerrain(std::vector<uint16_t> height)
{// nothing complex.. for now ;)
	uint16_t h[C_sizeX][C_sizeY];
	for (uint16_t y = 0; y < C_sizeY; y++)
	{
		for (uint16_t x = 0; x < C_sizeX; x++)
		{
			if (height.empty())
			{
			}
			else
			{
				h[x][y] = height[x + C_sizeX * y];
			}
		}
	}
#pragma omp parallel for
	for (uint16_t z = 0; z < C_sizeZ; z++)
	{
		for (uint16_t y = 0; y < C_sizeY; y++)
		{
			for (uint16_t x = 0; x < C_sizeX; x++)
			{
				int realZ = this->chunkPos.z * C_sizeZ + z;
				if (realZ < h[x][y] && realZ >= h[x][y] - 4)
				{
					this->blocks[x][y][z] = 2;					
					this->empty = false;
				}
				else if (realZ < h[x][y])
				{
					this->blocks[x][y][z] = 3;
					this->empty = false;
				}
				else if (realZ == h[x][y])
				{
					this->blocks[x][y][z] = 1;
					this->empty = false;
				}
				else
					this->blocks[x][y][z] = 0;
			}
		}
	}
}

void Game::Chunk::generateCaves(std::vector<bool> air)
{
	for (uint16_t z = 0; z < C_sizeZ; z++)
	{
		for (uint16_t y = 0; y < C_sizeY; y++)
		{
			for (uint16_t x = 0; x < C_sizeX; x++)
			{
				if (air[x + C_sizeX * (y + C_sizeY * z)]) 
				{
					blocks[x][y][z] = 0;
				}
			}
		}
	}
}

void Game::Chunk::generateMesh()
{

	for (int z = 0; z < C_sizeZ; z++)
	{
		for (int y = 0; y < C_sizeY; y++)
		{
			for (int x = 0; x < C_sizeX; x++)
			{
				uint16_t blockId = getBlockId(x, y, z);
				if (blockId > 0) {
					for (uint8_t face = 0; face < 6; face++) {
						int nx = x + nOffsets[face][0];
						int ny = y + nOffsets[face][1];
						int nz = z + nOffsets[face][2];

						uint16_t neighborBlockId = this->world->getBlockId(this->chunkPos, nx, ny, nz);
						if (neighborBlockId <= 0) {
							// Add the face to the mesh
							mesh.addFaceToMesh(x, y, z, face, blockId);
						}
					}
				}
			}
		}
	}
	mesh.generateMesh();
}

void Game::Chunk::modify(uint16_t blockId, int x, int y, int z)
{
	modified = true;
	redraw = true;
	blocks[x][y][z] = blockId;
}

void Game::Chunk::draw(const glm::mat4& view, const glm::mat4& projection)
{
	mesh.draw(view, projection, this->chunkPos);
}

Game::Chunk::~Chunk()
{
	delete(&mesh);
}