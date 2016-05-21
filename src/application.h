#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <memory>
#include <SDL2/SDL.h>

#include "texture_manager.h"
#include "shader_program.h"
#include "text_renderer.h"
#include "window.h"
#include "camera.h"
#include "input.h"
#include "time.h"
#include "game.h"

class ApplicationState;

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

	// Scenes
	Game game_;

	// TODO: give all the systems init and shutdown functions instead of relying on the constructor and detructor?
	// There could be a flag so check if they were shutdown correctly
	// if they wern't then print a warning and try to shutdown

	// Applicaton components
	TextRenderer text_;
	Window window_;
	Camera camera_;
	Input input_;
	Time time_;
	ShaderProgram text_shader_;
	TextureManager texture_manager_; // TODO: give text shader built in font, remove this

	std::stack<ApplicationState*> app_stack_;
};

#endif