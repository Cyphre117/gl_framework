#include "physics_world.h"
#include <system/helpers.h>

PhysicsWorld* PhysicsWorld::self_ = nullptr;

PhysicsWorld::PhysicsWorld()
: collision_config_(nullptr)
, dispatcher_(nullptr)
, broadphase_(nullptr)
, solver_(nullptr)
, world_(nullptr)
, gravity_(0.0f, -9.8f, 0.0f)
{
	self_ = this;
}

bool PhysicsWorld::init()
{
	collision_config_ = new btDefaultCollisionConfiguration();
	dispatcher_ = new btCollisionDispatcher( collision_config_ );
	broadphase_ = new btDbvtBroadphase();
	solver_ = new btSequentialImpulseConstraintSolver();
	world_ = new btDiscreteDynamicsWorld( dispatcher_, broadphase_, solver_, collision_config_ );
	setGravity( gravity_ );

	return true;
}

void PhysicsWorld::shutdown()
{
	DeleteNull( world_ );
	DeleteNull( dispatcher_ );
	DeleteNull( collision_config_ );
	DeleteNull( broadphase_ );
	DeleteNull( solver_ );
}

void PhysicsWorld::update( float dt )
{
	world_->stepSimulation( dt );
}

void PhysicsWorld::setGravity( glm::vec3 g )
{
	gravity_ = g;
	world_->setGravity( btVector3( g.x, g.y, g.z ) );
}