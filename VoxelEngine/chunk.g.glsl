#version 450 core
layout (points) in;                       // Input: one point per cube
layout (triangle_strip, max_vertices = 4) out; // Output: 4 vertices for one face

in ivec3 vPos[];         // Position passed from vertex shader
flat in int vBlockID[]; // Block ID passed from vertex shader
flat in int vFace[];    // Face index passed from vertex shader (0 to 5)

out vec2 TexCoord;      // Texture coordinates for fragment shader
flat out int BlockID;   // Block ID for fragment shader

// Predefined vertex offsets for all faces
const vec3 faceOffsets[6][4] = vec3[6][4](
    // -x face
    vec3[](vec3(0, 1, 1), vec3(0, 0, 1), vec3(0, 1, 0), vec3(0, 0, 0)),
    // +x face
    vec3[](vec3(1, 1, 0), vec3(1, 0, 0), vec3(1, 1, 1), vec3(1, 0, 1)),
    // -y face
    vec3[](vec3(0, 0, 1), vec3(1, 0, 1), vec3(0, 0, 0), vec3(1, 0, 0)),
    // +y face
    vec3[](vec3(0, 1, 0), vec3(1, 1, 0), vec3(0, 1, 1), vec3(1, 1, 1)),
    // -z face
    vec3[](vec3(1, 1, 0), vec3(0, 1, 0), vec3(1, 0, 0), vec3(0, 0, 0)),
    // +z face
    vec3[](vec3(0, 1, 1), vec3(1, 1, 1), vec3(0, 0, 1), vec3(1, 0, 1))
);

// Predefined texture coordinates for each face
const vec2 texOffsets[6][4] = vec2[6][4](
    // -x face
    vec2[](vec2(1, 0), vec2(1, 1), vec2(0, 0), vec2(0, 1)),
    // +x face
    vec2[](vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, 1)),
    // -y face
    vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1)),
    // +y face
    vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1)),
    // -z face
    vec2[](vec2(0, 0), vec2(1, 0), vec2(0, 1), vec2(1, 1)),
    // +z face
    vec2[](vec2(1, 1), vec2(0, 1), vec2(0, 0), vec2(1, 0))
);

void main()
{
    vec3 cubePos = vPos[0]; // Base position of the cube
    int blockID = vBlockID[0]; // Block ID of the cube
    int face = vFace[0]; // Face index to render (0 to 5)

    for (int vertex = 0; vertex < 4; vertex++) {
        vec3 worldPos = cubePos + faceOffsets[face][vertex];
        TexCoord = texOffsets[face][vertex];
        BlockID = blockID;
        gl_Position = vec4(worldPos, 1.0);
        EmitVertex();
    }
    EndPrimitive();
}
