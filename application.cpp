#include "application.h"
#include "game.h"
#include "window.h"
#include "text_renderer.h"
#include "shader_program.h"

Application::Application() :
done_(false),
game_(nullptr),
window_(nullptr),
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
	window_ = new Window( "system", 800, 600 );
	text_shader_ = new ShaderProgram( "shaders/text.vs", "shaders/text.fs" );
	text_ = new TextRenderer( "images/font8.bmp", text_shader_ );
	text_->setWindow( window_ );

	// Initialise states
	game_ = new Game;
	game_->init();
	game_->setWindow( window_ );
	game_->setTextRenderer( text_ );

	app_stack_.push( game_ );
}

void Application::shutdown()
{
	// First pop everything off the stack
	while( !app_stack_.empty() ) app_stack_.pop();

	// Shutdown states
	if( game_ ) {
		game_->shutdown();
		delete game_;
		game_ = nullptr;
	}

	// Shutdown components
	if( window_ ) {
		delete window_;
		window_ = nullptr;
	}
	if( text_shader_ ) {
		delete text_shader_;
		text_shader_ = nullptr;
	}
}

void Application::frame()
{
	filter_events();

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

struct FilterData {
	bool* done_;
	Window* window_;
};

// Filters out global events
int done_if_quit( void* userdata, SDL_Event* event )
{
	FilterData* data = (FilterData*)userdata;

	if( event->type == SDL_QUIT )
	{
		*data->done_ = true;
		return 0;
	}
	else if( event->type == SDL_WINDOWEVENT )
	{
		if( event->window.event == SDL_WINDOWEVENT_RESIZED )
		{
			data->window_->updateSizeInfo();
			SDL_Log("Window is now %i %i", data->window_->width(), data->window_->height() );
			return 0;
		}
	}

	return 1;
}

void Application::filter_events()
{
	SDL_PumpEvents();

	FilterData data;
	data.done_ = &done_;
	data.window_ = window_;

	SDL_FilterEvents( done_if_quit, &data );

	// TODO: Check for window changing size here
}