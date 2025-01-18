#version 450 core
layout (location = 0) in ivec3 aPos;
layout (location = 1) in int aBlockID;
layout (location = 2) in int aFace;

out vec2 TexCoord;
flat out int BlockID;
flat out int Face;

// Predefined vertex offsets for all faces
const vec3 vertexOffsets[6][6] = vec3[6][6](
    // -x face
    vec3[](vec3(0, 1, 1), vec3(0, 1, 0), vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, 1, 1)),
    // +x face
    vec3[](vec3(1, 0, 0), vec3(1, 1, 0), vec3(1, 1, 1), vec3(1, 1, 1), vec3(1, 0, 1), vec3(1, 0, 0)),
    // -y face
    vec3[](vec3(0, 0, 0), vec3(1, 0, 0), vec3(1, 0, 1), vec3(1, 0, 1), vec3(0, 0, 1), vec3(0, 0, 0)),
    // +y face
    vec3[](vec3(1, 1, 1), vec3(1, 1, 0), vec3(0, 1, 0), vec3(0, 1, 0), vec3(0, 1, 1), vec3(1, 1, 1)),
    // -z face
    vec3[](vec3(1, 1, 0), vec3(1, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0), vec3(0, 1, 0), vec3(1, 1, 0)),
    // +z face
    vec3[](vec3(0, 0, 1), vec3(1, 0, 1), vec3(1, 1, 1), vec3(1, 1, 1), vec3(0, 1, 1), vec3(0, 0, 1))
);

// Predefined texture coordinates for each vertex
const vec2 textureOffsets[6][6] = vec2[6][6](
    // -x face
    vec2[](vec2(1, 0), vec2(1, 1), vec2(0, 1), vec2(0, 1), vec2(0, 0), vec2(1, 0)),
    // +x face
    vec2[](vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(0, 0), vec2(1, 0), vec2(1, 1)),
    // -y face
    vec2[](vec2(0, 1), vec2(1, 1), vec2(1, 0), vec2(1, 0), vec2(0, 0), vec2(0, 1)),
    // +y face
    vec2[](vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0)),
    // -z face
    vec2[](vec2(0, 0), vec2(0, 1), vec2(1, 1), vec2(1, 1), vec2(1, 0), vec2(0, 0)),
    // +z face
    vec2[](vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(0, 0), vec2(1, 0), vec2(1, 1))
);

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calculate the vertex offset for this face and vertex
    vec3 offset = vertexOffsets[aFace][gl_VertexID % 6];
    vec2 texOffset = textureOffsets[aFace][gl_VertexID % 6];

    vec3 worldPos = aPos + offset;

	gl_Position = projection * view * model * vec4(worldPos, 1.0f);
	TexCoord = texOffset;
	BlockID = aBlockID;
	Face = aFace;
}