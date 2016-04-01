#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <string>

class Window
{
public:
	Window( std::string title, unsigned width, unsigned height );
	~Window();

	void present();
	void clear();

	void updateSizeInfo();
	void enable( GLenum cap ) { glEnable( cap ); }
	void disable( GLenum cap ) { glDisable( cap ); }
	void setBlendFunction( GLenum source_factor, GLenum dest_factor );
	void setClearColour( float r, float g, float b, float a );
	void saveScreenshot( const char * filename );

	int width() const { return width_; }
	int height() const { return height_; }
	SDL_GLContext context() const { return context_; }
	SDL_Window* window() const { return win_; }

private:
	SDL_Window* win_;
	SDL_GLContext context_;

	int width_;
	int height_;
};

#endif