#include "application.h"
#include <iostream>
#include <graphics/shader_program.h>
#include <graphics/texture_manager.h>
#include <system/helpers.h>

Application::Application() :
done_(false)
{}

bool Application::init()
{
	// Initialise Libraries
	if( SDL_Init(SDL_INIT_EVERYTHING) )
	{
		SDL_Log( "Error initalising SDL2: %s", SDL_GetError() );
		return false;
	}

	// Initialise systems
	window_.setTitle( "system" );
	window_.setSize( 800, 600 );
	if( !window_.init() ) return false;
	// TODO: get window to use sensible defaults
	// TODO: what if I do this before calling init???
	window_.setVsync( true );

	if( !texture_manager_.init() ) return false;

	if( !audio_manager_.init() ) return false;

	if( !physics_world_.init() ) return false;

	if( !input_.init() ) return false;

	text_.setTexture( texture_manager_.load( "font.png" ) );
	if( !text_.init() ) return false;

	if( !player_.init() ) return false;

	// Initialise states
	game_.setTime( &time_ );
	game_.setTextRenderer( &text_ );
	if( !game_.init() ) return false;

	app_stack_.push( &game_ );

	return true;
}

void Application::shutdown()
{
	// First pop everything off the stack
	while( !app_stack_.empty() ) app_stack_.pop();

	// shutdown scenes
	game_.shutdown();

	// shutdown systems
	player_.shutdown();
	text_.shutdown();
	input_.shutdown();
	physics_world_.shutdown();
	audio_manager_.shutdown();
	texture_manager_.shutdown();
	window_.shutdown();

	// Finally, shutdown libraries
	SDL_Quit();
}

void Application::frame()
{
	handle_events();

	time_.update();

	// Quit if there are no states on the stack
	if( app_stack_.empty() )
	{			
		done_ = true;
		return;
	}

	// If the state returns false, pop it from the stack
	if( !app_stack_.top()->frame() )
	{
		app_stack_.pop();
	}
}

void Application::handle_events()
{
	// store input BEFORE calling SDL_Pump events
	// so it can store the old keyboard values	
	input_.store_input();

	SDL_Event event;
	while( SDL_PollEvent( &event ) ) {
		switch( event.type )
		{
		case SDL_QUIT:
			done_ = true;
			break;

		case SDL_WINDOWEVENT:
			if( event.window.event == SDL_WINDOWEVENT_RESIZED )
				window_.updateSizeInfo();
			break;

		case SDL_KEYDOWN:
			if( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) done_ = true;
			else if( event.key.keysym.scancode == SDL_SCANCODE_LCTRL ) {
				if( input_.isCursorLocked() ) {
					input_.unlockCursor();
				} else input_.lockCursor();
			}
			// TODO: move the screenshot key to something else, what was minecraft again?
			else if( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
				window_.saveScreenshot("screenshot.bmp");
			}
			break;

		case SDL_MOUSEMOTION:
			input_.handle_mouse_event( event );
			break;
		default:
			break;
		}
	}
}