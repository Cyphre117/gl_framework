#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <SDL2/SDL.h>

class ApplicationState;
class ShaderProgram;
class TextRenderer;
class Window;
class Input;
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
	void handle_events();

	bool done_;

	// Application states
	Game* game_;

	// Applicaton components
	// TODO: convert these to unique_ptrs to enforce ownership?
	Input* input_;
	Window* window_;
	TextRenderer* text_;
	ShaderProgram* text_shader_;

	std::stack<ApplicationState*> app_stack_;
};

#endif