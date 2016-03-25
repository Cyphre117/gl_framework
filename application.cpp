#include "application.h"
#include "game.h"
#include "input.h"
#include "window.h"
#include "text_renderer.h"
#include "shader_program.h"

Application::Application() :
done_(false),
game_(nullptr),
input_(nullptr),
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
	input_ = new Input();

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
	if( text_ ) {
		delete text_;
		text_ = nullptr;
	}
	if( input_ ) {
		delete input_;
		input_ = nullptr;
	}
}

void Application::frame()
{
	handle_events();

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
	SDL_Event event;
	while( SDL_PollEvent( &event ) ) {
		if( event.type == SDL_QUIT ) done_ = true;
		if( event.type == SDL_WINDOWEVENT_RESIZED ) window_->updateSizeInfo();
		if( event.type == SDL_KEYDOWN )
			if( event.key.keysym.scancode == SDL_SCANCODE_ESCAPE ) done_ = true;
	}
}