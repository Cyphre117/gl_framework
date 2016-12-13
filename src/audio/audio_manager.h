#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <system/singleton.h>

class AudioManager : public Singleton
{
public:
	AudioManager();
	~AudioManager() {}

	bool init() { return true; }
	void shutdown() {}

	static AudioManager* get() { return self_; }

protected:
	static AudioManager* self_;

};

#endif