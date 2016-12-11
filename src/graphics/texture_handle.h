#ifndef TEXTURE_HANDLE_H
#define TEXTURE_HANDLE_H

#define GLEW_STATIC
#include <GL/glew.h>

struct TextureHandle
{
	TextureHandle();
	~TextureHandle() {}

	GLint wrap_s;
	GLint wrap_t;
	GLint min_filter;
	GLint mag_filter;

	void bind( GLuint texture_unit );

private:
	friend class TextureManager;

	GLuint name_;
};

#endif