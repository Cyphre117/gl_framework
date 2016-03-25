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

	// TODO: this should go in an update function
	text_->putChar('a', 0, 0, 32 );
	text_->putChar('b', 0.1, 0, 32 );
	text_->putChar('c', 0.2, 0, 32 );
	text_->putChar(254, 0, 0.2, 32 );

	if( !physics() )
		return false;

	if( !graphics() )
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

	text_->render();

	window_->present();
	return true;
}

bool Game::physics()
{
	return true;
}