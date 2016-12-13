#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <memory>
#include <SDL2/SDL.h>

#include <graphics/text_renderer.h>
#include <system/window.h>
#include <system/camera.h>
#include <system/input_manager.h>
#include <system/game_time.h>
#include <game.h>

class TextureManager;

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

	// TODO: give all the systems init functions a return flag to check if they were shutdown correctly
	// if they wern't then print a warning and try to shutdown

	// Applicaton components
	TextureManager texture_manager_;
	InputManager input_;
	TextRenderer text_;
	Window window_;
	Camera camera_;
	Time time_;

	std::stack<State*> app_stack_;
};

#endif
