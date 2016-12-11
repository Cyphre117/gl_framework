#ifndef DEFAULT_TEXTURE_H
#define DEFAULT_TEXTURE_H

#define GLEW_STATIC
#include <GL/glew.h>

#define o 1.0f,1.0f,1.0f
#define x 1.0f,0.4f,0.0f

float default_texture_data[] =
{
	o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,x,o,x,o,x,x,o,x,o,x,x,x,x,x,o,
	o,x,o,x,o,o,x,x,o,x,x,x,x,x,x,o,
	o,x,o,x,o,o,x,o,x,o,x,x,x,x,x,o,
	o,x,x,x,x,x,x,x,x,x,x,x,x,x,x,o,
	o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,o,

};

#undef o
#undef x

#endif