#include "text_renderer.h"
#include "shader_program.h"
#include "window.h"
#include <string>

TextRenderer::TextRenderer( std::string filename, ShaderProgram* shader )
{	
	{
		std::string projectPath;    
		char* charPath = SDL_GetBasePath();    
		projectPath = charPath;    
		SDL_free(charPath);
		filename = projectPath + filename;
	}

	// Creat and bind the font texture
	glGenTextures( 1, &font_bitmap_ );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, font_bitmap_ );

    // Load the actual font file
    SDL_Surface* image = SDL_LoadBMP( filename.c_str() );
    if( !image ) {
        SDL_Log("failed to load image '%s'", filename.c_str() );
    } else {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels );
    }
    SDL_FreeSurface( image );

    //*
    text_shader_ = shader;
	text_shader_->bind();

    glGenVertexArrays( 1, &vao_ );
    glGenBuffers( 1, &vbo_ );
	glBindVertexArray( vao_ );
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

    GLint posAttrib = glGetAttribLocation( text_shader_->getProgram(), "position");
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0 );

    GLint texAttrib = glGetAttribLocation( text_shader_->getProgram(), "vTexCoord");
    glEnableVertexAttribArray( texAttrib );
    glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)) );
}

void TextRenderer::render()
{
	//buildVertexBuffer();

	// Bind the shader
	text_shader_->bind();
    glBindTexture( GL_TEXTURE_2D, font_bitmap_ );

    // Push the verts to the GPU
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
	glBufferData( GL_ARRAY_BUFFER,
		sizeof(vertex_buffer_[0]) * vertex_buffer_.size(),
		vertex_buffer_.data(),
		GL_DYNAMIC_DRAW 
	);

	// Draw the size of the vertex buffer / the number of components per vert
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_.size() / 4 );

    // Clear the verts
    vertex_buffer_.clear();
}

void TextRenderer::putChar( unsigned char c, float x, float y, unsigned int size_pixels )
{
	static float tex_width = 1.0f / 16.0f;
	static float tex_height = 1.0f / 16.0f;
	float tex_x = (c % 16) * tex_width;
	float tex_y = (c / 16) * tex_height;

	float width = (size_pixels / (float)window_->width()) * 2.0f;
	float height = (size_pixels / (float)window_->height()) * -2.0f;

	pushVert( x, 			y, 			tex_x, 				tex_y );
	pushVert( x + width, 	y + height, tex_x + tex_width, 	tex_y + tex_height );
	pushVert( x, 			y + height, tex_x, 				tex_y + tex_height );
	pushVert( x, 			y, 			tex_x, 				tex_y );
	pushVert( x + width, 	y, 			tex_x + tex_width, 	tex_y );
	pushVert( x + width, 	y + height, tex_x + tex_width, 	tex_y + tex_height );
}

void TextRenderer::pushVert( float x, float y, float u, float v )
{
	vertex_buffer_.push_back( x );
	vertex_buffer_.push_back( y );
	vertex_buffer_.push_back( u );
	vertex_buffer_.push_back( v );
}