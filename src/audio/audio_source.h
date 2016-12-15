#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include <audio/audio_buffer.h>
#include <OpenAL/OpenAL.h>

// TODO (create a cpp to implement): 
// - set pitch
// - set gain
// - set position
// - set velocity
// - static volume to multiply all samples?

// For more information on how each attribute works see the OpenAl specification
// Section 4.3.2 deals with source attributes

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

	// 1.0f is the default pitch, 0.0f is an invalid value
	// Doubling increases by one octave, halfing decreases by one octave
	void setPitch( float pitch );

	// in range [0.0f, 1.0f]
	void setGain( float gain );

	// Can be set on playing sources
	void setLooping( bool enable );

	void setPosition( float x, float y, float z );

	void setVelocity( float x, float y, float z );

	ALuint source() const { return source_; }

protected:
	friend class AudioManager;

	ALuint source_;
};

#endif