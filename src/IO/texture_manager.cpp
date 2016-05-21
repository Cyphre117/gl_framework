#include "texture_manager.h"
#include "default_texture.h"
#include <SDL2/sdl.h>

bool TextureManager::setup_default_values_ = false;
std::string TextureManager::texture_base_path_ = "";

#ifdef _WIN32
	const char TextureManager::PATH_SEPERATOR_ = '\\';
#else
	const char TextureManager::PATH_SEPERATOR_ = '/';
#endif

TextureManager::TextureManager()
{}

void TextureManager::init()
{
	if( setup_default_values_ == false )
	{
		// Create a default texture
		glBindTexture( GL_TEXTURE_2D, 0 );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_FLOAT, default_texture_data );
		glGenerateMipmap( GL_TEXTURE_2D );

		// Get the base texture path of the application
		char* path = SDL_GetBasePath();
		texture_base_path_ = std::string(path) + "images";
		SDL_free(path);
	}
}

void TextureManager::shutdown()
{
	for( auto it = texture_cache_.begin(); it != texture_cache_.end(); ++it )
	{
		glDeleteTextures( 1, &it->second );
	}
}

TextureHandle TextureManager::load( std::string filename )
{
	TextureHandle texture;

	// First check to see if the image is in the cache
	auto it = texture_cache_.find( filename );
	if( it != texture_cache_.end() ) {
		texture.name_ = it->second;
		return texture;
	}

	// Otherwise try to load the image
	bool loaded = false;
	GLuint name;
	glGenTextures( 1, &name );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, name );

	loaded = load_bmp( texture_base_path_ + PATH_SEPERATOR_ + filename );

	if( loaded )
	{
		// Generate the mip maps
		glGenerateMipmap( GL_TEXTURE_2D );

		// Store the newly loaded image in the cache
		texture_cache_[filename] = name;
		texture.name_ = name;
	}
	else
	{
		SDL_Log( "Could not load '%s'", filename.c_str() );
		glDeleteTextures( 1, &name );
	}

	return texture;
}

bool TextureManager::load_bmp( std::string filepath )
{
	bool success = false;

	SDL_Surface* image = SDL_LoadBMP( filepath.c_str() );

	if( image )
	{
        // ABGR8888 seems to allign with OpenGLs RGBA format
		SDL_Surface* converted_image = SDL_ConvertSurfaceFormat( image, SDL_PIXELFORMAT_ABGR8888, 0 );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted_image->pixels );
        success = true;
        SDL_FreeSurface( converted_image );
	}

	SDL_FreeSurface( image );
	return success;
}