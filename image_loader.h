#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <map>

class ImageLoader
{
public:
	static GLuint load( std::string filename );
private:
	static bool load_bmp( std::string filepath );
	static bool load_png( std::string filepath ) { return false; }
	static bool load_ppm( std::string filepath ) { return false; }

	static std::map<std::string, GLuint> image_cache_;
};

#endif