#include "game.h"
#include "window.h"
#include "text_renderer.h"

Game::Game() :
window_(nullptr)
{
	// Do all initialisation in init()
}

void Game::init()
{
}

void Game::shutdown()
{
}

bool Game::frame()
{
	if( !handle_events() )
		return false;

	if( !graphics() )
		return false;

	if( !physics() )
		return false;

	return true;
}

bool Game::handle_events()
{
	SDL_Event event;
	while( SDL_PollEvent( &event ) ) {
		if( event.type == SDL_KEYDOWN )
			if( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) return false;
	}

	return true;
}

bool Game::graphics()
{
	window_->clear();

	text_->batchRender();

	window_->present();
	return true;
}

bool Game::physics()
{
	return true;
}