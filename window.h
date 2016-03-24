#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <string>

class Window
{
public:
	Window( std::string title, unsigned width, unsigned height );
	~Window();

	void present();

	void clear();
	void setClearColour( float r, float g, float b, float a );

	int width() { return width_; }
	int height() { return height_; }
	void updateSizeInfo();

	SDL_GLContext context() { return context_; }

private:
	SDL_Window* win_;
	SDL_GLContext context_;

	int width_;
	int height_;
};

#endif