#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Window;

class Input
{
public:
	Input();
	~Input();

	//TODO
	// - mouse position
	// - mouse buttons
	// - mouse movement
	// - hide / show cursor
	// - lock cursor to window

	// Saves the current state so we can compare states with last fame
	void store_input();
	void handle_mouse_event( const SDL_Event& event ); // TODO: possibly change this to handle all events instead of polling the keyboard?
	void lockCursor();
	void unlockCursor();
	void setWindow( Window* window ) { window_ = window; }

	Sint32 xMotion() { return mouse_state_.xrel; }
	Sint32 yMotion() { return mouse_state_.yrel; }
	bool isCursorLocked() { return cursor_locked_; }
	bool isDown( SDL_Scancode sc ) const { return keyboard_state_[sc]; }
	bool isUp( SDL_Scancode sc ) const { return !keyboard_state_[sc]; }
	bool isPressed( SDL_Scancode sc ) const { return keyboard_state_[sc] && !old_keyboard_state_[sc]; }
	bool isReleased( SDL_Scancode sc ) const { return !keyboard_state_[sc] && old_keyboard_state_[sc]; }

private:
	const Uint8* keyboard_state_;
	Uint8* old_keyboard_state_;
	int num_keys_;
	bool cursor_locked_;

	Window* window_;

	struct Mouse {
		Sint32 xrel;
		Sint32 yrel;
	} mouse_state_;
};

#endif