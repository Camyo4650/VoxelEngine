#pragma once

#include <vector>
#include "ChunkVertex.h"
#include "TextureArray.h"
#include "Mesh.h"
#include "VBO.h"
#include <mutex>
#include <atomic>

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
			void addFaceToMesh(uint8_t x, uint8_t y, uint8_t z, uint8_t face, uint16_t blockId);
			void uploadToGPU();
			void clear();
			~ChunkMesh();
		};
	}

	struct ChunkPalette {
		// Palette: Maps indices to block types
		std::vector<uint16_t> palette;

		// Indices: 3D array mapping positions to indices in the palette
		std::vector<std::vector<std::vector<uint16_t>>> indices;

		// Initialize with chunk dimensions
		ChunkPalette(uint8_t width, uint8_t height, uint8_t depth)
			: indices(width, std::vector<std::vector<uint16_t>>(height, std::vector<uint16_t>(depth, 0))) {
		}

		// Add a block type to the palette and return its index
		uint16_t addToPalette(uint16_t blockID) {
			auto it = std::find(palette.begin(), palette.end(), blockID);
			if (it != palette.end()) {
				return std::distance(palette.begin(), it); // Return existing index
			}
			palette.push_back(blockID); // Add new blockID
			return palette.size() - 1; // Return new index
		}

		// Set a block at a position (x, y, z)
		void setBlock(uint8_t x, uint8_t y, uint8_t z, uint16_t blockID) {
			uint16_t index = addToPalette(blockID);
			indices[x][y][z] = index;
		}

		// Get the block ID at a position (x, y, z)
		int getBlock(uint8_t x, uint8_t y, uint8_t z) const {
			uint16_t index = indices[x][y][z];
			return palette[index];
		}
	};

	class Chunk
	{
		GFX::ChunkMesh mesh;
		ChunkPalette palette;
		bool modified;
		bool ready = false;
		bool empty = true;
		World* world;

		std::mutex meshMutex; // Protects vertex data
		std::atomic<bool> meshReady = false; // Indicates if the mesh is ready
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
		
		void load(ChunkPalette palette);

		//  7  6  5  4  3  2  1  0
		//  _  _ -x +x -y +y -z +z
		bool isFaceExposed(int x, int y, int z, int face);
		void generateTerrain(std::vector<uint16_t> heightmap); // someday... algorithm ;)
		void generateCaves(std::vector<bool> air);
		void generateMesh();
		void uploadToGPU();
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