#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include <graphics/texture_handle.h>
#include <system/singleton.h>
#include <fstream>
#include <string>
#include <map>

// TODO: Use reference counted textures that the programmer has the responsibilty to load and unload
// when the number of references per texture hits 0 the manager releases the memory
// 
// If i try to make the framework too idiot proof then it will end up being handicapped
// I need to allow the programmer to take responsibility

class TextureManager : public Singleton
{
public:
	TextureManager() { self_ = this; }
	~TextureManager() {}

	bool init() override;		// performs startup
	void shutdown() override;	// deallocates all resources

	// All other subsystems that need access to the texture manager should use this
	static TextureManager* get() { return self_; }

	TextureHandle load( std::string filename );
	void unload( TextureHandle* tex );

private:
	static TextureManager* self_;

	std::string texture_base_path_;									// the path to the folder from which audio is loaded
	std::string extract_filetype( const std::string& filename );	// Returns the file extension after the dot e.g. "png"
	
	std::map<std::string, GLuint> texture_cache_;

	bool load_bmp( std::string filepath );
	bool load_png( std::string filepath );
	bool load_ppm( std::string filepath );
	void read_ppm_number( std::ifstream& file, int& num );
};

#endif