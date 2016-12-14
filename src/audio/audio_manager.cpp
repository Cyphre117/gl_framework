#include "audio_manager.h"
#include <SDL2/SDL.h>

AudioManager* AudioManager::self_ = nullptr;

#define AUDIO_FOLDER "audio"

#ifdef _WIN32
	#define PATH_SEPERATOR "\\"
#else
	#define PATH_SEPERATOR "/"
#endif

AudioManager::AudioManager() :
device_(nullptr),
context_(nullptr)
{
	self_ = this;
}

bool AudioManager::init()
{
	if( !device_ )
	{
		device_ = alcOpenDevice(nullptr);
		if( !device_ )
		{
			SDL_Log( "ERROR: filaed to open sound device." );
			return false;
		}
	}

	if( !context_ )
	{
		context_ = alcCreateContext(device_, nullptr);
		if( !context_ )
		{
			SDL_Log( "ERROR: filaed to create audio context." );
			return false;
		}
	}

	alcMakeContextCurrent(context_);

	return true;
}

void AudioManager::shutdown()
{
	// Remove any remaining buffers from the list
	for( auto it = audio_cache_.begin(); it != audio_cache_.end(); ++it )
	{
		alDeleteBuffers( 1, &it->second );
	}

	alcDestroyContext(context_);
	context_ = nullptr;
	alcCloseDevice(device_);
	device_ = nullptr;
}

AudioSource AudioManager::newSource()
{
	AudioSource source;
	alGenSources( 1, &source.source_ );
	return source;
}

void AudioManager::deleteSource( AudioSource* source )
{
	alDeleteSources( 1, &source->source_ );
	source->source_ = 0;
}

AudioBuffer AudioManager::load( std::string filename )
{
	AudioBuffer out_buffer;

	// First check if it's in the cache
	auto it = audio_cache_.find( filename );
	if( it != audio_cache_.end() ) {
		out_buffer.buffer_ = it->second;
		return out_buffer;
	}

	// try to load the file
	bool loaded = false;
	alGenBuffers( 1, &out_buffer.buffer_ );

	std::string extension = extract_filetype( filename );
	if( extension == "wav" )
	{
		loaded = load_wav( filename, out_buffer );
	}
	else
	{
		SDL_Log( "ERROR: unsupported audio extension %s", extension.c_str() );
		if( out_buffer.buffer() )
		{
			alDeleteBuffers( 1, &out_buffer.buffer_ );
			out_buffer.buffer_ = 0;
		}
	}

	if( loaded )
	{
		// If the file was loaded ok, store it in the cache
		audio_cache_[filename] = out_buffer.buffer();


		//ALuint sourceid;
		//alGenSources( 1, &sourceid );
		//alSourcei( sourceid, AL_BUFFER, out_buffer.buffer() );
		//alSourcePlay( sourceid );
	}
	else
	{
		SDL_Log("ERROR: failed to load %s", filename.c_str() );
	}

	return out_buffer;
}

void AudioManager::unload( AudioBuffer* buffer )
{
	for( auto it = audio_cache_.begin(); it != audio_cache_.end(); it++ )
	{
		if( it->second == buffer->buffer() )
		{
			alDeleteBuffers( 1, &buffer->buffer_ );
			buffer->buffer_ = 0;

			audio_cache_.erase(it);
			return;
		}
	}
}

bool AudioManager::load_wav( std::string filepath, const AudioBuffer& out_buffer )
{
	bool success = true;
	
	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 *wav_buffer;

	if( !SDL_LoadWAV( "./audio/powerup.wav", &wav_spec, &wav_buffer, &wav_length ) )
	{
		SDL_Log( "ERROR: could not load audio file" );
		success = false;
		return success;
	}

	ALenum format = 0;

	// Figure out what type of audio we have loaded
	// TODO: if the format is not natively supported by OpenAL (8 bit unsigned and 16 bit signed) the we should do a conversion
	// formats that should be supported include signed 8bit unsigned 16bit, signed and unsigned 32 bit

	switch( wav_spec.channels )
	{
	case 1: // Mono audio
		switch( wav_spec.format )
		{
			case AUDIO_U8:
				format = AL_FORMAT_MONO8;
			break;
			case AUDIO_S16:
				format = AL_FORMAT_MONO16;
			break;
			default:
			SDL_Log("ERROR: Audio has unsupported format: %d", wav_spec.format );
			success = false;
			break;
		}
	break;
	case 2: // Stereo (does not support 3d positioning)
		switch( wav_spec.format )
		{
			case AUDIO_U8:
				format = AL_FORMAT_MONO8;
			break;
			case AUDIO_S16:
				format = AL_FORMAT_STEREO16;
			break;
			default:
			SDL_Log("ERROR: Audio has unsupported format: %d", wav_spec.format );
			success = false;
			break;
		}
	break;
	default:
	SDL_Log("ERROR: unsupported number of audio channels: %d", wav_spec.channels );
	success = false;
	break;
	}

	alBufferData( out_buffer.buffer(), format, wav_buffer, wav_length, wav_spec.freq );

	// Now we have stored the wav data in the buffer we can free it
	SDL_FreeWAV( wav_buffer );

	return success;
}

std::string AudioManager::extract_filetype( const std::string& filename )
{
    size_t dot = filename.find_last_of('.');

    if( dot == std::string::npos )
    {
        // Did not find a dot
        return "";
    }

    // return everything after the dot
    return filename.substr(dot+1);
}