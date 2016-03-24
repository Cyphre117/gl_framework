#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "shader_program.h"
#include <string>

class TextRenderer
{
public:
	TextRenderer( std::string filename );
	~TextRenderer() {}

	void putChar() {}
	void putString() {}
	void batchRender();

	void setTextShader( ShaderProgram* shader ) { text_shader_ = shader; }

private:
	ShaderProgram* text_shader_;
	GLuint font_bitmap_;

	GLuint test_quad_;
};

#endif