#include "player.h"
#include <OpenAL/OpenAL.h>
#include <system/input_manager.h>
#include <glm/gtx/rotate_vector.hpp>
#include <cmath>
#include <iostream>

Player* Player::self_ = nullptr;

const float Player::HEIGHT = 1.85f;
const float Player::RADIUS = 0.3f;

Player::Player() :
input_(nullptr),
physics_world_(nullptr),
capsule_(nullptr),
motion_state_(nullptr),
pos_(0.0f, 5.0f, 3.0f),
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
	physics_world_ = PhysicsWorld::get();

	float mass = 50.0;

	btTransform t;
	t.setIdentity();
	t.setOrigin( glmVec3_btVec3(pos_) );
	motion_state_ = new btDefaultMotionState( t );

	btCapsuleShape* capsule_shape = new btCapsuleShape( RADIUS, HEIGHT );
    btVector3 inertia( 0, 0, 0 );
    capsule_shape->calculateLocalInertia( mass, inertia );
	btRigidBody::btRigidBodyConstructionInfo info( mass, motion_state_, capsule_shape, inertia );
	info.m_friction = 5.0f;

	capsule_ = new btRigidBody( info );
	capsule_->setActivationState( DISABLE_DEACTIVATION );
	capsule_->setAngularFactor( 0.0f ); // Prevent rotation
	physics_world_->world()->addRigidBody( capsule_ );

	return true;
}

void Player::shutdown()
{
	camera_.shutdown();
}

void Player::update( float dt )
{
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

	glm::vec3 total_movement(0);

	// apply forward and back movement
	if( input_->isDown( SDL_SCANCODE_W ) )
	{
		glm::vec3 forward( camera_.direction().x, 0, camera_.direction().z );
		forward = glm::normalize( forward );
		forward *= forward_move_speed_;
		total_movement += forward;
	}
	if( input_->isDown( SDL_SCANCODE_S ) )
	{
		glm::vec3 backward( -camera_.direction().x, 0, -camera_.direction().z );
		backward = glm::normalize( backward );
		backward *= forward_move_speed_;
		total_movement += backward;
	}

	// apply left and right moevment
	if( input_->isDown( SDL_SCANCODE_D ) )
	{
		glm::vec3 right = glm::normalize( camera_.right() );
		right *= strafe_move_speed_;
		total_movement += right;
	}
	if( input_->isDown( SDL_SCANCODE_A ) )
	{
		glm::vec3 left = glm::normalize( -camera_.right() );
		left *= strafe_move_speed_;
		total_movement += left;
	}

	if( input_->isDown( SDL_SCANCODE_SPACE ) && vel_.y == 0 )
	{
		capsule_->setLinearVelocity( capsule_->getLinearVelocity() + btVector3(0, 9, 0) );
		jump();
	}

	// Only apply horizontal movement
	glm::vec3 new_vel = glm::vec3( total_movement.x, capsule_->getLinearVelocity().y(), total_movement.z );

 	// Apply the velocty
 	capsule_->setLinearVelocity( glmVec3_btVec3(new_vel) );

	// Update the camera position
	btTransform t;
	motion_state_->getWorldTransform( t );
	pos_ = btVec3_glmVec3( t.getOrigin() );
	camera_.setPosition( pos_ );

	// Update velocity
	vel_ = btVec3_glmVec3(capsule_->getLinearVelocity());

	updateAudioListener();
}

void Player::updateAudioListener()
{
	alListener3f( AL_POSITION, pos_.x, pos_.y, pos_.z );
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

void Player::jump()
{
	// http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Callbacks_and_Triggers
}