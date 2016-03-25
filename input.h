#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>

class Input
{
public:
	Input();
	~Input();

	void update();

	bool isDown( SDL_Scancode scancode );
	bool isUp( SDL_Scancode scancode );
	bool isPressed( SDL_Scancode scancode );
	bool isReleased( SDL_Scancode scancode );

private:
	const Uint8* keyboard_state_;
	Uint8* old_keyboard_state_;
	int num_keys_;
};

#endif