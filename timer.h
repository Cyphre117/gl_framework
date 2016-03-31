#ifndef TIMER_H
#define TIMER_H

#include <SDL2/sdl.h>

class Timer
{
public:

	Timer();
	~Timer() {}

	void update();

	// Returns delta time in seconds
	float dt();
	
	Uint32 ticks();
	Uint32 old_ticks();

private:
	Uint32 ticks_;
	Uint32 old_ticks_;
	
};

#endif