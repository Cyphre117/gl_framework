#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <system/singleton.h>
#include <string>

class Window : public Singleton
{
public:
	Window();
	~Window();

	bool init();
	void shutdown();

	static Window* get() { return self_; }

	void present();
	void clear( GLbitfield mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	void updateSizeInfo();
	void enable( GLenum cap )					{ glEnable( cap ); }
	void disable( GLenum cap )					{ glDisable( cap ); }
	void saveScreenshot( const char * filename );

	void setTitle( std::string title );
	void setSize( int width, int height );
	void setBlendFunction( GLenum source_factor, GLenum dest_factor );
	void setClearColour( float r, float g, float b, float a );
	void setVsync( bool enable );

	int width()							const { return width_; }
	int height()						const { return height_; }
	SDL_GLContext context()				const { return context_; }
	SDL_Window* window()				const { return win_; }

private:
	static Window* self_;

	SDL_Window* win_;
	SDL_GLContext context_;

	std::string title_;
	int width_;
	int height_;
};

#endif
