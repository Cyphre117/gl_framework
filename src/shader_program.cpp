#include "shader_program.h"
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram() :
good_( true ),
tex_wrap_s_( GL_REPEAT ),
tex_warp_t_( GL_REPEAT ),
tex_min_filter_( GL_NEAREST ),
tex_mag_filter_( GL_NEAREST )
{}

void ShaderProgram::init( std::string vertex, std::string fragment )
{
	{
		vertex_shader_ = glCreateShader( GL_VERTEX_SHADER );

	    // Load the shader to a string and convert to const GLchar*
	    std::string source = load_file( vertex );
	    const GLchar* source_ptr = (const GLchar*)source.c_str();
	    
	    glShaderSource( vertex_shader_, 1, &source_ptr, NULL );
	    glCompileShader( vertex_shader_ );
	
	    if( !did_shader_compile_ok( vertex_shader_, vertex ) )
            good_ = false;
	}    
    {
    	fragment_shader_ = glCreateShader( GL_FRAGMENT_SHADER );

        // Load the shader to a string and convert to const GLchar*
        std::string source = load_file( fragment );
        const GLchar* source_ptr = (const GLchar*)source.c_str();
        
        glShaderSource( fragment_shader_, 1, &source_ptr , NULL );
        glCompileShader( fragment_shader_ );

        if( !did_shader_compile_ok( fragment_shader_, fragment ) )
            good_ = false;    
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

        // now cleanup the shaders
        // TODO: it's possible we want to the same vertex/fagment/etc. shader multple times in different programs
        // TODO: find a way of supporting this without the caller having to do extra work
        glDetachShader( program_, vertex_shader_ );
        glDetachShader( program_, fragment_shader_ );
        glDeleteShader( vertex_shader_ );
        glDeleteShader( fragment_shader_ ); 
    }
}

void ShaderProgram::shutdown()
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

GLint ShaderProgram::getUniformLocation( const GLchar* name )
{
    GLint uniform = glGetUniformLocation( program_, name );
    if( uniform == -1 ) {
        SDL_Log("ERROR did not find uniform '%s' in shader program '%i'", name, program_ );
    }
    return uniform;
}


GLint ShaderProgram::getAttribLocation( const GLchar* name )
{
    GLint attribute = glGetAttribLocation( program_, name );
    if( attribute == -1 ) {
        SDL_Log("ERROR did not find attribute '%s' in shader program '%i'", name, program_ );
    }
    return attribute;
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
        SDL_Log("Could not load  %s!", filename.c_str() );

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

bool ShaderProgram::did_shader_compile_ok( GLuint shader, std::string shader_name )
{    
    // Check the shader was compiled succesfully
    GLint status;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    if( status != GL_TRUE )
    {
        // Get the length of the error log
        GLint log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);

        // Now get the error log itself
        GLchar buffer[log_length];
        glGetShaderInfoLog( shader, log_length, NULL, buffer );

        // Print the error
        SDL_Log( "ERROR: compiling fragment shader '%s'", shader_name.c_str() );
        SDL_Log( "%s", buffer );
        return false;
    }

    return true;
}