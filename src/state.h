#ifndef STATE_H
#define STATE_H

class State
{
public:
	State() {}
	virtual ~State() {}

	virtual bool init() = 0;
	virtual void shutdown() = 0;

	virtual bool frame() = 0;
};

#endif