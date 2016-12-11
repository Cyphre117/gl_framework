#include <iostream>
#include <application.h>
#include <shader_program.h>

Application::Application() :
done_(false)
{}

bool Application::init()
{
	// Initialise Libraries
	if( SDL_Init(SDL_INIT_EVERYTHING) )
	{
		std::cout << "Error initalising SDL2: " << SDL_GetError() << std::endl;
		return false;
	}

	// Initialise systems
	if( !window_.init( "system", 800, 600 ) ) return false;

	if( !texture_manager_.init() ) return false;

	input_.setWindow( &window_ );
	input_.init();

	text_.setWindow( &window_ );
	text_.setTexture( texture_manager_.load( "font.bmp" ) );
	if( !text_.init() ) return false;

	camera_.setInput( &input_ );
	camera_.setWindow( &window_ );

	// Initialise states
	game_.setInput( &input_ );
	game_.setTime( &time_ );
	game_.setCamera( &camera_ );
	game_.setWindow( &window_ );
	game_.setTextRenderer( &text_ );
	if( !game_.init() ) return false;

	app_stack_.push( &game_ );

	// TODO: get window to use sensible defaults
	window_.setVsync( true );

	return true;
}

void Application::shutdown()
{
	// First pop everything off the stack
	while( !app_stack_.empty() ) app_stack_.pop();

	// shutdown scenes
	game_.shutdown();

	// shutdown systems
	text_.shutdown();
	input_.shutdown();
	window_.shutdown();
	texture_manager_.shutdown();

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
				window_.saveScreenshot("system_engine_screenshot.bmp");
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
