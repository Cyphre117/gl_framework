#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"

class Input;

class Camera
{
public:
	Camera();
	~Camera() {}

	//glm::vec3 position() const { return pos_; }
	//glm::vec3 direction() const { return dir_; }
	//glm::vec3 up() const { return up_; }
	
	// Returns the camera's view and projection matrices
	glm::mat4 view() const { return glm::lookAt( pos_, pos_ + dir_, up_ ); }
	glm::mat4 projection() const {
		return glm::perspective( vertical_fov_, window_->width() / (float)window_->height(), near_plane_, far_plane_ );
	}

	void update( float dt );

	// Forward, back, left, right, up, down
	// Set to default values in constructor
	// TODO
	//void setForwardKey( SDL_Scancode s ) {}
	//void setBackwardKey()
	//void setLeftKey()
	//void setRightKey()
	//void setUpKey()
	//void setDownKey()

	void setPosition( glm::vec3 pos ) { pos_ = pos; }
	void setLookAt( glm::vec3 dir ) { } // TODO
	void setVerticalFOV( float fov ) { vertical_fov_ = glm::radians(fov); }
	void setNearFar( float near, float far ) { near_plane_ = near; far_plane_ = far; }
	void setMoveSpeed( float forwardBack, float leftRight, float upDown );
	void setRotateSpeed( float horizontal, float vertical );

	void setInput( Input* input ) { input_ = input; }
	void setWindow( Window* window ) { window_ = window; }

private:

	glm::vec3 pos_;
	glm::vec3 dir_;
	glm::vec3 up_;
	glm::vec3 right_; // TODO: CALCULATE ME

	float vertical_fov_;
	float near_plane_;
	float far_plane_;

	float horizontal_rotate_speed_;
	float vertical_rotate_speed_;
	float vertical_move_speed_;
	float forward_move_speed_;
	float strafe_move_speed_;

	Input* input_;
	Window* window_;
};

#endif