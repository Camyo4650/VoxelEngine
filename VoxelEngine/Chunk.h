#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "ChunkVertex.h"
#include "TextureArray.h"
#include "Mesh.h"
#include "VBO.h"

#define C_sizeX 16
#define C_sizeY 16
#define C_sizeZ 64

namespace Game {
	struct ChunkPos
	{
		static const int SizeX = C_sizeX;
		static const int SizeY = C_sizeY;
		static const int SizeZ = C_sizeZ;
		static const int MaxZ = 16;

		const int32_t X, Y; // 2^32 * C_sizeX blocks wide
							// 2^32 * C_sizeY blocks long
		const int8_t Z;		// 2^8  * C_sizeZ blocks tall
	};

	class Chunk 
	{
		Engine::GFX::Mesh mesh;
		std::vector<GFX::ChunkVertex> vertices;
		Engine::GFX::TextureArray& texture;
		uint16_t blocks[C_sizeX][C_sizeY][C_sizeZ];
		bool modified;
		bool redraw;
		bool empty = true;
	public:
		//		-1			+1
		Chunk*	Cx; Chunk*	Cx1;
		Chunk*	Cy; Chunk*	Cy1;
		Chunk*	Cz; Chunk*	Cz1;
		int relX, relY, relZ;
		const ChunkPos chunkPos;
		Chunk(Engine::GFX::TextureArray& texture, ChunkPos chunkPos, Engine::GFX::VBO * vbo);
		bool isEmpty() const {
			return empty;
		}
		bool isEmpty(int x, int y, int z);
		static int getChunkId(ChunkPos pos) {
			size_t hx = std::hash<int32_t>{}(pos.X);
			size_t hy = std::hash<int32_t>{}(pos.Y);
			size_t hz = std::hash<int8_t>{}(pos.Z);
			return hx ^ (hy << 1) ^ (hz << 2);
		};
		int ID() const {
			return getChunkId(this->chunkPos);
		};
		//  7  6  5  4  3  2  1  0
		//  _  _ -x +x -y +y -z +z
		uint8_t getExposedFaces(int x, int y, int z);
		void generateTerrain(std::vector<uint16_t> heightmap); // someday... algorithm ;)
		void generateCaves(std::vector<bool> air);
		void generateVertices();
		void modify(uint16_t blockId, int x, int y, int z);
		void draw(const glm::mat4& view, const glm::mat4& projection);
		~Chunk();
	};
}

#endif