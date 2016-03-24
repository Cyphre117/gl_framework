#ifndef APPLICATION_STATE_H
#define APPLICATION_STATE_H

class ApplicationState
{
public:
	ApplicationState() {}
	virtual ~ApplicationState() {}

	virtual void init() = 0;
	virtual void shutdown() = 0;

	virtual bool frame() = 0;
};

#endif