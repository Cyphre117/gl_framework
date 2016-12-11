#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <shader_program.h>
#include <IO/texture_handle.h>
#include <string>
#include <vector>

class Window;

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	bool init();
	void shutdown();

	void putChar( unsigned char c, float x, float y, unsigned int size_pixels );
	void putString( std::string str, float x, float y, unsigned int size_pixels );

	void render();

	void setWindow( Window* window ) { window_ = window; }
	void setTexture( TextureHandle texture );
	void setShader( ShaderProgram* shader );

private:
	ShaderProgram* text_shader_;
	Window* window_;
	std::vector<GLfloat> vertex_buffer_;

	TextureHandle texture_;
	GLuint vao_;
	GLuint vbo_;

	static ShaderProgram default_text_shader_;
	static int num_instances_;

	// Default GLSL source for the text renderer shader
	static const std::string default_text_vert_src_;
	static const std::string default_text_frag_src_;

	void pushVert( float x, float y, float u, float v );
};

#endif
