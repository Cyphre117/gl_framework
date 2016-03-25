#include "text_renderer.h"
#include "shader_program.h"
#include <string>

TextRenderer::TextRenderer( std::string filename, ShaderProgram* shader ) :
rebuild_verts_( true )
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
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels );
    }
    SDL_FreeSurface( image );

    //*
    text_shader_ = shader;
	text_shader_->bind();

    glGenVertexArrays( 1, &vao_ );
	glBindVertexArray( vao_ );
    glGenBuffers( 1, &vbo_ );

    GLint posAttrib = glGetAttribLocation( text_shader_->getProgram(), "position");
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0 );
}

void TextRenderer::batchRender()
{
	// Only rebuild the vertex buffer when smoething changes
	if( rebuild_verts_ ) {
		buildVertexBuffer();
		rebuild_verts_ = false;
	}

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void TextRenderer::buildVertexBuffer()
{
	pushVert( 0.0f,  0.5f, 0.0f, 0.0f );
	pushVert( 0.5f, -0.5f, 0.0f, 0.0f );
	pushVert( -0.5f, -0.5f, 0.0f, 0.0f );

	glBindVertexArray( vao_ );
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
	glBufferData( GL_ARRAY_BUFFER,
		sizeof(vertex_buffer_[0]) * vertex_buffer_.size(),
		vertex_buffer_.data(),
		GL_DYNAMIC_DRAW 
	);
}

void TextRenderer::pushVert( float x, float y, float u, float v )
{
	vertex_buffer_.push_back( x );
	vertex_buffer_.push_back( y );
}