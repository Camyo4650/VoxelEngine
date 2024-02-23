#pragma once

#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "ChunkVertex.h"
#include "Texture.h"
#include "Mesh.h"
#include "VBO.h"

#define C_sizeX 16
#define C_sizeY 16
#define C_sizeZ 16

namespace Game {
	struct ChunkPos
	{
		const int32_t X, Y; // 2^32 * C_sizeX blocks wide
							// 2^32 * C_sizeY blocks long
		const int8_t Z;		// 2^8  * C_sizeZ blocks tall
	};

	class Chunk 
	{
		Engine::GFX::Mesh mesh;
		std::vector<GFX::ChunkVertex> vertices;
		Engine::GFX::Texture& texture;
		uint16_t blocks[C_sizeX][C_sizeY][C_sizeZ];
		bool modified;
		bool redraw;
	public:
		//		-1			+1
		Chunk*	Cx; Chunk*	Cx1;
		Chunk*	Cy; Chunk*	Cy1;
		Chunk*	Cz; Chunk*	Cz1;
		int relX, relY, relZ;
		const ChunkPos chunkPos;
		Chunk(Engine::GFX::Texture& texture, ChunkPos chunkPos, Engine::GFX::VBO * vbo);
		bool isEmpty(int x, int y, int z);
		//  7  6  5  4  3  2  1  0
		//  _  _ -x +x -y +y -z +z
		uint8_t getExposedFaces(int x, int y, int z);
		void generate(); // someday... algorithm ;)
		void modify(uint16_t blockId, int x, int y, int z);
		void draw(const glm::mat4& view, const glm::mat4& projection);
		~Chunk();
	};
}

#endif