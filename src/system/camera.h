#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <system/window.h>
#include <system/singleton.h>

class InputManager;

class Camera
{
public:
	Camera();
	~Camera() {}

	bool init();
	void shutdown();

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
	//void setVelocity( glm::vec3 vel ) { vel_ = vel; }
	void setDirection( glm::vec3 dir ) { dir_ = dir; }
	void setUp( glm::vec3 up ) { up_ = up; }
	void setRight( glm::vec3 right ) { right_ = right; }
	void setLookAt( glm::vec3 dir ) { } 				// TODO
	void setVerticalFOV( float fov ) { vertical_fov_ = glm::radians(fov); }
	void setNearFar( float near, float far ) { near_plane_ = near; far_plane_ = far; }

	glm::vec3 position() const { return pos_; }
	//glm::vec3 velocity() const { return vel_; }
	glm::vec3 direction() const { return dir_; }
	glm::vec3 up() const { return up_; }
	glm::vec3 right() const { return right_; }

	void setWindow( Window* window ) { window_ = window; }

private:
	glm::vec3 pos_; // Position
	glm::vec3 dir_; // Direction the camera is facing
	glm::vec3 up_;
	glm::vec3 right_; // TODO: CALCULATE ME

	float vertical_fov_;
	float near_plane_;
	float far_plane_;

	Window* window_;
};

#endif