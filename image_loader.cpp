#include "image_loader.h"
#include <SDL2/sdl.h>

std::map<std::string, GLuint> ImageLoader::image_cache_;
std::string ImageLoader::base_path_;

#ifdef _WIN32
	const char PATH_SEPERATOR_ = '\\';
#else
	const char PATH_SEPERATOR_ = '/';
#endif

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

    // resolve the full filepath
    if( base_path_ == "" )
    {
		char *basePath = SDL_GetBasePath();
		// TODO: if this is already the exe dir then things go wrong
		//base_path_ = std::string(basePath);
		SDL_free(basePath);
		SDL_Log("Found exe at location %s", base_path_.c_str() );
    }

    if( load_bmp( base_path_ + filename ) )
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
    if( !image )
    {
        SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Missing file", filepath.c_str(), NULL );
        SDL_Log("failed to load image '%s'", filepath.c_str() );
        success = false;
    }
    else
    {
        // ABGR8888 seems to allign with OpenGLs RGBA format
        SDL_Surface* convertedImage = SDL_ConvertSurfaceFormat( image, SDL_PIXELFORMAT_ABGR8888, 0 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convertedImage->pixels );
        success = true;
        SDL_FreeSurface( convertedImage );
    }
    SDL_FreeSurface( image );

    return success;
}