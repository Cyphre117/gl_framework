#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <memory>
#include <SDL2/SDL.h>

#include "texture_manager.h"
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
	~Application() {}

	// Returns false if an error occured during initalisation
	bool init();
	void shutdown();

	bool done() { return done_; }

	void frame();

private:
	void handle_events();

	bool done_;

	// Scenes
	Game game_;

	// TODO: give all the systems init and shutdown functions a return flag to check if they were shutdown correctly
	// if they wern't then print a warning and try to shutdown

	// Applicaton components
	TextureManager texture_manager_; // TODO: give text shader built in font, remove this
	TextRenderer text_;
	Window window_;
	Camera camera_;
	Input input_;
	Time time_;

	std::stack<ApplicationState*> app_stack_;
};

#endif