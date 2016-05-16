#include "image_loader.h"
#include <fstream>
#include <SDL2/sdl.h>

std::map<std::string, GLuint> ImageLoader::image_cache_;
std::string ImageLoader::base_path_;

#ifdef _WIN32
	const char ImageLoader::PATH_SEPERATOR_ = '\\';
#else
	const char ImageLoader::PATH_SEPERATOR_ = '/';
#endif

GLuint ImageLoader::load( std::string filename )
{
    SDL_Log( "Extension: %s", extract_filetype(filename).c_str() );

	// first check the map
	auto it = image_cache_.find( filename );
	if( it != image_cache_.end() ) {
		SDL_Log( "Fetched '%s' from the image cache", filename.c_str() );
		return it->second;
	}

	// If it's not already in the map, create a texture load it
    // TODO: postpone generating the OpenGL texture untill the texture has actually been found
    // otherwise return a default texture
	GLuint texture;
	glGenTextures( 1, &texture );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );

    // resolve the full filepath if it hasn't been done already
    if( base_path_ == "" )
    {
		char *basePath = SDL_GetBasePath();
		// TODO: if this is already the exe dir then things go wrong
		//base_path_ = std::string(basePath);
		SDL_free(basePath);
		SDL_Log("Found exe at location %s", base_path_.c_str() );
    }

    std::string filetype = extract_filetype( filename );
    bool loaded = false;

    /*
    // Use the apropriate image loader
    switch( filetype )
    {
        case "bmp":
            loaded = load_bmp( base_path_ + filename );
        break;
        case "ppm":
            loaded = load_ppm( base_path_ + filename );
        break;
        default:
            SDL_Log("Unrecognised filetype");
            return 0;
        break;
    }
    */

    if( loaded )
    {
        // Add the new texture to the cache
        image_cache_[filename] = texture;
        SDL_Log( "Loaded '%s'", filename.c_str() );
        return texture;
    }
    else
    {
        SDL_Log( "Failed to load '%s'", filename.c_str() );
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

bool ImageLoader::load_ppm( std::string filepath )
{
    bool success = false;

    std::ifstream image(filepath);

    if( !image )
    {
        SDL_Log( "Falied to load image: '%s'", filepath.c_str() );
    }

    //std::string magic_number = 


    return false;
}

std::string ImageLoader::extract_filetype( const std::string& filename )
{
    size_t dot = filename.find_last_of('.');

    if( dot == std::string::npos )
    {
        // Did not find a dot
        return "";
    }

    // return everything after the dot
    return filename.substr(dot+1);
}