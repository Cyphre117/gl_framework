#include "camera.h"

Camera::Camera() :
pos_( 0.0f, 0.5f, -5.0f ),
dir_( 0.0f, 0.0f, 1.0f ),
up_( 0.0f, 1.0f, 0.0f ),
vertical_fov_( glm::radians(45.0f) ),
near_plane_( 0.1f ),
far_plane_( 100.0f ),
input_(nullptr),
timer_(nullptr),
window_(nullptr)
{	
}