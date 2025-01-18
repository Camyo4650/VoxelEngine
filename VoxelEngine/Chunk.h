#pragma once

#include <vector>
#include "ChunkVertex.h"
#include "TextureArray.h"
#include "Mesh.h"
#include "VBO.h"

#define C_sizeX 16
#define C_sizeY 16
#define C_sizeZ 16

namespace Game {
	class World;

	struct ChunkPos
	{
		static const int SizeX = C_sizeX;
		static const int SizeY = C_sizeY;
		static const int SizeZ = C_sizeZ;
		static const int MaxZ = 64;

		int32_t x, y; // 2^32 * C_sizeX blocks wide
		// 2^32 * C_sizeY blocks long
		int8_t z;		// 2^8  * C_sizeZ blocks tall

		bool operator ==(const ChunkPos& other) const {
			return this->x == other.x && this->y == other.y && this->z == other.z;
		}

		bool operator !=(const ChunkPos& other) const {
			return this->x != other.x || this->y != other.y || this->z != other.z;
		}
	};

	namespace GFX {
		class ChunkMesh
		{
		private:
			std::vector<ChunkVertex> vertices;
		public:
			bool redraw;
			Engine::GFX::Mesh mesh;
			Engine::GFX::TextureArray& texture;

			ChunkMesh(Engine::GFX::TextureArray& texture, Engine::GFX::VBO* vbo);
			void draw(const glm::mat4& view, const glm::mat4& projection, const ChunkPos& chunkPos);
			void addFaceToMesh(int x, int y, int z, int face, int blockId);
			void generateMesh();
			~ChunkMesh();
		};
	}

	class Chunk
	{
		GFX::ChunkMesh mesh;
		uint16_t blocks[C_sizeX][C_sizeY][C_sizeZ];
		bool modified;
		bool redraw;
		bool empty = true;
		World* world;
	public:
		//		-1			+1
		Chunk* Cx; Chunk* Cx1;
		Chunk* Cy; Chunk* Cy1;
		Chunk* Cz; Chunk* Cz1;
		int relX, relY, relZ;
		const ChunkPos chunkPos;

		Chunk(World* world, ChunkPos chunkPos, Engine::GFX::TextureArray& texture, Engine::GFX::VBO* vbo);

		bool isEmpty() const {
			return empty;
		}
		bool isEmpty(int x, int y, int z);

		uint16_t getBlockId(uint8_t x, uint8_t y, uint8_t z) const;
		void setBlockId(uint8_t x, uint8_t y, uint8_t z, uint16_t blockId);

		//  7  6  5  4  3  2  1  0
		//  _  _ -x +x -y +y -z +z
		bool isFaceExposed(int x, int y, int z, int face);
		void generateTerrain(std::vector<uint16_t> heightmap); // someday... algorithm ;)
		void generateCaves(std::vector<bool> air);
		void generateMesh();
		void modify(uint16_t blockId, int x, int y, int z);
		void draw(const glm::mat4& view, const glm::mat4& projection);
		~Chunk();
	};
};


// Hash function for ChunkPos
namespace std {
	template <>
	struct hash<Game::ChunkPos> {
		std::size_t operator()(const Game::ChunkPos& pos) const {
			size_t seed = 1099511628211ULL; // A large prime (FNV offset basis)
			size_t hx = std::hash<int32_t>{}(pos.x);
			size_t hy = std::hash<int32_t>{}(pos.y);
			size_t hz = std::hash<int32_t>{}(pos.z);

			// Mix the hash values with golden ratio
			seed ^= hx + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= hy + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= hz + 0x9e3779b9 + (seed << 6) + (seed >> 2);

			return seed;
		}
	};
}