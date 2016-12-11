#include "text_renderer.h"
#include <graphics/image_loader.h>
#include <graphics/shader_program.h>
#include <system/window.h>
#include <string>

ShaderProgram TextRenderer::default_text_shader_;
int TextRenderer::num_instances_ = 0;

const std::string TextRenderer::default_text_vert_src_ = 
"#version 410 core\n"
"in vec2 vPosition;"
"in vec2 vTexCoord;"
"out vec2 fTexCoord;"
"void main(){"
    "fTexCoord = vTexCoord;"
    "gl_Position = vec4( vPosition, 0.0, 1.0 );"
"}";

const std::string TextRenderer::default_text_frag_src_ =
"#version 150 core\n"
"in vec2 fTexCoord;"
"out vec4 outColour;"
"uniform sampler2D font_bitmap;"
"void main() {"
   	"vec4 sample = texture(font_bitmap, fTexCoord );"
   	"if( sample.g < 1.0f )"
   	"{"
   		"outColour = vec4( 0.0f, 0.0f, 0.0f, 0.0f );"
  	"} else {"
  		"outColour = sample;"
  	"}"
"}";

TextRenderer::TextRenderer() :
text_shader_(nullptr),
window_(nullptr),
vao_(0),
vbo_(0)
{
	// Increment the number of text renderer instances
	num_instances_++;
}

TextRenderer::~TextRenderer()
{
	// Decrement the number of text renderer instances
	num_instances_--;
}

bool TextRenderer::init()
{
	bool success = true;

	// TODO: hardcode the shader default texture

	// If this is the first instance, create the default shader
	if( num_instances_ == 1 )
	{
		default_text_shader_.setVertexSourceString( default_text_vert_src_ );
		default_text_shader_.setFragmentSourceString( default_text_frag_src_ );
		default_text_shader_.init();
	}

	// If the shader is unset use the default shader
	if( text_shader_ == nullptr )
	{
		text_shader_ = &default_text_shader_;
	}

	text_shader_->bind();

    glGenVertexArrays( 1, &vao_ );
    glGenBuffers( 1, &vbo_ );
	glBindVertexArray( vao_ );
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );

	GLint sampler = text_shader_->getUniformLocation( "font_bitmap" );
	if( sampler != -1 )
	{
		glUniform1i( sampler, 0 );
	}
	else success = false;

    GLint posAttrib = text_shader_->getAttribLocation( "vPosition" );
    if( posAttrib != -1 )
	{
		glEnableVertexAttribArray( posAttrib );
		glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0 );
	}
	else success = false;

    GLint texAttrib = text_shader_->getAttribLocation( "vTexCoord" );
	if( posAttrib != -1 )
	{
	    glEnableVertexAttribArray( texAttrib );
	    glVertexAttribPointer( texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (void*)(2*sizeof(GLfloat)) );
	}
	else success = false;

	return success;
}

void TextRenderer::shutdown()
{
	glDeleteBuffers( 1, &vbo_ );
	glDeleteVertexArrays( 1, &vao_ );

	if( num_instances_ == 1 )
	{
		default_text_shader_.shutdown();
	}
}

void TextRenderer::setTexture( TextureHandle texture )
{
	texture.min_filter = GL_NEAREST;
	texture.mag_filter = GL_NEAREST;
	texture_ = texture;
}

void TextRenderer::setShader( ShaderProgram* shader )
{
	//If null is passed go back to using the default shader
	if( shader == nullptr || shader == NULL )
	{
		text_shader_ = &default_text_shader_;
	}
	else
	{
		text_shader_ = shader;		
	}
}

void TextRenderer::render()
{
	// Disable depth testing, text is always on top
	window_->disable( GL_DEPTH_TEST );

	// Bind the shader
	text_shader_->bind();
	texture_.bind( 0 );

    // Push the verts to the GPU
	glBindVertexArray( vao_ );    
	glBindBuffer( GL_ARRAY_BUFFER, vbo_ );
	glBufferData( GL_ARRAY_BUFFER,
		sizeof(vertex_buffer_[0]) * vertex_buffer_.size(),
		vertex_buffer_.data(),
		GL_STREAM_DRAW 
	);

	// Draw the size of the vertex buffer / the number of components per vert
    glDrawArrays(GL_TRIANGLES, 0, vertex_buffer_.size() / 4 );

    // Clear the verts
    vertex_buffer_.clear();

	window_->enable( GL_DEPTH_TEST );
}

void TextRenderer::putString( std::string str, float x, float y, unsigned int size_pixels )
{
	float char_width = (size_pixels / (float)window_->width()) * 2.0f;

	for( size_t i = 0; i < str.size(); ++i )
	{
		// If it's a new line, move down and back
		if( str[i] == '\n' ) {
			y += (size_pixels / (float)window_->height()) * -2.0f;
			x -= char_width * (i + 1);
			continue;
		}

		putChar( str[i], x + char_width * i, y, size_pixels );
	}
}

void TextRenderer::putChar( unsigned char c, float x, float y, unsigned int size_pixels )
{
	static float tex_width = 1.0f / 16.0f;
	static float tex_height = 1.0f / 16.0f;
	float tex_x = (c % 16) * tex_width;
	float tex_y = (c / 16) * tex_height;

	float width = (size_pixels / (float)window_->width()) * 2.0f;
	float height = (size_pixels / (float)window_->height()) * -2.0f;

	pushVert( x, 			y, 			tex_x, 				tex_y );
	pushVert( x + width, 	y + height, tex_x + tex_width, 	tex_y + tex_height );
	pushVert( x, 			y + height, tex_x, 				tex_y + tex_height );
	pushVert( x, 			y, 			tex_x, 				tex_y );
	pushVert( x + width, 	y, 			tex_x + tex_width, 	tex_y );
	pushVert( x + width, 	y + height, tex_x + tex_width, 	tex_y + tex_height );
}

void TextRenderer::pushVert( float x, float y, float u, float v )
{
	vertex_buffer_.push_back( x );
	vertex_buffer_.push_back( y );
	vertex_buffer_.push_back( u );
	vertex_buffer_.push_back( v );
}