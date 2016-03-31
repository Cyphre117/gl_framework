#include "timer.h"

Timer::Timer() :
ticks_(0),
old_ticks_(0)
{
}

void Timer::update()
{
	old_ticks_ = ticks_;
	ticks_ = SDL_GetTicks();
}

float Timer::dt()
{
	return (ticks_ - old_ticks_) / 1000.0f;
}