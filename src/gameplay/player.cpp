#include "player.h"
#include <OpenAL/OpenAL.h>
#include <system/input_manager.h>
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>

Player* Player::self_ = nullptr;

Player::Player() :
horizontal_rotate_speed_(0.1f),
vertical_rotate_speed_(0.2f),
vertical_move_speed_(2.0f),
forward_move_speed_(5.0f),
strafe_move_speed_(5.0f)
{
	self_ = this;
}

bool Player::init()
{
	camera_.init();
	input_ = InputManager::get();

	return true;
}

void Player::shutdown()
{
	camera_.shutdown();
}

void Player::update( float dt )
{
	//camera_.update( dt );
	static const glm::vec3 UP_Y = glm::vec3( 0.0f, 1.0f, 0.0f );

	// apply vertical rotation
	glm::vec3 new_dir = glm::rotate( direction(), vertical_rotate_speed_ * dt * -input_->yMotion(), camera_.right() );

	// Make sure we aren't rolling over and turning upside down
	if( glm::dot( glm::cross( UP_Y, camera_.right()), new_dir ) >= 0.0f )
	{
		camera_.setDirection( new_dir );
	}
	else
	{
		// If we are going to go upside down, clamp the direction to verticle
		if( new_dir.y > 0.0f )
			camera_.setDirection( UP_Y );
		else
			camera_.setDirection( -UP_Y );
	}

	// apply horizontal rotation
	camera_.setDirection( glm::normalize( glm::rotate( direction(), horizontal_rotate_speed_ * -dt * input_->xMotion(), UP_Y ) ) );
	camera_.setRight( glm::normalize( glm::rotate( camera_.right(), horizontal_rotate_speed_ * -dt * input_->xMotion(), UP_Y ) ) );

	// calculate the new up vector
	camera_.setUp( glm::cross( camera_.right(), direction() ) );

	// apply side to side movement
	if( input_->isDown( SDL_SCANCODE_D ) )
		moveRight();
	if( input_->isDown( SDL_SCANCODE_A ) )
		moveLeft();

	// apply forward / back
	if( input_->isDown( SDL_SCANCODE_W ) )
		moveForward();
	if( input_->isDown( SDL_SCANCODE_S ) )
		moveBackward();

	
	// apply up / down
	if( input_->isDown( SDL_SCANCODE_E ) )
		vel_ += UP_Y * vertical_move_speed_;
	if( input_->isDown( SDL_SCANCODE_Q ) )
		vel_ -= UP_Y * vertical_move_speed_;

 	// Apply the velocty to the posiiton
 	camera_.setPosition( position() + vel_ * dt );

	updateAudioListener();
}

void Player::updateAudioListener()
{
	alListener3f( AL_POSITION, camera_.position().x, camera_.position().y, camera_.position().z );
	alListener3f( AL_VELOCITY, vel_.x, vel_.y, vel_.z );

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

void Player::setMoveSpeed( float forwardBack, float leftRight, float upDown )
{
	forward_move_speed_ = forwardBack;
	strafe_move_speed_ = leftRight;
	vertical_move_speed_ = upDown;
}

void Player::setRotateSpeed( float horizontal, float vertical )
{
	horizontal_rotate_speed_ = horizontal;
	vertical_rotate_speed_ = vertical;
}

void Player::moveForward()
{
	// set the velocity if
	// the old velocity and new velocity are pointing in different directions (away from each other)
	// OR the old velocity is less than the new velocity
	if( ((vel_.x > 0.0f) != (direction().x > 0.0f)) || std::abs(vel_.x) < std::abs(direction().x * forward_move_speed_) )
	{
		vel_.x = direction().x * forward_move_speed_;
	}
	if( (vel_.y > 0.0f) != (direction().y > 0.0f) || std::abs(vel_.y) < std::abs(direction().y * forward_move_speed_) )
	{
		vel_.y = direction().y * forward_move_speed_;
	}
	if( (vel_.z > 0.0f) != (direction().z > 0.0f) || std::abs(vel_.z) < std::abs(direction().z * forward_move_speed_) )
	{
		vel_.z = direction().z * forward_move_speed_;
	}
}

void Player::moveBackward()
{
	if( ((vel_.x > 0.0f) != (-direction().x > 0.0f)) || std::abs(vel_.x) < std::abs(direction().x * forward_move_speed_) )
	{
		vel_.x = -direction().x * forward_move_speed_;
	}
	if( (vel_.y > 0.0f) != (-direction().y > 0.0f) || std::abs(vel_.y) < std::abs(direction().y * forward_move_speed_) )
	{
		vel_.y = -direction().y * forward_move_speed_;
	}
	if( (vel_.z > 0.0f) != (-direction().z > 0.0f) || std::abs(vel_.z) < std::abs(direction().z * forward_move_speed_) )
	{
		vel_.z = -direction().z * forward_move_speed_;
	}
}

// TODO: make this work like forward
void Player::moveRight()
{
	vel_ += camera_.right() * strafe_move_speed_;
}

// TODO: make this work like forward
void Player::moveLeft()
{
	vel_ -= camera_.right() * strafe_move_speed_;
}