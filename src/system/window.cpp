#include "window.h"
#include <vector>

Window::Window() :
width_(0),
height_(0)
{}

Window::~Window() {}

bool Window::init( std::string title, unsigned width, unsigned height )
{
    // Specify the version of OpenGL we want
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create the window ready for OpenGL rendering
    win_ = SDL_CreateWindow( title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

    if( win_ == NULL ) {
        SDL_Log("Could not create SDL_window*: %s\n", SDL_GetError());
        return false;
    }

    context_ = SDL_GL_CreateContext(win_);
    if( context_ == NULL ) {
        SDL_Log("Could not create gl context: %s\n", SDL_GetError());
        return false;
    }

    // Load OpenGL functions
    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if( error != GLEW_OK ) {
        SDL_Log("Failed to init glew");
        return false;
    }

    // Get openGL version
    SDL_Log("OpenGL version %s", glGetString( GL_VERSION ) );

    setClearColour( 0.0, 0.0, 0.0, 1.0 );
    enable( GL_BLEND );
    setBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    enable( GL_DEPTH_TEST );

    SDL_GetWindowSize( win_, &width_, &height_ );

    // If we got to here, assume initialization went fine
    return true;
}

void Window::shutdown()
{
    SDL_DestroyWindow( win_ );
    SDL_GL_DeleteContext( context_ );
}

void Window::present()
{
    SDL_GL_SwapWindow( win_ );	
}

void Window::clear( GLbitfield mask )
{
    glClear( mask );
}

void Window::setClearColour( float r, float g, float b, float a )
{
    glClearColor( r, g, b, a );
}

// https://www.opengl.org/sdk/docs/man2/xhtml/glBlendFunc.xml
void Window::setBlendFunction( GLenum source_factor, GLenum dest_factor )
{
    glBlendFunc( source_factor, dest_factor );
}

void Window::updateSizeInfo()
{
    SDL_GetWindowSize( win_, &width_, &height_ );
    glViewport( 0, 0, width_, height_ );
}

void Window::setVsync( bool enable )
{
    if( enable )
    {
        // Try to use late swap tearing first
        int error = SDL_GL_SetSwapInterval( -1 );

        // If that doesn't work try normal Vsync
        if( error )
        {
            error = SDL_GL_SetSwapInterval( 1 );
        }

        if( error )
        {
            SDL_Log( "Could not enable VSync %s", SDL_GetError() );
        }
    }
    else
    {
        // Pass zero to disable it
        SDL_GL_SetSwapInterval( 0 );
    }
}

void Window::saveScreenshot( const char * filename )
{
    std::vector<unsigned char> pixels;
    pixels.reserve( width_ * height_ * 4 );
    glReadPixels( 0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    SDL_Surface * surf = SDL_CreateRGBSurfaceFrom( pixels.data(), width_, height_, 8 * 4, width_ * 4, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000 );
    SDL_SaveBMP( surf, filename );

    SDL_FreeSurface(surf);
}
