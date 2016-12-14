#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <OpenAL/OpenAL.h>
#include <system/singleton.h>
#include <audio/audio_buffer.h>
#include <audio/audio_source.h>
#include <string>
#include <map>

// TODO: make audio buffer objects in the audio cache reference counted

class AudioManager : public Singleton
{
public:
	AudioManager();
	~AudioManager() {}

	bool init();
	void shutdown();

	static AudioManager* get() { return self_; }

	AudioBuffer load( std::string filename );
	void unload( AudioBuffer* buffer );

	AudioSource newSource();
	void deleteSource( AudioSource* source );

protected:
	static AudioManager* self_;

	ALCdevice* device_;
	ALCcontext* context_;

	std::map<std::string, ALuint> audio_cache_;

	bool load_wav( std::string filepath, const AudioBuffer& out_buffer );

	std::string extract_filetype( const std::string& filename );
};

#endif