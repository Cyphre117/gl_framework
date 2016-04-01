#version 410

uniform sampler2D tex;

in vec2 fTexCoord;

out vec4 outColour;

void main()
{
    outColour = texture(tex, fTexCoord);
}
