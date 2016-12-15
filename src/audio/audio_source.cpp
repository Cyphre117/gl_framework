#include "audio_source.h"
#include <OpenAL/OpenAL.h>

void AudioSource::setPitch( float pitch )
{
	alSourcef( source_, AL_PITCH, pitch );
}

void AudioSource::setGain( float gain )
{
	alSourcef( source_, AL_GAIN, gain );
}

void AudioSource::setLooping( bool enable )
{
	alSourcei( source_, AL_LOOPING, (enable ? AL_TRUE : AL_FALSE) );
}

void AudioSource::setPosition( float x, float y, float z )
{
	alSource3f( source_, AL_POSITION, x, y, z );
}

void AudioSource::setVelocity( float x, float y, float z )
{
	alSource3f( source_, AL_VELOCITY, x, y, z );
}