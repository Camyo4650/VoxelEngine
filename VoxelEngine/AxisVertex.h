#pragma once

#ifndef AXIS_VERTEX_H
#define AXIS_VERTEX_H

namespace Game {
namespace GFX {
	struct AxisVertex {
		float pos[3]; // 0,0,0 1,0,0 0,1,0 0,0,1
		int color;
	};
}
}

#endif