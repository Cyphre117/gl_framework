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
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels );
    }
    SDL_FreeSurface( image );

    //*
    text_shader_ = shader;
	text_shader_->bind();

    glGenVertexArrays( 1, &vao_ );
	glBindVertexArray( vao_ );
    glGenBuffers( 1, &vbo_ );

    buildVertexBuffer();

    GLint posAttrib = glGetAttribLocation( text_shader_->getProgram(), "position");
    glEnableVertexAttribArray( posAttrib );
    glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0 );

    GLint texAttrib = glGetAttribLocation( text_shader_->getProgram(), "vTexCoord");
    glEnableVertexAttribArray( texAttrib );
    glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)) );
}

void TextRenderer::batchRender()
{
	// Only rebuild the vertex buffer when smoething changes
	if( rebuild_verts_ ) {
		//buildVertexBuffer();
		rebuild_verts_ = false;
	}

	text_shader_->bind();
    glBindTexture( GL_TEXTURE_2D, font_bitmap_ );

	// Draw the size of the vertex buffer / the number of components per vert
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_.size() / 4 );
}

void TextRenderer::buildVertexBuffer()
{
	vertex_buffer_.clear();

	pushVert( -0.5f,  0.5f,  0.0f, 0.0f );
	pushVert(  0.5f, -0.5f,  1.0f, 1.0f );
	pushVert( -0.5f, -0.5f,  0.0f, 1.0f );
	pushVert( -0.5f,  0.5f,  0.0f, 0.0f );
	pushVert(  0.5f,  0.5f,  1.0f, 0.0f );
	pushVert(  0.5f, -0.5f,  1.0f, 1.0f );

	//glBindVertexArray( vao_ );
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
	vertex_buffer_.push_back( u );
	vertex_buffer_.push_back( v );
}