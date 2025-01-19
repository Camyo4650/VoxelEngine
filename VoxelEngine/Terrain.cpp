#include "Terrain.h"
#include "World.h"

Game::Terrain::Terrain()
{
	noise = FastNoise::New<FastNoise::Perlin>();
}

std::vector<uint16_t> Game::Terrain::getHeightMap(ChunkPos chunkPos)
{
	std::vector<float> layer1(C_sizeX * C_sizeY);
	//std::vector<float> layer2(C_sizeX * C_sizeY);
	/*
	ChunkPos id = { chunkPos.x, chunkPos.y, chunkPos.z };
	if (storedNoise.contains(id) && !storedNoise[id].heightMap.empty()) layer1 = storedNoise[id].heightMap;
	else {
		noise->GenUniformGrid2D(layer1.data(), chunkPos.x * C_sizeX, chunkPos.y * C_sizeY, C_sizeX, C_sizeY, 0.01, 1337);
		//noise->GenUniformGrid2D(layer2.data(), chunkPos.X * C_sizeX, chunkPos.Y * C_sizeY, C_sizeX, C_sizeY, 0.06, 1337);
		storedNoise[id].heightMap = layer1;
	}
	*/
	noise->GenUniformGrid2D(layer1.data(), chunkPos.x * C_sizeX, chunkPos.y * C_sizeY, C_sizeX, C_sizeY, 0.01, 1337);
	std::vector<uint16_t> height;
	for (int y = 0; y < C_sizeY; y++)
	{
		for (int x = 0; x < C_sizeX; x++)
		{
			height.push_back(4 * C_sizeZ * (4 + layer1[x + C_sizeX * y] / 4));
		}
	}

	return height;
}
std::vector<bool> Game::Terrain::getCaves(ChunkPos chunkPos)
{
	std::vector<float> cave1(C_sizeX * C_sizeY * C_sizeZ);
	std::vector<float> cave2(C_sizeX * C_sizeY * C_sizeZ);
	/*
	ChunkPos id = { chunkPos.x, chunkPos.y, chunkPos.z };
	if (storedNoise.contains(id) && !storedNoise[id].caveMap1.empty() && !storedNoise[id].caveMap2.empty())
	{
		cave1 = storedNoise[id].caveMap1;
		cave2 = storedNoise[id].caveMap2;
	}
	else {
		noise->GenUniformGrid3D(cave1.data(), chunkPos.x * C_sizeX, chunkPos.y * C_sizeY, chunkPos.z * C_sizeZ, C_sizeX, C_sizeY, C_sizeZ, 0.1, 1337);
		noise->GenUniformGrid3D(cave2.data(), chunkPos.x * C_sizeX, chunkPos.y * C_sizeY, chunkPos.z * C_sizeZ, C_sizeX, C_sizeY, C_sizeZ, 0.01, 1337);
		storedNoise[id].caveMap1 = cave1;
		storedNoise[id].caveMap2 = cave2;
	}
	*/
	noise->GenUniformGrid3D(cave1.data(), chunkPos.x * C_sizeX, chunkPos.y * C_sizeY, chunkPos.z * C_sizeZ, C_sizeX, C_sizeY, C_sizeZ, 0.1, 1337);
	noise->GenUniformGrid3D(cave2.data(), chunkPos.x * C_sizeX, chunkPos.y * C_sizeY, chunkPos.z * C_sizeZ, C_sizeX, C_sizeY, C_sizeZ, 0.01, 1337);
	std::vector<bool> caveData;
	for (int z = 0; z < C_sizeZ; z++)
	{
		for (int y = 0; y < C_sizeY; y++)
		{
			for (int x = 0; x < C_sizeX; x++)
			{
				caveData.push_back(cave1[x + C_sizeX * (y + C_sizeY * z)] > cave2[x + C_sizeX * (y + C_sizeY * z)]);
			}
		}
	}
	return caveData;
}
