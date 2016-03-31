#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"

class Input;
class Timer;

class Camera
{
public:
	Camera();
	~Camera() {}

	glm::vec3 position() const { return pos_; }
	glm::vec3 direction() const { return dir_; }
	glm::vec3 up() const { return up_; }
	
	// Returns the camera's view matrix
	glm::mat4 view() const { return glm::lookAt( pos_, pos_ + dir_, up_ ); }
	glm::mat4 projection() const {
		return glm::perspective( vertical_fov_, window_->width() / (float)window_->height(), near_plane_, far_plane_ );
	}

	void update() {
		// apply verticle rotation
		// check for roll over ()
		// apply horizontal rotation

		// apply side to side movement
		// apply forward / back
		// apply up / down
	}

	// Forward, back, left, right, up, down
	// Set to default values in constructor
	void setForwardKey( SDL_Scancode s ) {}
	//void setBackwardKey()
	//void setLeftKey()
	//void setRightKey()
	//void setUpKey()
	//void setDownKey()

	void setPosition( glm::vec3 pos ) { pos_ = pos; }
	void setDirection( glm::vec3 dir ) { dir_ = glm::normalize(dir); }
	void setVerticalFOV( float fov ) { vertical_fov_ = glm::radians(fov); }
	void setNearFar( float near, float far ) { near_plane_ = near; far_plane_ = far; }

	void setInput( Input* input ) { input_ = input; }
	void setTimer( Timer* timer ) { timer_ = timer; }
	void setWindow( Window* window ) { window_ = window; }

private:

	glm::vec3 pos_;
	glm::vec3 dir_;
	glm::vec3 up_;
	glm::vec3 right_; // TODO: CALCULATE ME

	float vertical_fov_;
	float near_plane_;
	float far_plane_;

	Input* input_;
	Timer* timer_;
	Window* window_;
};

#endif