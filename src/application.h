#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <memory>
#include <SDL2/SDL.h>

class ApplicationState;
class TextureManager;
class ShaderProgram;
class TextRenderer;
class Window;
class Camera;
class Input;
class Time;
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
	std::unique_ptr<Game> game_;

	// Applicaton components
	std::unique_ptr<Input> input_;
	std::unique_ptr<Time> time_;
	std::unique_ptr<Window> window_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<TextRenderer> text_;
	std::unique_ptr<ShaderProgram> text_shader_;
	std::unique_ptr<TextureManager> texture_manager_;

	std::stack<ApplicationState*> app_stack_;
};

#endif