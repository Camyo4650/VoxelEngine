#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in ivec3 aCoords;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aBlockID;
layout (location = 4) in float aFace;

out vec2 TexCoord;
out float BlockID;
out float Face;
out float yiay;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	BlockID = aBlockID;
	Face = aFace;
	if (aPos.x == 0.f) yiay = 0.f;
	else yiay = 1.f;
}