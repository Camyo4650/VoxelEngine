#pragma once

#ifndef CHUNK_VERTEX_H
#define CHUNK_VERTEX_H

namespace Game {
namespace GFX {

	struct ChunkVertex
	{
		int position[3]; // relative to current chunk position
		int coords[3];
		int texCoords[2];
		int blockId;
		int faceNum; 
		//	0	FRONT	-X (X = 0)
		//	1	BACK	+X (X = 1)
		//	2	LEFT	-Y (Y = 0)
		//	3	RIGHT	+Y (Y = 1)
		//	4	BOTTOM	-Z (Z = 0)
		//	5	TOP		+Z (Z = 1)
	};
}
}

#endif