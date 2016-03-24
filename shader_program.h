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
	ShaderProgram( std::string vertex, std::string fragment );
	~ShaderProgram();

	bool good() { return good_; }

	void bind();

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

	// Good if shaders compiled without error
	bool good_;
	std::string load_file( std::string filename );

	GLuint program_;
	GLuint vertex_shader_;
	GLuint fragment_shader_;

	GLint tex_wrap_s_;
	GLint tex_warp_t_;
	GLint tex_min_filter_;
	GLint tex_mag_filter_;

};

#endif