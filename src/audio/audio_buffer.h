#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include <OpenAL/OpenAL.h>

class AudioBuffer
{
public:
	AudioBuffer() : buffer_(0) {}
	~AudioBuffer() {}

	ALuint buffer() const { return buffer_; }

protected:
	friend class AudioManager;

	ALuint buffer_;
};

#endif