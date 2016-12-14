#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include <audio/audio_buffer.h>
#include <OpenAL/OpenAL.h>

// TODO (create a cpp to implement): 
// - set pitch
// - set gain
// - set position
// - set velocity
// - set looping

class AudioSource
{
public:
	AudioSource() : source_(0) {}
	~AudioSource() {}

	void play( AudioBuffer buffer )
	{
		alSourcei( source_, AL_BUFFER, buffer.buffer() );
		alSourcePlay( source_ );
	}

	ALuint source() const { return source_; }

protected:
	friend class AudioManager;

	ALuint source_;
};

#endif