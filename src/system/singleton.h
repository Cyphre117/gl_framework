#ifndef SINGLETON_H
#define SINGLETON_H

class Singleton
{
public:
	Singleton() { /*self_ = this*/ }
	virtual ~Singleton() { /*do nothing*/ }

	virtual bool init() = 0;
	virtual void shutdown() = 0;
protected:
};

#endif