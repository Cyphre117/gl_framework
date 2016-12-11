#include "camera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <system/input.h>
#include <cmath>

const glm::vec3 UP_Y = glm::vec3( 0.0f, 1.0f, 0.0f );

Camera::Camera() :
pos_( 0.0f, 0.5f, 5.0f ),
dir_( 0.0f, 0.0f, -1.0f ),
up_( 0.0f, 1.0f, 0.0f ),
right_( 1.0f, 0.0f, 0.0f ),
vertical_fov_( glm::radians(45.0f) ),
near_plane_( 0.1f ),
far_plane_( 200.0f ),
horizontal_rotate_speed_(0.1f),
vertical_rotate_speed_(0.2f),
vertical_move_speed_(2.0f),
forward_move_speed_(5.0f),
strafe_move_speed_(5.0f),
input_(nullptr),
window_(nullptr)
{	
}

 void Camera::update( float dt )
 {

	// apply vertical rotation
	glm::vec3 new_dir = glm::rotate( dir_, vertical_rotate_speed_ * dt * -input_->yMotion(), right_ );

	// Make sure we aren't rolling over and turning upside down
	if( glm::dot( glm::cross(UP_Y, right_), new_dir ) >= 0.0f )
	{
		dir_ = new_dir;
	}
	else
	{
		// If we are going to go upside down, clamp the direction to verticle
		if( new_dir.y > 0.0f )
			dir_ = UP_Y;
		else
			dir_ = -UP_Y;		
	}

	// apply horizontal rotation
	dir_ = glm::normalize( glm::rotate( dir_, horizontal_rotate_speed_ * -dt * input_->xMotion(), UP_Y ) );
	right_ = glm::normalize( glm::rotate( right_, horizontal_rotate_speed_ * -dt * input_->xMotion(), UP_Y ) );

	// calculate the new up vector
	up_ = glm::cross( right_, dir_ );

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
 	pos_ += vel_ * dt;

	// Reset the velocity read for next update
	//vel_ = glm::vec3( 0.0f, 0.0f, 0.0f );
}

void Camera::setMoveSpeed( float forwardBack, float leftRight, float upDown )
{
	forward_move_speed_ = forwardBack;
	strafe_move_speed_ = leftRight;
	vertical_move_speed_ = upDown;
}

void Camera::setRotateSpeed( float horizontal, float vertical )
{
	horizontal_rotate_speed_ = horizontal;
	vertical_rotate_speed_ = vertical;
}

void Camera::moveForward()
{
	// set the velocity if
	// the old velocity and new velocity are pointing in different directions (away from each other)
	// OR the old velocity is less than the new velocity
	if( ((vel_.x > 0.0f) != (dir_.x > 0.0f)) || std::abs(vel_.x) < std::abs(dir_.x * forward_move_speed_) )
	{
		vel_.x = dir_.x * forward_move_speed_;
	}
	if( (vel_.y > 0.0f) != (dir_.y > 0.0f) || std::abs(vel_.y) < std::abs(dir_.y * forward_move_speed_) )
	{
		vel_.y = dir_.y * forward_move_speed_;
	}
	if( (vel_.z > 0.0f) != (dir_.z > 0.0f) || std::abs(vel_.z) < std::abs(dir_.z * forward_move_speed_) )
	{
		vel_.z = dir_.z * forward_move_speed_;
	}
}

void Camera::moveBackward()
{
	if( ((vel_.x > 0.0f) != (-dir_.x > 0.0f)) || std::abs(vel_.x) < std::abs(dir_.x * forward_move_speed_) )
	{
		vel_.x = -dir_.x * forward_move_speed_;
	}
	if( (vel_.y > 0.0f) != (-dir_.y > 0.0f) || std::abs(vel_.y) < std::abs(dir_.y * forward_move_speed_) )
	{
		vel_.y = -dir_.y * forward_move_speed_;
	}
	if( (vel_.z > 0.0f) != (-dir_.z > 0.0f) || std::abs(vel_.z) < std::abs(dir_.z * forward_move_speed_) )
	{
		vel_.z = -dir_.z * forward_move_speed_;
	}
}

// TODO: make this work like forward
void Camera::moveRight()
{
	vel_ += right_ * strafe_move_speed_;
}

// TODO: make this work like forward
void Camera::moveLeft()
{
	vel_ -= right_ * strafe_move_speed_;
}
