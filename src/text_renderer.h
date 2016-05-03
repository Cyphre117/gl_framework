#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "shader_program.h"
#include <string>
#include <vector>

class Window;

class TextRenderer
{
public:
	TextRenderer( std::string filename, ShaderProgram* shader );
	~TextRenderer() {}

	void putChar( unsigned char c, float x, float y, unsigned int size_pixels );
	void putString( std::string str, float x, float y, unsigned int size_pixels );

	void render();

	void setWindow( Window* window ) { window_ = window; }

private:
	ShaderProgram* text_shader_;
	Window* window_;
	std::vector<GLfloat> vertex_buffer_;

	GLuint font_bitmap_;
	GLuint vao_;
	GLuint vbo_;

	void pushVert( float x, float y, float u, float v );
};

#endif