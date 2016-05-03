#include "application.h"
#include "game.h"
#include "input.h"
#include "time.h"
#include "window.h"
#include "camera.h"
#include "text_renderer.h"
#include "shader_program.h"

Application::Application() :
done_(false),
game_(nullptr),
input_(nullptr),
window_(nullptr),
camera_(nullptr),
text_shader_(nullptr)
{
	SDL_Init(SDL_INIT_EVERYTHING);
}

Application::~Application()
{
	SDL_Quit();
}

void Application::init()
{
	// Initialise components
	window_.reset( new Window( "system", 800, 600 ) );
	text_shader_.reset( new ShaderProgram( "shaders/text.vs", "shaders/text.fs" ) );
	text_.reset( new TextRenderer( "images/font.bmp", text_shader_.get() ) );
	input_.reset( new Input() );
	time_.reset( new Time() );
	camera_.reset( new Camera() );
	text_->setWindow( window_.get() );
	input_->setWindow( window_.get() );
	camera_->setInput( input_.get() );
	camera_->setWindow( window_.get() );

	// Initialise states
	game_.reset( new Game );
	game_->setInput( input_.get() );
	game_->setTime( time_.get() );
	game_->setCamera( camera_.get() );
	game_->setWindow( window_.get() );
	game_->setTextRenderer( text_.get() );
	game_->init();

	app_stack_.push( game_.get() );

	window_->setVsync( true );
}

void Application::shutdown()
{
	// First pop everything off the stack
	while( !app_stack_.empty() ) app_stack_.pop();

	// Shutdown states
	game_->shutdown();
	game_.reset(nullptr);

	// Shutdown components
	window_.reset(nullptr);
	text_shader_.reset(nullptr);
	text_.reset(nullptr);
	input_.reset(nullptr);
	time_.reset(nullptr);
	camera_.reset(nullptr);
}

void Application::frame()
{
	handle_events();

	time_->update();

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
	input_->store_input();

	SDL_Event event;
	while( SDL_PollEvent( &event ) ) {
		switch( event.type )
		{
		case SDL_QUIT:
			done_ = true;
			break;
		case SDL_WINDOWEVENT:
			if( event.window.event == SDL_WINDOWEVENT_RESIZED )
				window_->updateSizeInfo();
			break;
		case SDL_KEYDOWN:
			if( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) done_ = true;
			else if( event.key.keysym.scancode == SDL_SCANCODE_LCTRL ) {
				if( input_->isCursorLocked() ) {
					input_->unlockCursor();
				} else input_->lockCursor();
			}
			else if( event.key.keysym.scancode == SDL_SCANCODE_SPACE ) {
				window_->saveScreenshot("system_engine_screenshot.bmp");
			}
			break;
		case SDL_MOUSEMOTION:
			input_->handle_mouse_event( event );
			break;
		default:
			break;
		}
	}
}