#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram() {}

	// Returns true if shader compilation was a success, false if there was an error
	bool init();
	void shutdown();

	// Returns true if the source files were loaded successfully, false if they could not be loaded
	bool loadVertexSourceFile( std::string file_path );
	bool loadFragmentSourceFile( std::string file_path ); 

	// Sets the shader source strings directly
	void setVertexSourceString( const std::string& source ) { vertex_source_ = source; }
	void setFragmentSourceString( const std::string& source ) { fragment_source_ = source; }

	void bind();
	GLint getUniformLocation( const GLchar* name );
	GLint getAttribLocation( const GLchar* name );
	GLint getProgram() { return program_; }

	void setTextureFilter( GLint min, GLint mag ) {
		tex_min_filter_ = min;
		tex_mag_filter_ = mag;
	}
	void setTextureWrap( GLint wrap_s, GLint wrap_t ) {
		tex_wrap_s_ = wrap_s;
		tex_warp_t_ = wrap_t;
	}

private:

	// Loads the text file 'filename' and passes the contents to the pointer
	bool load_file( std::string filename, std::string* file_contents  );
	bool did_shader_compile_ok( GLuint shader );

	std::string vertex_source_;
	std::string fragment_source_;

	GLuint program_;
	GLuint vertex_shader_;
	GLuint fragment_shader_;

	GLint tex_wrap_s_;
	GLint tex_warp_t_;
	GLint tex_min_filter_;
	GLint tex_mag_filter_;

};

#endif