#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <graphics/texture_handle.h>
#include <string>
#include <map>

// TODO: switch this from an instance pre state design to a singleton design
// Use reference counted textures that the programmer has the responsibilty to load and unload
// when the number of references per texture hits 0 the manager releases the memory
// 
// If i try to make the framework too idiot proof then it will end up being handicapped
// I need to allow the programmer to take responsibility

class TextureManager
{
public:
	static TextureManager* get();
	void shutdown();

	TextureHandle load( std::string filename );

private:
	static TextureManager* self_;

	TextureManager() {}
	~TextureManager() {}

	bool init();							// Returns true on success
	static bool setup_default_values_;
	static std::string texture_base_path_;
	//const std::string TEXTURE_FOLDER_;
	//const char PATH_SEPERATOR_;
	
	std::map<std::string, GLuint> texture_cache_;

	bool load_bmp( std::string filepath );
};

#endif