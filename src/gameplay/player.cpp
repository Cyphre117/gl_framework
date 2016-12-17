#include "player.h"
#include <OpenAL/OpenAL.h>

Player* Player::self_ = nullptr;

Player::Player()
{
	self_ = this;
}

bool Player::init()
{
	camera_.init();

	return true;
}

void Player::shutdown()
{
	camera_.shutdown();
}

void Player::update( float dt )
{
	camera_.update( dt );

	updateAudioListener();
}

void Player::updateAudioListener()
{
	alListener3f( AL_POSITION, camera_.position().x, camera_.position().y, camera_.position().z );
	alListener3f( AL_VELOCITY, camera_.velocity().x, camera_.velocity().y, camera_.velocity().z );

	ALfloat orientation[] = {
		camera_.position().x + camera_.direction().x,
		camera_.position().y + camera_.direction().y,
		camera_.position().z + camera_.direction().z,
		camera_.up().x,
		camera_.up().y,
		camera_.up().z
	};

	alListenerfv( AL_ORIENTATION, orientation );
}