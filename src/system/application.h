#ifndef APPLICATION_H
#define APPLICATION_H

#include <stack>
#include <memory>
#include <SDL2/SDL.h>

#include <gameplay/player.h>
#include <graphics/text_renderer.h>
#include <physics/physics_world.h>
#include <system/window.h>
#include <system/input_manager.h>
#include <system/game_time.h>
#include <audio/audio_manager.h>
#include <game.h>

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
	AudioManager audio_manager_;
	TextRenderer text_;
	PhysicsWorld physics_world_;
	Window window_;
	//Camera camera_;
	Player player_;
	Time time_;

	std::stack<State*> app_stack_;
};

#endif
