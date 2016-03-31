#version 410

//uniform sampler2D tex;

//in vec3 fColour;
//in vec2 fTexCoord;

out vec4 outColour;

void main()
{
   //vec4 texCol = texture(tex, fTexCoord);
   /*
   if( texCol.r < 0.5 ) {
       outColour = line;
   } else {
       outColour = texture(tex, fTexCoord) * tint;
   }
   */
   outColour = vec4( 0.5f, 0.5f, 1.0f, 1.0f );
}
