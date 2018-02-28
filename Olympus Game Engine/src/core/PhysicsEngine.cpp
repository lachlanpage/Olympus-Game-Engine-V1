#include "PhysicsEngine.h"

#include <iostream>

float RAD2DEG = 180.0 / 3.141592653589793238463;

PhysicsEngine::PhysicsEngine() {
	broadphase = new btDbvtBroadphase();
	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	//physics solver
	solver = new btSequentialImpulseConstraintSolver;

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btCollisionShape* fallShape = new btSphereShape(1);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));

	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));

	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);

	//calculate rigid body and add to world
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	dynamicsWorld->addRigidBody(fallRigidBody);
}

void PhysicsEngine::Update() {
	//update each step
	//uncomment to be framerate dependent
	//dynamicsWorld->stepSimulation(1 / 60.f, 10);
	dynamicsWorld->stepSimulation(2*Time::Instance()->getDeltaTime(), 10);

	btTransform trans;

	for (rigidBodyListIterator = rigidBodyList.begin(); rigidBodyListIterator != rigidBodyList.end(); rigidBodyListIterator++) {
		Entity *entity = rigidBodyListIterator->second.first;
		btRigidBody* rigidBody = rigidBodyListIterator->second.second;

		//yaw = y axis
		//pitch = x axis
		//roll = z axis
		entity->setPosition(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
		entity->setRotation(glm::vec3(trans.getRotation().getX()*RAD2DEG, trans.getRotation().getY()*RAD2DEG, trans.getRotation().getZ()*RAD2DEG));
		rigidBody->getMotionState()->getWorldTransform(trans);
	}
}

void PhysicsEngine::addBodies(std::vector<Entity*> entityList) {

	for (auto entity : entityList) {
		if (entity->GetComponent<PhysicsComponent>() != nullptr) {
			int id = entity->m_ID;
			rigidBodyListIterator = rigidBodyList.find(id);
			if (rigidBodyListIterator != rigidBodyList.end()) {
				//rigidbody already exists in map
				//could use this as a way to select bodies potentially
			}
			else {
				dynamicsWorld->addRigidBody(entity->GetComponent<PhysicsComponent>()->getBody());
				rigidBodyList.insert(std::make_pair(id, std::make_pair(entity, entity->GetComponent<PhysicsComponent>()->getBody())));
			}
		}
	}
}

btDiscreteDynamicsWorld* PhysicsEngine::getDynamicsWorld() {
	return dynamicsWorld;
}