#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <system/singleton.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

class PhysicsWorld : public Singleton
{
public:
	PhysicsWorld();
	~PhysicsWorld() {}

	bool init();
	void shutdown();

	static PhysicsWorld* get() { return self_; }

	void update( float dt );
	btRigidBody* addRigidBody( glm::vec3 pos, float mass, btCollisionShape* shape );

	// Setters
	void setDebugDrawer( btIDebugDraw* debug_drawer );
	void setGravity( glm::vec3 g );

	// Getters
	glm::vec3 gravity() const { return gravity_; }
	btDynamicsWorld* world() const { return world_; }

protected:
	static PhysicsWorld* self_;

	// Bullet physics objects
	btCollisionConfiguration*	collision_config_;
	btDispatcher*				dispatcher_;
	btBroadphaseInterface*		broadphase_;
	btConstraintSolver*			solver_;
	btDynamicsWorld*			world_;

	glm::vec3 gravity_;
};

#endif