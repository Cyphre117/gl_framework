#include "game.h"
#include "input.h"
#include "window.h"
#include "text_renderer.h"

Game::Game() :
window_(nullptr)
{
	// Do all initialisation in init()
	//TODO: do i have to?
	//	This was important for making sure all openGL stuff
	//	was cleanup up before quiting the library
	//	but are theere any other things which are order dependant like that?
	// 	can init and shutdown just be moved to ctor and dtor
}

void Game::init()
{
}

void Game::shutdown()
{
}

bool Game::frame()
{
	if( !update() )
		return false;

	if( !physics() )
		return false;

	if( !graphics() )
		return false;

	return true;
}

bool Game::update()
{
	text_->putChar('a', 0, 0, 32 );
	text_->putChar('b', 0.1, 0, 32 );
	text_->putChar('c', 0.2, 0, 32 );
	text_->putChar(254, 0, 0.2, 32 );
	text_->putString("Hello world!\nHow are you?", -1, 1, 32 );

	for( int s = (int)SDL_SCANCODE_UNKNOWN; s <= (int)SDL_SCANCODE_SLEEP; ++s )
	{
		SDL_Scancode sc = (SDL_Scancode)s;

		if( input_->isPressed( sc ) )
		{
			SDL_Log("%s PRESSED", SDL_GetKeyName( SDL_GetKeyFromScancode( sc ) ) );
		}
		if( input_->isDown( sc ) )
		{
			SDL_Log("%s DOWN", SDL_GetKeyName( SDL_GetKeyFromScancode( sc ) ) );
		}
		if( input_->isReleased( sc ) )
		{
			SDL_Log("%s RELEASED", SDL_GetKeyName( SDL_GetKeyFromScancode( sc ) ) );
		}
	}

	return true;
}

bool Game::graphics()
{
	window_->setClearColour( rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 1.0f );
	window_->clear();

	text_->render();

	window_->present();
	return true;
}

bool Game::physics()
{
	return true;
}