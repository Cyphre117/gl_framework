#include "shader_program.h"
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram( std::string vertex, std::string fragment ) :
good_( true ),
tex_wrap_s_( GL_REPEAT ),
tex_warp_t_( GL_REPEAT ),
tex_min_filter_( GL_NEAREST ),
tex_mag_filter_( GL_NEAREST )
{
	{
		vertex_shader_ = glCreateShader( GL_VERTEX_SHADER );

	    // Load the shader to a string and convert to const GLchar*
	    std::string source = load_file( vertex );
	    const GLchar* source_ptr = (const GLchar*)source.c_str();
	    
	    glShaderSource( vertex_shader_, 1, &source_ptr, NULL );
	    glCompileShader( vertex_shader_ );
	
	    // Check the shader was compiled correctly
	    GLint status;
	    glGetShaderiv( vertex_shader_, GL_COMPILE_STATUS, &status );
	    if( status != GL_TRUE )
	    {
	        // There was an error compiling the shader
	        char buffer[512];
	        glGetShaderInfoLog( vertex_shader_, 512, NULL, buffer );
	        SDL_Log( "ERROR compiling vertex shader '%s' ", vertex.c_str() );
	        SDL_Log( "%s", buffer );
	        good_ = false;
	    }
	    //else SDL_Log("Compiled vertex Shader '%s'", vertex.c_str() );
	}    
    {
    	fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);

        // Load the shader to a string and convert to const GLchar*
        std::string source = load_file( fragment );
        const GLchar* source_ptr = (const GLchar*)source.c_str();
        
        glShaderSource( fragment_shader_, 1, &source_ptr , NULL );
        glCompileShader( fragment_shader_ );
    
        // Check the shader was compiled succesfully
        GLint status;
        glGetShaderiv( fragment_shader_, GL_COMPILE_STATUS, &status );
        if( status != GL_TRUE )
        {
            char buffer[512];
            glGetShaderInfoLog( fragment_shader_, 512, NULL, buffer );
            SDL_Log( "ERROR compiling fragment shader '%s'", fragment.c_str() );
            SDL_Log( "%s", buffer );
            good_ = false;
        }
        //else SDL_Log("Compiled fragment Shader '%s'", fragment.c_str() );
    }

    // Check the vertex and fragment shaders are good
    if( vertex_shader_ == 0 || fragment_shader_ == 0 ) good_ = false;

    if( good_ ) {
    	 // Create the shader program, attach the vertex and fragment shaders
	    program_ = glCreateProgram();
	    glAttachShader( program_, vertex_shader_ );
	    glAttachShader( program_, fragment_shader_ );
	    glBindFragDataLocation( program_, 0, "outColour" );
	    glLinkProgram( program_ );

	    SDL_Log("Compiled %s and %s", vertex.c_str(), fragment.c_str() );
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram( program_ );
    glDeleteShader( vertex_shader_ );
    glDeleteShader( fragment_shader_ );
}

void ShaderProgram::bind()
{
	glUseProgram( program_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex_wrap_s_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex_warp_t_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex_min_filter_ );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex_mag_filter_ );
}

// TODO: convert this to SDL rw ops and move to seperate header file
std::string ShaderProgram::load_file( std::string filename )
{
    std::string projectPath;
    
    char* charPath = SDL_GetBasePath();    
    projectPath = charPath;    
    SDL_free(charPath);

    std::ifstream file( projectPath + filename );
    if( !file.good() )
        //SDL_Log("Loaded %s", filename.c_str() );
    //else 
        SDL_Log("Could not load  %s!", filename.c_str() );

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}