#include "text_renderer.h"
#include "shader_program.h"
#include <string>

TextRenderer::TextRenderer( std::string filename )
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


    //glBindVertexArray( test_quad_ );
}

void TextRenderer::batchRender()
{
    // Ready the shader
	//text_shader_->bind();
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glDrawArrays(GL_TRIANGLES, 0, 3);
}