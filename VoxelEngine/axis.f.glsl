#version 330 core

flat in int Color; 
out vec4 FragColor;

void main()
{
    if (Color == 0) FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    else if (Color == 1) FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    else if (Color == 2) FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
    else FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
} 