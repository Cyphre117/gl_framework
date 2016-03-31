#version 150 core

in vec2 fTexCoord;

out vec4 outColour;

uniform sampler2D font_bitmap;

void main()
{
   	vec4 sample = texture(font_bitmap, fTexCoord );

   	if( sample.g < 1.0f )
   	{
   		outColour = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
  	}
  	else
  	{
  		outColour = sample;
  	}
}
