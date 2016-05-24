#include "texture_handle.h"

TextureHandle::TextureHandle() :
wrap_s(GL_REPEAT),
wrap_t(GL_REPEAT),
min_filter(GL_LINEAR_MIPMAP_LINEAR),
mag_filter(GL_LINEAR_MIPMAP_LINEAR),
name_(0)
{}

void TextureHandle::bind( GLuint texture_unit )
{
	glActiveTexture( GL_TEXTURE0 + texture_unit );
	glBindTexture( GL_TEXTURE_2D, name_ );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_s );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter );
}