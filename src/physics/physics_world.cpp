#include "physics_world.h"
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
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

	// Required for ghost object to work correctly
	world_->getPairCache()->setInternalGhostPairCallback( new btGhostPairCallback() );

	return true;
}

void PhysicsWorld::shutdown()
{
	// Delete all the collision objects
	for( int i = world_->getNumCollisionObjects()-1; i >= 0; i-- )
	{
		btCollisionObject* obj = world_->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);

		if( body && body->getMotionState() )
		{
			delete body->getMotionState();
		}

		world_->removeCollisionObject( obj );
		delete obj;
	}

	// TODO: should also delete collision shapes as noted in the bullet quickstart guide?

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

btRigidBody* PhysicsWorld::addRigidBody( glm::vec3 pos, float mass, btCollisionShape* shape )
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin( glmVec3_btVec3(pos) );
    btMotionState* motion = new btDefaultMotionState( transform );

	if( mass != 0.0 )
	{
		btVector3 inertia( 0.0f, 0.0f, 0.0f );
		shape->calculateLocalInertia( mass, inertia );

		btRigidBody::btRigidBodyConstructionInfo info( mass, motion, shape, inertia );
		btRigidBody* rigid_body = new btRigidBody( info );
		world_->addRigidBody( rigid_body );
		return rigid_body;
	}
	else
	{
		// mass == 0.0 threfore it's a static shape, doesn't have inertia
		btRigidBody::btRigidBodyConstructionInfo info( mass, motion, shape );
		btRigidBody* rigid_body = new btRigidBody( info );
		world_->addRigidBody( rigid_body );
		return rigid_body;
	}

}

void PhysicsWorld::setDebugDrawer( btIDebugDraw* debug_drawer )
{
	world_->setDebugDrawer( debug_drawer );
}

void PhysicsWorld::setGravity( glm::vec3 g )
{
	gravity_ = g;
	world_->setGravity( btVector3( g.x, g.y, g.z ) );
}