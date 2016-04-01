#include "image_loader.h"
#include <SDL2/sdl.h>

std::map<std::string, GLuint> ImageLoader::image_cache_;

GLuint ImageLoader::load( std::string filename )
{
	// first check the map
	auto it = image_cache_.find( filename );
	if( it != image_cache_.end() ) {
		SDL_Log( "Fetched '%s' from the image cache", filename.c_str() );
		return it->second;
	}

	// If it's not already in the map, load it
	GLuint texture;
	glGenTextures( 1, &texture );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );

    // TODO: resolve the full filepath
    if( load_bmp( filename ) )
    {
    	// Add the new texture to the map
    	image_cache_[filename] = texture;
    	SDL_Log( "Loaded '%s'", filename.c_str() );
    	return texture;
    }
    else
    {
    	SDL_Log( "Could not load '%s'", filename.c_str() );
    	glDeleteTextures( 1, &texture );
    	return 0;
    }
}

bool ImageLoader::load_bmp( std::string filepath )
{
	bool success = false;

	SDL_Surface* image = SDL_LoadBMP( filepath.c_str() );
	// ABGR8888 seems to allign with OpenGLs RGBA format
	SDL_Surface* convertedImage = SDL_ConvertSurfaceFormat( image, SDL_PIXELFORMAT_ABGR8888, 0 );
    if( !image )
    {
        SDL_Log("failed to load image '%s'", filepath.c_str() );
        success = false;
    }
    else
    {
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedImage->pixels );
        success = true;
    }
    SDL_FreeSurface( image );
    SDL_FreeSurface( convertedImage );

    return success;
}