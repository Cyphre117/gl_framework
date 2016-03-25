#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Input
{
public:
	Input();
	~Input();

	// Saves the current state so we can compare states with last fame
	void store_input();

	bool isDown( SDL_Scancode sc ) { return keyboard_state_[sc]; }
	bool isUp( SDL_Scancode sc ) { return !keyboard_state_[sc]; }
	bool isPressed( SDL_Scancode sc ) { return keyboard_state_[sc] && !old_keyboard_state_[sc]; }
	bool isReleased( SDL_Scancode sc ) { return !keyboard_state_[sc] && old_keyboard_state_[sc]; }

private:
	const Uint8* keyboard_state_;
	Uint8* old_keyboard_state_;
	int num_keys_;
};

#endif