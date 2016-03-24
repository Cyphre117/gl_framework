#include "window.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

Window::Window( std::string title, unsigned width, unsigned height ) :
width_(0),
height_(0)
{
    // Specify the version of OpenGL we want
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create the window ready for OpenGL rendering
    win_ = SDL_CreateWindow( title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
    context_ = SDL_GL_CreateContext(win_);
    if( context_ == NULL ) {
    	SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
    }

    // Load OpenGL functions
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if( error != GLEW_OK )
    	SDL_Log("Failed to init glew");

    // Get openGL version
    SDL_Log("OpenGL version %s", glGetString( GL_VERSION ) );

	setClearColour( 0.0, 0.0, 0.0, 1.0 );

    SDL_GetWindowSize( win_, &width_, &height_ );
}

Window::~Window()
{	
	SDL_DestroyWindow( win_ );
    SDL_GL_DeleteContext( context_ );
}

void Window::present()
{
    SDL_GL_SwapWindow( win_ );	
}

void Window::clear()
{
    glClear( GL_COLOR_BUFFER_BIT );
}

void Window::setClearColour( float r, float g, float b, float a )
{
    glClearColor( r, g, b, a );
}

void Window::updateSizeInfo()
{
    SDL_GetWindowSize( win_, &width_, &height_ );	
}