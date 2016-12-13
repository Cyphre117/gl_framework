#include "texture_manager.h"
#include <graphics/default_texture.inl>
#include <graphics/pico_png.inl>
#include <SDL2/sdl.h>

TextureManager* TextureManager::self_ = nullptr;

// The folder to look in relative to the base path
#define TEXTURE_FOLDER "images"

#ifdef _WIN32
	#define PATH_SEPERATOR "\\"
#else
	#define PATH_SEPERATOR "/"
#endif

void TextureManager::shutdown()
{
	for( auto it = texture_cache_.begin(); it != texture_cache_.end(); ++it )
	{
		glDeleteTextures( 1, &it->second );
	}
}

bool TextureManager::init()
{
	bool success = true;

	// Create a default texture
	glBindTexture( GL_TEXTURE_2D, 0 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, 16, 16, 0, GL_RGB, GL_FLOAT, default_texture_data );
	glGenerateMipmap( GL_TEXTURE_2D );

	// Get the base texture path of the application
	char* path = SDL_GetBasePath();
	if( path == NULL ) {
		SDL_Log("Could not get texture base path: %s\n", SDL_GetError());
		success = false;
	}
	texture_base_path_ = std::string(path) + TEXTURE_FOLDER + PATH_SEPERATOR;
	SDL_free(path);

	return success;
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

	// Send to the apropriate loader function
	std::string extension = extract_filetype( filename );
	if( extension == "bmp" )
	{
		loaded = load_bmp( texture_base_path_ + filename );
	}
	else if( extension == "png" )
	{
		loaded = load_png( texture_base_path_ + filename );
	}
	else if( extension == "ppm" || extension == "pbm" || extension == "pgm" )
	{
		loaded = load_ppm( texture_base_path_ + filename );
	}

	// Check if the file was loaded ok
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
		SDL_Log( "Could not load '%s%s'", texture_base_path_.c_str(), filename.c_str() );
		glDeleteTextures( 1, &name );
	}
	return texture;
}

void TextureManager::unload( TextureHandle tex )
{
	for( auto it = texture_cache_.begin(); it != texture_cache_.end(); it++ )
	{
		if( it->second == tex.name_ )
		{
			// If we found the texture handle, clean it up and erase it from the cache
			glDeleteTextures( 1, &tex.name_);

			texture_cache_.erase(it);
			return;
		}
	}
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

bool TextureManager::load_png( std::string filepath )
{
	bool success = false;

	// Load the file into a vector
	std::ifstream file( filepath.c_str(), std::ios::in|std::ios::binary|std::ios::ate);

	// Get the file size
	std::streamsize size = 0;
	if(file.seekg(0, std::ios::end).good()) size = file.tellg();
	if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	// If the file was empty, quit
	if( size <= 0 ) {
		return success;
	}

	// Read the file into a buffer
	std::vector<unsigned char> buffer;
	buffer.resize( size );
	file.read( (char*)buffer.data(), size );

	// Decode the file into another buffer
	std::vector<unsigned char> image;
	unsigned long width = 0, height = 0;
	int error = decodePNG( image, width, height, buffer.data(), buffer.size() );

	if( width != height )
	{
		SDL_Log("png image %s is not square!", filepath.c_str());
		return success;
	}

	if( error == 0 )
	{
		success = true;
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data() );
	}

	return success;
}

bool TextureManager::load_ppm( std::string filepath )
{
	bool success = false;

	// Load the file into a vector
	std::ifstream file( filepath.c_str() );

	// Get the file size
	std::streamsize size = 0;
	if(file.seekg(0, std::ios::end).good()) size = file.tellg();
	if(file.seekg(0, std::ios::beg).good()) size -= file.tellg();

	// If the file was empty, quit
	if( size <= 2 ) {
		return success;
	}

	// The first two chars of a ppm are the magic number, consisting of a 'P' followed by 1-6 depending on the type
	if( file.get() != 'P' ) {
		SDL_Log(".ppm had unknown magic number");
		return success;
	}

	std::vector<unsigned char> image;
	char type = file.get();
	int width = 0, height = 0, depth = 0;

	// TODO: figure out how to decode binary ppm types

	read_ppm_number( file, width );
	read_ppm_number( file, height );

	// Ensure the image is square
	if( width != height )
	{
		SDL_Log("ppm image %s is not square!", filepath.c_str());
		return success;
	}

	// Type 1 is a black and white 1 bit image
	if( type == '1' )
	{
		std::string s;

		while( file )
		{
			s = file.get();
			if( s[0] == '0') {
				image.push_back(255);
			} else if( s[0] == '1' ) {
				image.push_back(0);
			} else if( s[0] == '#') {
				std::getline( file, s );
			}
		}
	}

	// types 2 and 3 are grey and rgb respectively
	if( type == '2' || type == '3' )
	{
		read_ppm_number( file, depth );
	
		std::string s;

		while( file )
		{
			file >> s;
			if( s == "#" ) {
				std::getline( file, s );
			} else if( !s.empty() ) {
				image.push_back( (std::stoi( s ) / (float)depth) * 255 );
			}
		}
	}

	switch( type )
	{
	case '1': // ASCII 0-1 (black & white)
	case '2': // ASCII 0-255 (grey)
		success = true;
		// 1 channel data
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, image.data() );
	break;
	case '3': // ASCII 0-255 x3 (RGB)
		success = true;
		// 3 channel data
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.data() );
	break;
	case '4': // Binary 0-1 (black & white)
		SDL_Log("ERROR: cannot decode binary ppm type %d", type);
	break;
	case '5': // Binary 0-255 (grey)
		SDL_Log("ERROR: cannot decode binary ppm type %d", type);
	break;
	case '6': // Binary 0-255 x3 (RGB)
		SDL_Log("ERROR: cannot decode binary ppm type %d", type);
	break;
	default: // Unknown
		SDL_Log("ERROR: Trying to decode .ppm file with unknown magic number");
	break;	
	}

	return success;
}

void TextureManager::read_ppm_number( std::ifstream& file, int& num )
{
	std::string s;
	while( file ) { // read untill we got the width
		file >> s;
		if( s == "#" ) {
			std::getline( file, s );
		} else {
			num = std::stoi(s);
			break;
		}
	}
}

std::string TextureManager::extract_filetype( const std::string& filename )
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