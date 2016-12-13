#include "audio_manager.h"

AudioManager* AudioManager::self_ = nullptr;

AudioManager::AudioManager()
{ self_ = this; }