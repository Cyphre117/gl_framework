#version 410

in vec3 vPosition;
in vec3 vColour;

out vec3 fColour;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    fColour = vColour;
    gl_Position = projection * view * vec4(vPosition, 1.0);
}
