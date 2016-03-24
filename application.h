#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <SDL2/SDL.h>

class ApplicationState;
class ShaderProgram;
class TextRenderer;
class Window;
class Game;

class Application
{
public:
	Application();
	~Application();

	void init();
	void shutdown();

	bool done() { return done_; }

	void frame();

private:

	// Removes quit events from the queue
	void filter_events();

	bool done_;

	Game* game_;
	Window* window_;
	TextRenderer* text_;
	ShaderProgram* text_shader_;

	std::stack<ApplicationState*> app_stack_;
};

#endif