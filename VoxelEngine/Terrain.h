#pragma once

#include <map>
#include <FastNoise/FastNoise.h>
#include "Chunk.h"

#define G_DEBUG

namespace Game
{
	struct NoiseData {
		std::vector<float> heightMap;
		std::vector<float> caveMap1;
		std::vector<float> caveMap2;
	};

	class Terrain
	{
		FastNoise::SmartNode<FastNoise::Perlin> noise;
		std::map<int, NoiseData> storedNoise;
	public:
		Terrain();
		std::vector<uint16_t> getHeightMap(ChunkPos chunkPos);
		std::vector<bool> getCaves(ChunkPos chunkPos);
	};
}