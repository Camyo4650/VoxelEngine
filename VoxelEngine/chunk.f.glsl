#version 450 core
const ivec2 atlasSize = ivec2(128, 128);
const ivec2 tileSize = ivec2(8, 8);

uniform sampler2DArray textureAtlas;
uniform float opacity;
uniform vec3 hint;

in vec2 TexCoord;
flat in int layer;
flat in int BlockID; 
flat in int Face;
out vec4 FragColor;

//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    vec3 faceColor = hint;
    // temporary "shadow"
    if (Face == 4) faceColor = faceColor*1/2.f;
    else if (Face != 5) faceColor = faceColor*2/3.f;

    //FragColor = vec4(vec3(texture(textureAtlas, (TexCoord+vec2(mod(tile, tileSize.x), floor(tile/tileSize.x)))*tileSize / atlasSize))*faceColor, opacity);
    FragColor = vec4(vec3(texture(textureAtlas, vec3(TexCoord, layer)))*faceColor, opacity);
    //FragColor = vec4(vec3(texture(textureAtlas, TexCoord))*faceColor, opacity);
    //FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
} 