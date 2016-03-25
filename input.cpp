#include "input.h"
#include <cstring>

Input::Input() :
keyboard_state_(nullptr),
old_keyboard_state_(nullptr),
num_keys_(0)
{
	keyboard_state_ = SDL_GetKeyboardState( &num_keys_ );
	old_keyboard_state_ = new Uint8[ num_keys_ ];
	SDL_Log("num keys %i", num_keys_ );
}

Input::~Input()
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
}