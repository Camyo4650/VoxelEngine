#version 330 core
const ivec2 atlasSize = ivec2(128, 128);
const ivec2 tileSize = ivec2(8, 8);

uniform sampler2D textureAtlas;
uniform float opacity;
uniform vec3 hint;

in vec2 TexCoord;
in float BlockID; 
in float Face;
in float yiay;
out vec4 FragColor;

//in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    vec3 faceColor = hint;
    // temporary "shadow"
    if (Face == 4) faceColor = faceColor*1/2.f;
    else if (Face != 5) faceColor = faceColor*2/3.f;

    float tile = 0.0f;
    if (BlockID == 1)
    {
        if (Face == 5) tile = 0.0f;
        else if (Face == 4) tile = 2.0f;
        else tile = 1.0f;
    }
    else {
        tile = 2.0f;
    }
    FragColor = vec4(vec3(texture(textureAtlas, (TexCoord+vec2(mod(tile, tileSize.x), floor(tile/tileSize.x)))*tileSize / atlasSize))*faceColor, opacity);
    //FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
} 