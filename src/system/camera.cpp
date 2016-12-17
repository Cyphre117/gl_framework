#include "camera.h"
#include <glm/gtx/rotate_vector.hpp>
#include <system/input_manager.h>
#include <OpenAL/OpenAL.h>
#include <cmath>

//const glm::vec3 UP_Y = glm::vec3( 0.0f, 1.0f, 0.0f );

Camera::Camera() :
pos_( 0.0f, 0.5f, 5.0f ),
dir_( 0.0f, 0.0f, -1.0f ),
up_( 0.0f, 1.0f, 0.0f ),
right_( 1.0f, 0.0f, 0.0f ),
vertical_fov_( glm::radians(45.0f) ),
near_plane_( 0.1f ),
far_plane_( 200.0f ),
window_(nullptr)
{	
}

bool Camera::init()
{
	window_ = Window::get();

	return true;
}

void Camera::shutdown()
{

}

void Camera::update( float dt )
{
/*
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
	//vel_ = glm::vec3( 0.0f, 0.0f, 0.0f );*/
}

