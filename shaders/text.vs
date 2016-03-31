#version 150 core

in vec2 position;
in vec2 vTexCoord;

out vec2 fTexCoord;

void main()
{
    fTexCoord = vTexCoord;
    gl_Position = vec4(position, 0.0, 1.0);
}
