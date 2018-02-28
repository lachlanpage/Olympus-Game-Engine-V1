#ifndef PHYSICSENGINE_H 
#define PHYSICSENGINE_H 
#include <bullet/btBulletDynamicsCommon.h>
#include "Entity.h"
#include "../components/PhysicsComponent.h"
#include "../utilities/GLDebugDrawer.h"
#include <iostream>
class PhysicsEngine {
public:
	PhysicsEngine();
	void Update();
	void addBodies(std::vector<Entity*>);
	btDiscreteDynamicsWorld* getDynamicsWorld();
private:

	GLDebugDrawer debugDrawer;
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	std::map<int, std::pair<Entity*,btRigidBody*>> rigidBodyList;
	std::map<int, std::pair<Entity*, btRigidBody*>>::iterator rigidBodyListIterator;
};

#endif