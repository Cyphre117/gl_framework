#include "input.h"
#include <system/window.h>
#include <cstring>

Input::Input() :
keyboard_state_(nullptr),
old_keyboard_state_(nullptr),
num_keys_(0),
cursor_locked_(false)
{}

Input::~Input() {}

void Input::init()
{
	// NOTE: Documentatin suggests this function cannot fail?
	keyboard_state_ = SDL_GetKeyboardState( &num_keys_ );
	old_keyboard_state_ = new Uint8[ num_keys_ ];
}

void Input::shutdown()
{
	if( old_keyboard_state_ ) {
		delete old_keyboard_state_;
		old_keyboard_state_ = nullptr;
	}
}

void Input::store_input()
{
	// Copy last frames keyboard state to the old state array
	std::memcpy( old_keyboard_state_, keyboard_state_, num_keys_ );

	mouse_state_.xrel = 0;
	mouse_state_.yrel = 0;
}

void Input::handle_mouse_event( const SDL_Event& event )
{
	if( event.type == SDL_MOUSEMOTION && cursor_locked_ )
	{
		mouse_state_.xrel += event.motion.xrel;
		mouse_state_.yrel += event.motion.yrel;
	}
}

void Input::lockCursor() { 
    SDL_WarpMouseInWindow( window_->window(), window_->width()/2, window_->height()/2 );
    SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_ShowCursor(SDL_DISABLE);
    cursor_locked_ = true;
}

void Input::unlockCursor() {
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_ShowCursor(SDL_ENABLE);
	cursor_locked_ = false;
}