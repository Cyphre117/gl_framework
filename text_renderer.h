#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "shader_program.h"
#include <string>

class TextRenderer
{
public:
	TextRenderer( std::string filename, ShaderProgram* shader );
	~TextRenderer() {}

	void putChar() {}
	void putString() {}
	void batchRender();

	void setTextShader( ShaderProgram* shader );

private:
	ShaderProgram* text_shader_;
	GLuint font_bitmap_;

	GLuint vao_;
	GLuint vbo_;

	void buildVertexBuffer();
};

#endif