#version 330 core
layout (location = 0) in ivec3 aPos;
layout (location = 1) in ivec3 aCoords;
layout (location = 2) in ivec2 aTexCoord;
layout (location = 3) in int aBlockID;
layout (location = 4) in int aFace;

out vec2 TexCoord;
flat out int BlockID;
flat out int Face;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	BlockID = aBlockID;
	Face = aFace;
}