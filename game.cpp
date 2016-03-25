#include "game.h"
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
	if( !handle_events() )
		return false;

	if( !update() )
		return false;

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

bool Game::update()
{
	text_->putChar('a', 0, 0, 32 );
	text_->putChar('b', 0.1, 0, 32 );
	text_->putChar('c', 0.2, 0, 32 );
	text_->putChar(254, 0, 0.2, 32 );
	text_->putString("Hello world!\nHow are you?", -1, 1, 32 );

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