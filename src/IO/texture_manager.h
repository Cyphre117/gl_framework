#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "texture_handle.h"
#include <string>
#include <map>

class TextureManager
{
public:
	TextureManager();
	~TextureManager() {}

	void init();
	void shutdown();

	TextureHandle load( std::string filename );

private:
	static bool setup_default_values_;
	static std::string texture_base_path_;
	static const std::string TEXTURE_FOLDER_;
	static const char PATH_SEPERATOR_;
	
	std::map<std::string, GLuint> texture_cache_;

	bool load_bmp( std::string filepath );
};

#endif