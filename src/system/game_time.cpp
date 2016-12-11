#include "game_time.h"

Time::Time() :
ticks_(0),
old_ticks_(0)
{}

void Time::update()
{
	old_ticks_ = ticks_;
	ticks_ = SDL_GetTicks();
}

float Time::dt()
{
	return (ticks_ - old_ticks_) / 1000.0f;
}