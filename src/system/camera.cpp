#include "camera.h"
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

void Camera::setLookAt( glm::vec3 look_at )
{
	dir_ = glm::normalize( (look_at - pos_) );
}